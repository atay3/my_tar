#include "append.h"
#include "main.h"
#include "utils.h"

int append_archive(int argc, char** argv) {
    // if (argc < 4) {
    //     print_error(ERROR_USG);
    //     return -1;
    // }

    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    off_t pos = lseek(archive_fd, -(BLOCK_SIZE), SEEK_END);
    posix_header file_data;
    int i = 3; // index of file_name

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

    close(archive_fd);
    return 0;
}

bool is_end_of_archive(posix_header* file_data) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (((const char*)file_data)[i] != '\0') {
            return false;
        }
    }
    return true;
}