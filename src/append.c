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

    // off_t pos = lseek(archive_fd, -(2*BLOCK_SIZE), SEEK_END);
    posix_header file_data;
 
    while (read(archive_fd, &file_data, BLOCK_SIZE)) {
        // printf("Read file header at position: %ld\n", pos);
        // printf("Raw header data: ");
        // for (int i = 0; i < BLOCK_SIZE; i++) {
        //     printf("%02x ", ((unsigned char*)&file_data)[i]);
        // }
        // printf("\n");
        // if (is_end_of_archive(&file_data)) {
        //     break;
        // }

        if (!is_end_of_archive(&file_data)) {
            printf("Raw size field: %s\n", file_data.size);
            unsigned int size = strtoll(file_data.size, NULL, 8);
            printf("Original file size: %u\n", size);

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

    // off_t current_pos = lseek(archive_fd, -(BLOCK_SIZE), SEEK_END);
    // while (read(archive_fd, &file_data, BLOCK_SIZE)) {
    //     // lseek(archive_fd, current_pos - BLOCK_SIZE, SEEK_SET);

    //     if (is_end_of_archive(&file_data)) {
    //         printf("End of archive found.\n");
    //         break;
    //     }

    //     printf("Raw size field: %s\n", file_data.size);
    //     unsigned int size = strtoll(file_data.size, NULL, 8);
    //     printf("Original file size: %u\n", size);

    //     if (size % BLOCK_SIZE != 0) {
    //         size += BLOCK_SIZE - size % BLOCK_SIZE;
    //         printf("Adjusted file size with padding: %u\n", size);
    //     }

    //     current_pos -= (size + BLOCK_SIZE);
    //     lseek(archive_fd, pos, SEEK_CUR);
    // }

    // for (int i = 3; i < argc; i++) {
    //     char* file_name = argv[i];
    //     write_file_data(archive_fd, file_name);
    //     write_file_content(archive_fd, file_name);
    // }

    print_end_block(archive_fd);

    close(archive_fd);
    return 0;
}

// bool is_end_of_archive(posix_header* file_data) {
//     for (int i = 0; i < BLOCK_SIZE; i++) {
//         if (i == 6) continue;
//         if (((const char*)file_data)[i] != '\0') {
//             printf("Non-null byte found: %d\n", ((const char*)file_data)[i]);
//             return false;
//         }
//     }
//     return true;
// }

// bool is_valid_header(const posix_header* header) {
//     for (int i = 0; i < 100; ++i) {
//         if (header->name[i] != '\0') {
//             return true;
//         }
//     }
//     return false;
// }