#include <stddef.h>
#include <string.h>
#include "../../include/upkd/auth.h"

// Authentication code
static char *authCode = "here_is_johnny";

// Authentication status and machine details
static size_t isAuthenticated = 0;      // 0: Not authenticated, 1: Authenticated
static char *mac = NULL;                // MAC address of the authenticated machine
static char *ip = NULL;                 // IP address of the authenticated machine

size_t is_auth_code(const char *code) {
    return strstr(code, authCode) != NULL;
}

void authenticate(const char *macAddr, const char *ipAddr) {
    isAuthenticated = 1;
    mac = strdup(macAddr);
    ip = strdup(ipAddr);
}

void deAuthenticate() {
    isAuthenticated = 0;
    mac = NULL;
    ip = NULL;
}

size_t is_authenticated() {
    return isAuthenticated;
}

const char *get_auth_mac() {
    return mac;
}

const char *get_auth_ip() {
    return ip;
}