#include "include/upkd/upkd.h"
#include "include/crypto/crypto.h"
#include "include/data_noise/data_noise.h"

int main(int argc, char *argv[]) {

    // Set the key and initialization vector for AES encryption
    set_key((unsigned char[]) {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    });
    set_iv((unsigned char[]) {
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    });

    // Print the encryption algorithm and the key and initialization vector
    printf("\nEncryption: AES-128-CBC\n");
    print_hex("Key", get_key(), AES_KEY_SIZE);
    print_hex("IV", get_iv(), AES_BLOCK_SIZE);
    fflush(stdout);

    // Set the source MAC address and the interface name
    set_src_mac("00:0c:29:4d:3e:7c");
    set_interface_name("eno2");

    // Print the source MAC address and the interface name
    printf("\nSource MAC: %s", get_src_mac());
    printf("\nInterface: %s\n", get_interface_name());
    fflush(stdout);

    // Start the UFW log observer
    observe_ufw_log();

    return 0;
}