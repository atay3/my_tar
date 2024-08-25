#include "list.h"
#include "archive.h"
#include "main.h"
#include "utils.h"

int list_archive(char* archive_name) {
    // if (argc < 3) {
    //     print_error(ERROR_MSG);
    //     return -1;
    // }

    int archive_fd = open(archive_name, O_RDONLY);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    posix_header file_data;

    while (read(archive_fd, &file_data, BLOCK_SIZE) == BLOCK_SIZE) {

        if (file_data.name[0] == '\0') {
            break;
        }
        
        print_files(file_data.name);

        int file_size = strtol(file_data.size, NULL, 8);

        // skip the file content blocks
        int blocks_to_skip = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        lseek(archive_fd, blocks_to_skip * BLOCK_SIZE, SEEK_CUR);
    }

    close(archive_fd);
    return 0;
}

void print_files(char* file_name) {
    char newline = '\n';
    write(STDOUT_FILENO, file_name, my_strlen(file_name));
    write(STDOUT_FILENO, &newline, 1);
}