#include "extract.h"

int extract_archive(char* archive_name) {
    int archive_fd = open(archive_name, O_RDONLY);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }
    
    posix_header file_data;

    while (read(archive_fd, &file_data, BLOCK_SIZE) == BLOCK_SIZE) {
        // if (file_data.name[0] == '\0') {
        //     break; // end of archive
        // }

        unsigned int file_size = strtoll(file_data.size, NULL, 8);

        printf("Raw header data: ");
        for (int i = 0; i < BLOCK_SIZE; i++) {
            printf("%02x ", ((unsigned char*)&file_data)[i]);
        }
        printf("\n");

        printf("File name: %s\n", file_data.name);
        printf("File size: %s\n", file_data.size);
        printf("Checksum: %s\n", file_data.checksum_str);
        printf("Mode: %s\n", file_data.mode);
        printf("Typeflag: %c\n", file_data.typeflag[0]);
        printf("Typeflag (hex): %02x\n", (unsigned char)file_data.typeflag[0]);
        // printf("Typeflag (raw byte): %02x\n", ((unsigned char*)&file_data)[156]);

        if (file_data.typeflag[0] == DIRTYPE) { // directory
            printf("DIRECTORYY\n");
            mkdir(file_data.name, 0755);
        } else if (file_data.typeflag[0] == REGTYPE || file_data.typeflag[0] == AREGTYPE) { // regular file
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