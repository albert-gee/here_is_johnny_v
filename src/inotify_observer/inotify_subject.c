#include "../../include/inotify_observer/inotify_subject.h"
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>

#define EVENT_SIZE (sizeof(struct inotify_event))   // Size of one inotify_event structure
#define BUF_LEN (1024 * (EVENT_SIZE + 16))          // Buffer size to read inotify events

// Static function prototypes

/**
 * @brief Reads events from the inotify file descriptor and notifies all registered observers.
 *
 * This function reads events from the inotify file descriptor and processes them in a loop.
 * For each event, it calls the `handleEvent` method of all registered observers.
 *
 * @param inotifyFileDescriptor The file descriptor for the inotify instance.
 * @param observers An array of pointers to `InotifyObserver` instances that will handle the events.
 * @param numObservers The number of observers in the `observers` array.
 */
static void handle_events(int inotifyFileDescriptor, InotifyObserver **observers, int numObservers);

/**
 * @brief Initializes the inotify instance and adds watches for the specified events.
 *
 * This function creates an inotify instance, adds watches for the specified path and event masks,
 * and returns the file descriptor for the inotify instance.
 *
 * @param path The path to monitor for file system events.
 * @param masks An array of event masks to watch for.
 * @param numMasks The number of event masks in the `masks` array.
 * @return The file descriptor for the inotify instance, or -1 if an error occurred.
 */
static int initialize_inotify(const char *path, uint32_t *masks, int numMasks);

// Public functions

void observe(const char *path, uint32_t *masks, int numMasks, InotifyObserver **observers, int numObservers) {
    int inotifyFileDescriptor = initialize_inotify(path, masks, numMasks);
    handle_events(inotifyFileDescriptor, observers, numObservers);
    close(inotifyFileDescriptor);
}

// Static functions

static void handle_events(int inotifyFileDescriptor, InotifyObserver **observers, int numObservers) {
    char buffer[BUF_LEN]; // Buffer to store raw event data read from the inotify file descriptor

    // Continuously read events from the inotify file descriptor
    while (1) {
        // Read events from the inotify file descriptor. The read operation is non-blocking due to IN_NONBLOCK flag.
        ssize_t length = read(inotifyFileDescriptor, buffer, BUF_LEN);
        if (length < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No events to read; continue the loop
                continue;
            } else {
                perror("read");
                break;
            }
        }

        // Index to keep track of the current position in the buffer
        unsigned long i = 0;
        while (i < length) {
            // Cast the current position in the buffer to an inotify_event structure
            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            // Notify all observers for each event
            for (int j = 0; j < numObservers; j++) {
                observers[j]->handleEvent(event);
            }

            // Move the index to the next event in the buffer
            i += EVENT_SIZE + event->len;
        }
    }
}

static int initialize_inotify(const char *path, uint32_t *masks, int numMasks) {
    // Create an inotify instance with non-blocking mode
    int inotifyFileDescriptor = inotify_init1(IN_NONBLOCK);
    if (inotifyFileDescriptor < 0) {
        perror("inotify_init1");
        return -1;
    }

    // Add a watch for each specified event mask
    for (int i = 0; i < numMasks; i++) {

        // Add a watch to the inotify instance for the specified path and event mask
        int watchFileDescriptor = inotify_add_watch(inotifyFileDescriptor, path, masks[i]);
        if (watchFileDescriptor == -1) {
            perror("inotify_add_watch");
            close(inotifyFileDescriptor);
            return -1;
        }
    }

    // Return the file descriptor for the inotify instance
    return inotifyFileDescriptor;
}