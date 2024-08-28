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

    // off_t pos = lseek(archive_fd, -(2 * BLOCK_SIZE), SEEK_END);
    off_t pos = lseek(archive_fd, -(2 * BLOCK_SIZE), SEEK_SET);

    printf("Initial position: %ld\n", pos);

    posix_header file_data;
    int i = 3; // index of file_name

    while (read(archive_fd, &file_data, BLOCK_SIZE) && i < argc) {
        printf("Read file header at position: %ld\n", pos);
        if (!is_end_of_archive(&file_data)) {
            char* file_name = argv[i];
            size_t size = strtoll(file_data.size, NULL, 8);
            printf("Raw size field: %s\n", file_data.size);
            printf("Original file size: %zu\n", size);

            if (size % BLOCK_SIZE != 0) {
                size += BLOCK_SIZE - size % BLOCK_SIZE;
                printf("Adjusted file size with padding: %zu\n", size);
            }
            
            pos = lseek(archive_fd, size, SEEK_CUR);
            printf("Position after seeking past file data: %ld\n", pos);
            write_file_data(archive_fd, file_name);
            write_file_content(archive_fd, file_name);
            i++;
        } 
        // else {
            pos -= BLOCK_SIZE;
            lseek(archive_fd, -pos, SEEK_CUR);
            printf("Position after moving: %ld\n", pos);
        // }
        
    }

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
    printf("an end block\n");
    return true;
}