#ifndef HERE_IS_JOHNNY_V_FS_MONITOR_COMMANDS_H
#define HERE_IS_JOHNNY_V_FS_MONITOR_COMMANDS_H

/**
 * @brief Watches the file system for changes.
 *
 * This function watches the file system for changes and sends noise to the server when a change is detected.
 *
 * @param path The path to the directory to be monitored.
 */
void watch_fs(const char *path);

#endif //HERE_IS_JOHNNY_V_FS_MONITOR_COMMANDS_H
