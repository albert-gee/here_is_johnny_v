#ifndef HERE_IS_JOHNNY_V_UPKD_H
#define HERE_IS_JOHNNY_V_UPKD_H

#define LOG_PATH "/var/log/ufw.log"
#define BUF_SIZE 1024
#define MAX_DATA_LENGTH 400
#define DECRYPTED_BLOCK_SIZE 15

/**
* @brief Observes the UFW log file for modifications and processes new log entries.
*
* This function opens the UFW log file, backtracks to the last line, and sets up an inotify observer
* to monitor the file for modifications. When the log file is modified, it processes new log entries
* for connection knocks.
*/
void observe_ufw_log();

#endif // HERE_IS_JOHNNY_V_UPKD_H
