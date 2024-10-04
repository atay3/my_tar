#include "../include/append.h"
#include "../include/main.h"
#include "../include/utils.h"

// New implementation. Instead of lseek from the end, loop through the whole tar file
void go_to_end_tar_file(int archive_fd);

int append_archive(int argc, char** argv) {
    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }
    
    //go to the end of tar
    go_to_end_tar_file(archive_fd);
    posix_header* file_data = NULL;
    for (int i = 3; i < argc; i++) {
        char* file_name = argv[i];
        int file_fd = open(file_name, O_RDONLY);
        if (file_fd < 0) {
            print_tar_error(file_name);
            return -1;
        }

        write_file_data(archive_fd, file_name);
        write_file_content(archive_fd, file_name);
        free(file_data);
    }

    // append end of tar file
    print_end_block(archive_fd);
    close(archive_fd);
    return 0;
}

unsigned long octal_to_decimal(const char *octal_str) {
    return strtoul(octal_str, NULL, 8);
}

void go_to_end_tar_file(int archive_fd) {
    char buffer[BLOCK_SIZE];
    
    int counter = 0;
    unsigned long size = 1;
    while (size) {
        read(archive_fd, buffer, BLOCK_SIZE);
        size = octal_to_decimal(&buffer[124]);
        if (!size) {
            lseek(archive_fd, -BLOCK_SIZE, SEEK_CUR);
            break;
        }
        lseek(archive_fd, size + (BLOCK_SIZE - size % 512) , SEEK_CUR); 
        counter++;
    }
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

bool is_valid_header(const posix_header* header) {
    for (int i = 0; i < 100; ++i) {
        if (header->name[i] != '\0') {
            return true;
        }
    }
    return false;
}
