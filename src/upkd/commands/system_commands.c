#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/commands/system_commands.h"

#define BUFFER_SIZE 128  // Define the buffer size for reading output from the shell script

char *execute_shell_script(const char* script) {
    // Open a pipe to the shell command for reading
    FILE* pipe = popen(script, "r");
    if (!pipe) {
        perror("popen failed");
        return NULL;
    }

    // Allocate initial memory for storing the output of the script
    size_t output_size = BUFFER_SIZE * 10;
    char *output = malloc(output_size);
    if (!output) {
        perror("malloc failed");
        pclose(pipe);
        return NULL;
    }
    output[0] = '\0'; // Initialize output as an empty string

    // Temporary buffer for reading the output from the pipe
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {

        // Check if there is enough space in the output buffer
        size_t new_length = strlen(output) + strlen(buffer) + 1;
        if (new_length > output_size) {

            // If not enough space, double the output buffer size
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (!new_output) {
                perror("realloc failed");
                free(output);
                pclose(pipe);
                return NULL;
            }
            output = new_output;
        }

        // Append the new data to the output string
        strcat(output, buffer);
    }

    // Close the pipe and check if there was an error during the process
    int status = pclose(pipe);
    if (status == -1) {
        perror("pclose failed");
    } else {
        printf("\nShell script exited with status %d\n", WEXITSTATUS(status));
        fflush(stdout);
    }

    return output;
}

void uninstall() {

    // Get the path of the executable
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        perror("Failed to get the executable path");
    }
    path[count] = '\0'; // Null-terminate the path
    int status = remove(path);
    if (status == 0) {
        printf("Program deleted successfully.\n");
    } else {
        perror("Unable to delete the program");
    }
}