#include "extract.h"

int extract_archive(char* archive_name) {
    int archive_fd = open(archive_name, O_RDONLY);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }
    unsigned char buffer[BLOCK_SIZE];
    posix_header file_data;
    while (read(archive_fd, buffer, BLOCK_SIZE) > 0) {
        my_memcpy(&file_data, buffer, BLOCK_SIZE);
        unsigned char typeflag = buffer[156];

        if (*file_data.name == '\0') break;

        // if (typeflag == DIRTYPE) {
        //     printf("Directory detected\n");
        // } else {
        //     printf("Not a directory. Typeflag: %c (hex: %02x)\n", typeflag, typeflag);
        // }
        unsigned int file_size = strtoll(file_data.size, NULL, 8);

        if (typeflag == DIRTYPE) { // directory
            printf("DIRECTORY\n");
            printf("Creating directory: %s\n", file_data.name);

            if (mkdir(file_data.name, 0755) < 0) {
                printf("Error creating directory\n");
                return -1;
            }
        } else if (typeflag == REGTYPE || typeflag == AREGTYPE) { // regular file
            printf("REG FILE\n");
            int out_fd = open(file_data.name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) { // unable to create file
                close(archive_fd);
                return -1;
            }

            // read file contents
            char buffer[BLOCK_SIZE];
            int bytes_to_read = file_size;
            while (bytes_to_read > 0) {
                int bytes = read(archive_fd, buffer, (bytes_to_read < BLOCK_SIZE) ? bytes_to_read : BLOCK_SIZE);
                if (bytes < 0) {
                    close(out_fd);
                    close(archive_fd);
                    return -1;
                }
                write(out_fd, buffer, bytes);
                bytes_to_read -= bytes;
            }

            close(out_fd);

            if (file_size % BLOCK_SIZE != 0) {
                lseek(archive_fd, BLOCK_SIZE - (file_size % BLOCK_SIZE), SEEK_CUR);
            }
        }
    }

    close(archive_fd);

    return 0;
}