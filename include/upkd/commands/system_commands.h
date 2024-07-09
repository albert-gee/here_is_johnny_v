#ifndef HERE_IS_JOHNNY_V_SYSTEM_COMMANDS_H
#define HERE_IS_JOHNNY_V_SYSTEM_COMMANDS_H

/**
 * @brief Executes a shell script.
 *
 * This function executes a shell script.
 *
 * @param script The shell script to execute.
 * @return The output of the shell script.
 */
char *execute_shell_script(const char* script);

/**
 * @brief Uninstalls the program.
 */
void uninstall();

#endif //HERE_IS_JOHNNY_V_SYSTEM_COMMANDS_H
