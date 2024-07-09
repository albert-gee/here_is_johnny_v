#ifndef HERE_IS_JOHNNY_V_INOTIFY_OBSERVER_H
#define HERE_IS_JOHNNY_V_INOTIFY_OBSERVER_H

#include <sys/inotify.h>

/**
 * @struct InotifyObserver
 * @brief A structure to observe inotify events.
 *
 * This structure is used to observe inotify events. It defines an observer in the observer pattern,
 * which contains a handleEvent function to handle inotify events.
 *
 * @see inotify_subject.h for the subject in the observer pattern.
 */
typedef struct {
    void (*handleEvent)(struct inotify_event *event); /**< Function pointer to handle inotify events */
} InotifyObserver;

/**
 * @brief Returns the name of the event mask.
 *
 * This function returns the name of the event mask.
 *
 * @param mask The event mask.
 * @return The name of the event mask.
 */
char *getEventName(uint32_t mask);

#endif // HERE_IS_JOHNNY_V_INOTIFY_OBSERVER_H
