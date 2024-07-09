#ifndef HERE_IS_JOHNNY_V_FILE_OPERATIONS_H
#define HERE_IS_JOHNNY_V_FILE_OPERATIONS_H

#include <stdio.h>

/**
 * @brief Opens a fileToObserve with the specified name and mode.
 *
 * This function attempts to open a fileToObserve with the given name and mode.
 * If the fileToObserve cannot be opened, it prints an error message.
 *
 * @param fileName The name of the fileToObserve to open.
 * @param mode The mode in which to open the fileToObserve.
 * @return A pointer to the opened fileToObserve, or NULL if the fileToObserve could not be opened.
 */
FILE *open_file(const char *fileName, const char *mode);

/**
 * @brief Closes an open fileToObserve.
 *
 * This function closes the given fileToObserve if it is not NULL.
 *
 * @param file The fileToObserve pointer to close.
 */
void close_file(FILE *file);

/**
 * @brief Gets the current position of the fileToObserve pointer.
 *
 * This function returns the current position of the fileToObserve pointer in the given fileToObserve.
 * If the position cannot be determined, it prints an error message.
 *
 * @param file The fileToObserve pointer for which to get the position.
 * @return The current position of the fileToObserve pointer, or -1 if an error occurred.
 */
off_t get_file_position(FILE *file);

/**
 * @brief Sets the position of the fileToObserve pointer.
 *
 * This function sets the position of the fileToObserve pointer in the given fileToObserve to the specified position.
 * If the position cannot be set, it prints an error message.
 *
 * @param file The fileToObserve pointer for which to set the position.
 * @param pos The position to set.
 * @param whence The reference point for the position (SEEK_SET, SEEK_CUR, or SEEK_END).
 * @return 0 on success, or -1 if an error occurred.
 */
int set_file_position(FILE *file, off_t pos, int whence);

/**
 * @brief Moves the fileToObserve pointer to the beginning of the last line.
 *
 * This function backtracks from the end of the fileToObserve to the beginning of the last line.
 * If an error occurs during the operation, it prints an error message.
 *
 * @param file The fileToObserve pointer for which to move the position.
 * @return 0 on success, or -1 if an error occurred.
 */
int backtrack_to_last_line(FILE *file);

/**
 * @brief Reads the entire content of a fileToObserve into a string.
 *
 * This function reads the entire content of the specified fileToObserve and returns it as a null-terminated string.
 * The caller is responsible for freeing the allocated memory. If an error occurs, it prints an error message.
 *
 * @param fileName The name of the fileToObserve to read.
 * @return A pointer to the fileToObserve content string, or NULL if an error occurred.
 */
char *get_file_content(const char *fileName);

/**
 * @brief Writes the given content to a fileToObserve.
 *
 * This function writes the specified content to the fileToObserve with the given name.
 * If the fileToObserve cannot be opened or the content cannot be written, it prints an error message.
 *
 * @param fileName The name of the fileToObserve to write to.
 * @param content The content to write to the fileToObserve.
 * @return 0 on success, or -1 if an error occurred.
 */
int write_to_file(const char *fileName, const char *content);

/**
 * @brief Appends the given content to a fileToObserve.
 *
 * This function appends the specified content to the fileToObserve with the given name.
 * If the fileToObserve cannot be opened or the content cannot be written, it prints an error message.
 *
 * @param fileName The name of the fileToObserve to append to.
 * @param content The content to append to the fileToObserve.
 * @return 0 on success, or -1 if an error occurred.
 */
int append_to_file(const char *fileName, const char *content);

#endif // HERE_IS_JOHNNY_V_FILE_OPERATIONS_H
