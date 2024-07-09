#ifndef HERE_IS_JOHNNY_V_DATA_NOISE_H
#define HERE_IS_JOHNNY_V_DATA_NOISE_H

/**
 * @brief Set the interface name to use for sending network packets.
 *
 * @param name The interface name.
 */
void set_interface_name(char *name);

/**
 * @brief Set the source MAC address to use for sending network packets.
 *
 * @param mac The source MAC address.
 */
void set_src_mac(char *mac);

/**
 * @brief Get the source MAC address.
 *
 * @return The source MAC address.
 */
const char *get_src_mac();

/**
 * @brief Get the interface name.
 *
 * @return The interface name.
 */
const char *get_interface_name();


/**
 * @brief Craft and send network packets with data encoded in the header fields.
 *
 * The data is encoded into the packet fields:
 * - Ethernet source MAC address - 6 bytes (not used in this implementation)
 * - IP source address - 4 bytes
 * - TCP source port - 2 bytes
 * - TCP destination port - 2 bytes
 * - TCP sequence number - 4 bytes
 * - TCP acknowledgment number - 4 bytes
 *
 * @param data the data to send
 * @param dataLen the length of the data
 */
void send_noise(char *data, size_t dataLen);

#endif //HERE_IS_JOHNNY_V_DATA_NOISE_H
