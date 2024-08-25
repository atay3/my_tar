#include "append.h"
#include "main.h"
#include "utils.h"

int append_archive(int argc, char** argv) {
    if (argc < 4) {
        print_error(ERROR_MSG);
        return -1;
    }

    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    off_t pos = lseek(archive_fd, -(BLOCK_SIZE), SEEK_END);
    posix_header file_data;
    int i = 3; // index of file_name
    // char* file_name = argv[3];

    while (read(archive_fd, &file_data, BLOCK_SIZE) == BLOCK_SIZE) {

        if (!is_end_of_archive(&file_data) && i < argc) {
            char* file_name = argv[i];
            size_t size = strtoll(file_data.size, NULL, 8);
            if (size % BLOCK_SIZE){
                size += BLOCK_SIZE - size % BLOCK_SIZE;
            }
            pos = lseek(archive_fd, size, SEEK_CUR);
            write_file_data(archive_fd, file_name);
            write_file_content(archive_fd, file_name);
            i++;
        }
        pos -= BLOCK_SIZE;
        lseek(archive_fd, -pos, SEEK_CUR);
    }

    // for (int i = 3; i < argc; i++) {
    //     char* file_name = argv[i];
    //     int file_fd = open(file_name, O_RDONLY);
    //     if (file_fd == -1) {
    //         printf("Failed to open input file");
    //         continue;
    //     }

    //     write_file_data(archive_fd, file_name);

    //     close(file_fd);
    // }

    close(archive_fd);
    return 0;
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