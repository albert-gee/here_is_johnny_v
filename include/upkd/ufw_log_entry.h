#ifndef HERE_IS_JOHNNY_V_UFW_LOG_ENTRY_H
#define HERE_IS_JOHNNY_V_UFW_LOG_ENTRY_H

/**
 * @brief Structure to hold UFW log entry details.
 *
 * This structure contains fields representing various components of a UFW log entry,
 * such as network interfaces, IP addresses, packet details, and flags.
 */
struct UfwLogEntry {
    char in_interface[20];  /**< The input network interface */
    char out_interface[20]; /**< The output network interface */
    char mac_address[42];   /**< The MAC address */
    char src_ip[16];        /**< The source IP address */
    char dst_ip[16];        /**< The destination IP address */
    int length;             /**< The length of the packet */
    char tos[6];            /**< The type of service */
    char prec[6];           /**< The precedence */
    int ttl;                /**< The time-to-live value */
    int id;                 /**< The packet ID */
    char protocol[10];      /**< The protocol used */
    int spt;                /**< The source port */
    int dpt;                /**< The destination port */
    int window;             /**< The window size */
    char res[6];            /**< Reserved field */
    char syn[4];            /**< SYN flag */
    char urg_p[4];          /**< URG pointer */
};

/**
 * @brief Parses a log line into a UfwLogEntry structure.
 *
 * This function takes a log line as input and parses it to fill the fields
 * of the provided UfwLogEntry structure. The function returns 1 on success
 * and 0 on failure.
 *
 * @param line The log line to parse.
 * @param entry The UfwLogEntry structure to populate.
 * @return 1 on successful parsing, 0 on failure.
 */
int parse_log_line(const char *line, struct UfwLogEntry *entry);

#endif // HERE_IS_JOHNNY_V_UFW_LOG_ENTRY_H
