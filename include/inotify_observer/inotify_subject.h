#ifndef HERE_IS_JOHNNY_V_INOTIFY_SUBJECT_H
#define HERE_IS_JOHNNY_V_INOTIFY_SUBJECT_H

#include "inotify_observer.h"

/**
 * @brief Sets up an inotify watch_fs on the specified path and notifies observers on events.
 *
 * This function initializes an inotify instance to monitor the specified path for events
 * matching the provided masks. When an event occurs, it notifies the registered observers.
 *
 * @param path The fileToObserve or directory path to monitor.
 * @param masks An array of event masks to watch_fs for (e.g., IN_MODIFY).
 * @param numMasks The number of event masks in the masks array.
 * @param observers An array of pointers to observers to be notified of events.
 * @param numObservers The number of observers in the observers array.
 */
void observe(const char *path, uint32_t *masks, int numMasks, InotifyObserver **observers, int numObservers);

#endif // HERE_IS_JOHNNY_V_INOTIFY_SUBJECT_H
