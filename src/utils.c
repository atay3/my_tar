#include "utils.h"

void print_error(char* message) {
    write(STDERR_FILENO, message, my_strlen(message));
}

int my_strlen(const char* str_1) {
    int length = 0;

    while (*str_1 != '\0') {
        length++;
        str_1++;
    }

    return length;
}

char* my_strdup(const char* str_1) {
    int length = my_strlen(str_1);
    char* string = (char*) malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; i++) {
        string[i] = str_1[i];
    }

    string[length] = '\0';

    return string;
}

char* my_strncpy(char* str_1, const char* str_2, int length) {
    int i;
    for (i = 0; i < length && str_2[i] != '\0'; i++) {
        str_1[i] = str_2[i];
    }
    
    str_1[i] = '\0';

    return str_1;
}

int my_strcmp(const char* str_1, const char* str_2) {
    while (*str_1 != '\0' || *str_2 != '\0') {
        if (*str_1 < *str_2) {
            return -1;
        }
        else if (*str_1 > *str_2) {
            return 1;
        }
        str_1++;
        str_2++;
    }
    return 0;
}

void print_file_dne(const char* file_name) {
    char* msg_1 = "my_tar: ";
    char* msg_2 = ": Cannot stat: No such file or directory\n";
    write(STDERR_FILENO, msg_1, my_strlen(msg_1));
    write(STDERR_FILENO, file_name, my_strlen(file_name));
    write(STDERR_FILENO, msg_2, my_strlen(msg_2));
}

void print_tar_error(const char* archive_name) {
    char* msg_1 = "my_tar: Cannot open ";
    char newline = '\n';
    write(STDERR_FILENO, msg_1, my_strlen(msg_1));
    write(STDERR_FILENO, archive_name, my_strlen(archive_name));
    write(STDERR_FILENO, &newline, 1);
}

void print_end_block(int archive_fd) {
    char end_block[BLOCK_SIZE] = {0};
    
    // calculate current size of the archive
    off_t archive_size = lseek(archive_fd, 0, SEEK_END);

    while (archive_size < MIN_ARCHIVE_SIZE) {
        int diff = MIN_ARCHIVE_SIZE - archive_size;
        if (diff >= BLOCK_SIZE) {
            write(archive_fd, end_block, BLOCK_SIZE);
        } else if (diff < BLOCK_SIZE) {
            write(archive_fd, end_block, BLOCK_SIZE - diff);
        }
        archive_size += BLOCK_SIZE;
    }
}

void* my_memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}