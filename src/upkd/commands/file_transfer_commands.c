#include <time.h>
#include <malloc.h>
#include <string.h>
#include "../../../include/utils/file_operations.h"
#include "../../../include/data_noise/data_noise.h"
#include "../../../include/crypto/crypto.h"

void process_received_file(const char* data) {

    // Generate a file name based on time
    char file_name[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(file_name, "%d-%d-%d-%d-%d-%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    // Parse the extracted fileToObserve data and write it to a fileToObserve
    write_to_file(file_name, data);

    printf("Wrote data to the file: %s\n", file_name);
}

void send_file(const char *file_name) {
    printf("\nSending file: %s\n", file_name);

    // Read the fileToObserve data
    char *file_data = get_file_content(file_name);
    if (file_data == NULL) {
        return;
    }

    // Wrap fileToObserve data in <FILE> tags
    size_t wrapped_file_data_len = strlen(file_data) + 14;
    char *wrapped_file_data = (char *)malloc(wrapped_file_data_len);
    if (wrapped_file_data == NULL) {
        return;
    }
    snprintf(wrapped_file_data, wrapped_file_data_len, "<FILE>%s</FILE>", file_data);

    // Send the fileToObserve data as noise
    send_noise(wrapped_file_data, wrapped_file_data_len);

    // Free the allocated memory
    free(file_data);
}