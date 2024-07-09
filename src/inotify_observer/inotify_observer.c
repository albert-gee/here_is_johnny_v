#include <sys/inotify.h>

/**
 * @brief Returns the name of the event mask.
 *
 * This function returns the name of the event mask.
 *
 * @param mask The event mask.
 * @return The name of the event mask.
 */
char *getEventName(uint32_t mask) {
    if (mask & IN_ACCESS) {
        return "IN_ACCESS";
    }
    if (mask & IN_MODIFY) {
        return "IN_MODIFY";
    }
    if (mask & IN_ATTRIB) {
        return "IN_ATTRIB";
    }
    if (mask & IN_CLOSE_WRITE) {
        return "IN_CLOSE_WRITE";
    }
    if (mask & IN_CLOSE_NOWRITE) {
        return "IN_CLOSE_NOWRITE";
    }
    if (mask & IN_OPEN) {
        return "IN_OPEN";
    }
    if (mask & IN_MOVED_FROM) {
        return "IN_MOVED_FROM";
    }
    if (mask & IN_MOVED_TO) {
        return "IN_MOVED_TO";
    }
    if (mask & IN_CREATE) {
        return "IN_CREATE";
    }
    if (mask & IN_DELETE) {
        return "IN_DELETE";
    }
    if (mask & IN_DELETE_SELF) {
        return "IN_DELETE_SELF";
    }
    if (mask & IN_MOVE_SELF) {
        return "IN_MOVE_SELF";
    }
    return "UNKNOWN";
}