#include <string.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include "../../include/inotify_observer/inotify_subject.h"
#include "../../include/upkd/ufw_log_entry.h"
#include "../../include/utils/file_operations.h"
#include "../../include/upkd/db.h"
#include "../../include/upkd/upkd.h"
#include "../../include/upkd/data_processor.h"

static off_t file_pos = 0; // Last read position in the fileToObserve
static FILE *fileToObserve = NULL;

// Local function prototypes

/**
 * @brief Processes log entries for connection knocks when a monitored file is modified.
 *
 * This function is triggered by an inotify event indicating that the monitored file has been modified.
 * It reads new log entries from the file, parses them, validates connection knocks, and processes
 * them by inserting decoded port information into a hash table.
 *
 * @param event Pointer to the inotify event structure.
 */
static void observe_connection_knocks(struct inotify_event *event);

// Public functions

void observe_ufw_log() {
    // Open the UFW log file for reading
    fileToObserve = open_file(LOG_PATH, "r");
    if (fileToObserve == NULL) {
        return;
    }

    // Backtrack to the last line of the log file
    if (backtrack_to_last_line(fileToObserve) != 0) {
        close_file(fileToObserve);
        return;
    }

    // Get the current file position
    file_pos = get_file_position(fileToObserve);
    if (file_pos == -1) {
        close_file(fileToObserve);
        return;
    }

    // Initialize the inotify observer structure with the event handler function
    InotifyObserver ufwLogConnectionObserver = {.handleEvent = observe_connection_knocks};

    // Define the inotify event masks to monitor (in this case, only modifications)
    uint32_t masks[] = {IN_MODIFY};
    int numMasks = sizeof(masks) / sizeof(masks[0]);

    // Start observing the log file for modifications
    observe(LOG_PATH, masks, numMasks, (InotifyObserver *[]) {&ufwLogConnectionObserver}, 1);

    // Close the fileToObserve
    close_file(fileToObserve);
}

// Static functions

static void observe_connection_knocks(struct inotify_event *event) {
    // File was modified. Read new log entries.

    // Set fileToObserve position to the last read position
    if (set_file_position(fileToObserve, file_pos, SEEK_SET) != 0) {
        return;
    }

    // Read new log entries line by line
    char buffer[BUF_SIZE];
    while (fgets(buffer, sizeof(buffer), fileToObserve) != NULL) {

        // Parse the log entry
        struct UfwLogEntry entry;
        if (parse_log_line(buffer, &entry)) {

            // Create a source MAC address because
            // entry.mac_address contains both source and destination MAC
            char mac_src[18];  // Updated size to fit half of the larger MAC address field
            strncpy(mac_src, entry.mac_address + 18, 17);
            mac_src[17] = '\0';

            // Allocate memory for the decoded string of source and destination ports
            char *decodedString = (char *) malloc(6 * sizeof(char)); // 6 bytes to store at least 3 characters
            if (decodedString == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                return; // Handle allocation failure
            }

            // Encode source port (spt) and destination port (dpt) into characters
            snprintf(decodedString, 6, "%c%c", (char) entry.spt, (char) entry.dpt);

            // Insert the decoded string into the hash table with source MAC address as the key
            // The received data is a concatenated string of all decoded strings for the same MAC address
            const char *receivedData = hash_table_insert(mac_src, decodedString, MAX_DATA_LENGTH);
            if (receivedData != NULL) {

                // Process the concatenated value stored in the hash table
                process_received_data(receivedData, mac_src, entry.src_ip);
            }

        } else {
            // Log line is not of interest, continue to next line
        }
    }

    // Update the file position to the last read position
    file_pos = get_file_position(fileToObserve); // Update last read position
}
