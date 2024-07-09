#include <string.h>
#include <stdlib.h>

#include "../../include/upkd/ufw_log_entry.h"

int parse_log_line(const char *line, struct UfwLogEntry *entry) {

    char *token;
    char *rest = (char *)line;

    // Skip timestamp
    token = strtok_r(rest, " ", &rest);
    if (token == NULL) {
        return 0;
    }

    // Skip hostname
    token = strtok_r(rest, " ", &rest);
    if (token == NULL) {
        return 0;
    }

    // Skip program name
    token = strtok_r(rest, " ", &rest);
    if (token == NULL) {
        return 0;
    }

    // Skip [UFW
    token = strtok_r(rest, " ", &rest);
    if (token == NULL) {
        return 0;
    }

    // Entry type
    char *entry_type = strtok_r(rest, " ", &rest);
    if (entry_type == NULL) {
        return 0;
    }

    // Check if the entry type is "BLOCK]"
    if (strcmp(entry_type, "BLOCK]") != 0) {
        return 0;
    }

    // Parse remaining tokens
    while ((token = strtok_r(NULL, " ", &rest)) != NULL) {
        // Determine the type of token and update entry accordingly
        if (strncmp(token, "IN=", 3) == 0) {
            strcpy(entry->in_interface, token + 3);
        } else if (strncmp(token, "OUT=", 4) == 0) {
            strcpy(entry->out_interface, token + 4);
        } else if (strncmp(token, "MAC=", 4) == 0) {
            strcpy(entry->mac_address, token + 4);
        } else if (strncmp(token, "SRC=", 4) == 0) {
            strcpy(entry->src_ip, token + 4);
        } else if (strncmp(token, "DST=", 4) == 0) {
            strcpy(entry->dst_ip, token + 4);
        } else if (strncmp(token, "LEN=", 4) == 0) {
            entry->length = atoi(token + 4);
        } else if (strncmp(token, "TOS=", 4) == 0) {
            strcpy(entry->tos, token + 4);
        } else if (strncmp(token, "PREC=", 5) == 0) {
            strcpy(entry->prec, token + 5);
        } else if (strncmp(token, "TTL=", 4) == 0) {
            entry->ttl = atoi(token + 4);
        } else if (strncmp(token, "ID=", 3) == 0) {
            entry->id = atoi(token + 3);
        } else if (strncmp(token, "PROTO=", 6) == 0) {
            strcpy(entry->protocol, token + 6);
        } else if (strncmp(token, "SPT=", 4) == 0) {
            entry->spt = atoi(token + 4);
        } else if (strncmp(token, "DPT=", 4) == 0) {
            entry->dpt = atoi(token + 4);
        } else if (strncmp(token, "WINDOW=", 7) == 0) {
            entry->window = atoi(token + 7);
        } else if (strncmp(token, "RES=", 4) == 0) {
            strcpy(entry->res, token + 4);
        } else if (strncmp(token, "SYN", 3) == 0) {
            strcpy(entry->syn, token);
        } else if (strncmp(token, "URGP=", 5) == 0) {
            strcpy(entry->urg_p, token + 5);
        }
    }

    return 1;
}