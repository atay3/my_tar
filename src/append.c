#include "append.h"
#include "archive.h"
#include "main.h"
#include "utils.h" // note: using for testing

void append_archive(int argc, char** argv) {
    if (argc < 4) {
        print_error(ERROR_MSG);
        return;
    }

    char* archive_name = argv[2]; // archive to write in
    char* buffer[BUFFER_SIZE];
    int archive_fd = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // open the archive that is being written into
    if (archive_fd == -1) {
        printf("Failed to open archive file");
        return;
    }

    for (int i = 3; i < argc; i++) {
        char* file = argv[i];
        int file_fd = open(file, O_RDONLY);
        if (file_fd == -1) {
            printf("Failed to open input file");
            continue;
        }

        ssize_t bytes_read;
        ssize_t total_bytes_written = 0;

        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            ssize_t bytes_written = write(archive_fd, buffer, bytes_read);
            if (bytes_written != bytes_read) {
                printf("Failed to write file content to archive");
                break;
            }
            total_bytes_written += bytes_written;
        }

        if (bytes_read == -1) {
            printf("Failed to read file content");
        }

        // calculate and add padding if necessary
        int padding = BUFFER_SIZE - (total_bytes_written % BUFFER_SIZE);
        if (padding != BUFFER_SIZE) {
            char pad[BUFFER_SIZE] = {0};
            ssize_t bytes_written = write(archive_fd, pad, padding);
            if (bytes_written != padding) {
                perror("Failed to write padding to archive");
            }
        }

        close(file_fd);
    }

    close(archive_fd);
}