#include "update.h"

int update_archive(int argc, char** argv) {
    char* archive_name = argv[2];

    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    posix_header file_data;
    int i = 3;
    int found = 0;
    off_t pos = 0;

    while (read(archive_fd, &file_data, BLOCK_SIZE) == BLOCK_SIZE) {
        if (file_data.name[0] == '\0') {
            break;
        }

        struct stat file_stat;
        char* file_name = argv[i];
        if (stat(file_name, &file_stat) < 0) {
            print_file_dne(file_name);
            return -1;
        }

        if (my_strcmp(file_data.name, file_name) == 0) {
            found = 1;
            unsigned int archive_mtime = strtol(file_data.time, NULL, 8);
            printf("Found file: %s, archive time: %u, file time: %ld\n", file_name, archive_mtime, file_stat.st_mtime);

            if (file_stat.st_mtime > archive_mtime) {
                printf("Updating: %s\n", file_name);
                lseek(archive_fd, pos, SEEK_SET);
                write_file_data(archive_fd, file_name);
                write_file_content(archive_fd, file_name);
            }
        }

        int file_size = strtol(file_data.size, NULL, 8);

        // skip the file content blocks
        int blocks_to_skip = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        lseek(archive_fd, blocks_to_skip * BLOCK_SIZE, SEEK_CUR);
        pos += BLOCK_SIZE + blocks_to_skip * BLOCK_SIZE;

        if (i < argc) {
            i++;
            found = 0;
        }
    }

    printf("found: %d\n", found);

    if (found == 0) {
        // lseek(archive_fd, 0, SEEK_END);
        append_archive(argc, argv);
    }

    close(archive_fd);
    return 0;
}