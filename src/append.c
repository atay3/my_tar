#include "append.h"
// #include "archive.h"
#include "main.h"
#include "utils.h"

void append_archive(int argc, char** argv) {
    if (argc < 4) {
        print_error(ERROR_MSG);
        return;
    }

    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd == -1) {
        printf("Failed to open archive file");
        return;
    }

    off_t pos = lseek(archive_fd, -(BLOCK_SIZE), SEEK_END);
    // char buffer[BLOCK_SIZE];
    posix_header file_data;

    while (read(archive_fd, &file_data, BLOCK_SIZE) == BLOCK_SIZE) {

        if (is_end_of_archive(&file_data)) {
            printf("EOF marker\n");
            pos -= BLOCK_SIZE;
            lseek(archive_fd, -pos, SEEK_CUR);
        }

        // size_t file_size = strtoll(file_data.size, NULL, 8);
        // size_t padding = (BLOCK_SIZE - (file_size % BLOCK_SIZE)) % BLOCK_SIZE;
        // lseek(archive_fd, file_size + padding, SEEK_CUR);
        
    }

    for (int i = 3; i < argc; i++) {
        char* file_name = argv[i];
        int file_fd = open(file_name, O_RDONLY);
        if (file_fd == -1) {
            printf("Failed to open input file");
            continue;
        }

        write_file_data(archive_fd, file_name);

        close(file_fd);
    }

    close(archive_fd);
}

// bool is_end_of_archive(int archive_fd, char* buffer) {
//     // Read the last block
//     ssize_t bytes_read = read(archive_fd, buffer, BLOCK_SIZE);
//     if (bytes_read != BLOCK_SIZE) {
//         perror("read failed");
//         return false;
//     }

//     // Check if the last block is entirely null bytes
//     for (int i = 0; i < BLOCK_SIZE; i++) {
//         if (buffer[i] != '\0') {
//             return false;
//         }
//     }

//     return true;
// }

bool is_end_of_archive(posix_header* file_data) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (((const char*)file_data)[i] != '\0') {
            return false;
        }
    }
    return true;
}