#include "append.h"
#include "main.h"
#include "utils.h"

int append_archive(int argc, char** argv) {
    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    off_t pos = lseek(archive_fd, -(2*BLOCK_SIZE), SEEK_END);
    posix_header file_data;
    char buffer[BLOCK_SIZE] = {'\0'};
 
    // Before reading file(s)
    // Extract info and fill posix_header with file info.

    //For each file, before reading, open tar file, append new header. Then Start reading:
        // Read BLOCK_SIZE from file, into a tmp buffer.
        // append tmp_buffer to tar file

    // int i = 3;
    // char* file_name = argv[i];
    // int file_fd = open(file_name, O_RDONLY);
    // if (file_fd < 0) {
    //     print_tar_error(file_name);
    //     return -1;
    // }
    // (void)argc;
    // while (read(archive_fd, &file_data, BLOCK_SIZE)) {
    //     if (!is_end_of_archive(&file_data)) {
    //             unsigned int size = strtoll(file_data.size, NULL, 8);

    //             if (size % BLOCK_SIZE != 0) {
    //                 size += BLOCK_SIZE - size % BLOCK_SIZE;
    //                 printf("Adjusted file size with padding: %u\n", size);
    //             }
                
    //             pos = lseek(archive_fd, size, SEEK_CUR);
    //             printf("Position after seeking past file data: %ld\n", pos);
    //             write_file_data(archive_fd, file_name);
    //             size_t n_bytes = 0;
    //             while ((n_bytes = read(file_fd, &buffer, BLOCK_SIZE)) > 0) {
    //                 ((char*)&file_data)[n_bytes] = '\0';
    //                 // write_file_data(archive_fd, file_name);
    //                 write_file_content(archive_fd, buffer);
    //             }
    //             // write_file_data(archive_fd, file_name);
    //             // write_file_content(archive_fd, file_name);
    //         }
    //         else {
    //             pos -= BLOCK_SIZE;
    //             lseek(archive_fd, -pos, SEEK_CUR);
    //         }
    // }

    for (int i = 3; i < argc; i++) {
        char* file_name = argv[i];
        int file_fd = open(file_name, O_RDONLY);
        if (file_fd < 0) {
            print_tar_error(file_name);
            return -1;
        }

        while (read(file_fd, &buffer, BLOCK_SIZE) > 0) {
            if (!is_end_of_archive(&file_data)) {
                // printf("Raw size field: %s\n", file_data.size);
                unsigned int size = strtoll(file_data.size, NULL, 8);
                // printf("Original file size: %u\n", size);

                if (size % BLOCK_SIZE != 0) {
                    size += BLOCK_SIZE - size % BLOCK_SIZE;
                    printf("Adjusted file size with padding: %u\n", size);
                }
                
                pos = lseek(archive_fd, size, SEEK_CUR);
                printf("Position after seeking past file data: %ld\n", pos);
            }
            pos -= BLOCK_SIZE;
            lseek(archive_fd, pos, SEEK_CUR);
        }
        write_file_data(archive_fd, file_name);
        write_file_content(archive_fd, file_name);
    }

    print_end_block(archive_fd);

    close(archive_fd);
    return 0;
}

bool is_end_of_archive(posix_header* file_data) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (i == 6) continue;
        if (((const char*)file_data)[i] != '\0') {
            printf("Non-null byte found: %d\n", ((const char*)file_data)[i]);
            return false;
        }
    }
    return true;
}

// bool is_valid_header(const posix_header* header) {
//     for (int i = 0; i < 100; ++i) {
//         if (header->name[i] != '\0') {
//             return true;
//         }
//     }
//     return false;
// }