#ifndef HERE_IS_JOHNNY_V_AUTH_H
#define HERE_IS_JOHNNY_V_AUTH_H

#include <stddef.h>

/**
 * Checks if the provided code matches the authentication code.
 *
 * @param code The code to be checked.
 * @return Non-zero if the code matches, 0 otherwise.
 */
size_t is_auth_code(const char *code);

/**
 * Authenticates a machine.
 *
 * @param macAddr The MAC address of the machine.
 * @param ipAddr The IP address of the machine.
 */
void authenticate(const char *macAddr, const char *ipAddr);

/**
 * De-authenticates the current machine.
 */
void deAuthenticate();

/**
 * Checks if a machine is currently authenticated.
 *
 * @return Non-zero if a machine is authenticated, 0 otherwise.
 */
size_t is_authenticated();

/**
 * Gets the MAC address of the authenticated machine.
 *
 * @return The MAC address of the authenticated machine.
 */
const char *get_auth_mac();

/**
 * Gets the IP address of the authenticated machine.
 *
 * @return The IP address of the authenticated machine.
 */
const char *get_auth_ip();

#endif //HERE_IS_JOHNNY_V_AUTH_H
