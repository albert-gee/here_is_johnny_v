#include "../../include/utils/file_operations.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

FILE *open_file(const char *fileName, const char *mode) {
    FILE *file = fopen(fileName, mode);
    if (file == NULL) {
        perror("Failed to open fileToObserve");
    }
    return file;
}

void close_file(FILE *file) {
    if (file != NULL) {
        fclose(file);
    }
}

off_t get_file_position(FILE *file) {
    off_t pos = ftell(file);
    if (pos == -1) {
        perror("Error telling fileToObserve position");
    }
    return pos;
}

int set_file_position(FILE *file, off_t pos, int whence) {
    if (fseek(file, pos, whence) != 0) {
        perror("Error seeking fileToObserve");
        return -1;
    }
    return 0;
}

int backtrack_to_last_line(FILE *file) {
    if (set_file_position(file, 0, SEEK_END) != 0) {
        return -1;
    }

    while (get_file_position(file) > 0) {
        if (set_file_position(file, -1, SEEK_CUR) != 0) {
            return -1;
        }
        if (fgetc(file) == '\n') {
            break;
        }
        if (set_file_position(file, -1, SEEK_CUR) != 0) {
            return -1;
        }
    }
    return 0;
}

char *get_file_content(const char *fileName) {
    FILE *file = open_file(fileName, "r");
    if (file == NULL) {
        return NULL;
    }

    if (set_file_position(file, 0, SEEK_END) != 0) {
        close_file(file);
        return NULL;
    }
    long fileSize = get_file_position(file);
    if (fileSize == -1) {
        close_file(file);
        return NULL;
    }
    rewind(file);

    char *fileContent = malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Failed to allocate memory");
        close_file(file);
        return NULL;
    }

    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead < fileSize) {
        perror("Failed to read fileToObserve");
        free(fileContent);
        close_file(file);
        return NULL;
    }

    fileContent[bytesRead] = '\0';

    close_file(file);
    return fileContent;
}

int write_to_file(const char *fileName, const char *content) {
    FILE *file = open_file(fileName, "w");
    if (file == NULL) {
        return -1;
    }

    size_t contentLength = strlen(content);
    size_t bytesWritten = fwrite(content, 1, contentLength, file);
    if (bytesWritten < contentLength) {
        perror("Failed to write to fileToObserve");
        close_file(file);
        return -1;
    }

    close_file(file);
    return 0;
}

int append_to_file(const char *fileName, const char *content) {

    FILE *file = open_file(fileName, "a");
    if (file == NULL) {
        perror("Failed to open fileToObserve");
        return -1;
    }

    size_t contentLength = strlen(content);
    size_t bytesWritten = fwrite(content, 1, contentLength, file);
    if (bytesWritten < contentLength) {
        perror("Failed to write to fileToObserve");
        close_file(file);
        return -1;
    }

    close_file(file);
    return 0;
}