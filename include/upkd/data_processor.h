#ifndef HERE_IS_JOHNNY_V_DATA_PROCESSOR_H
#define HERE_IS_JOHNNY_V_DATA_PROCESSOR_H

/**
 * @brief Parses and processes a concatenated string of received data.
 *
 * Data comes divided into packets. This function converts the concatenated value of data, checks for authentication,
 * and processes commands, shell scripts, or file data based on specific tags within the converted data.
 *
 * @param receivedData The concatenated value string of received data to be parsed.
 * @param mac_src The source MAC address associated with the concatenated value.
 * @param ip_src The source IP address associated with the concatenated value.
 */
void process_received_data(const char *receivedData, const char *mac_src, const char *ip);

#endif //HERE_IS_JOHNNY_V_DATA_PROCESSOR_H
