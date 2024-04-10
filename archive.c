#include "my_tar.h"

void create_archive(int argc, char** argv) {
    char* archive_name = argv[2];

    //archive file descriptor
    int archive_fd = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    for (int i = 3; i < argc; i++) {
        write_metadata(archive_fd, argv[i]);
        write_file_content(archive_fd, argv[i]);
    }

    close(archive_fd);
}

void write_metadata(int archive_fd, const char* file_name) {
    struct stat file_stat;

    if (lstat(file_name, &file_stat) == -1) {
        printf("Error retrieving file stats\n"); //remove later
        return;
    }

    struct FileMetadata metadata;

    my_strncpy(metadata.name, file_name, my_strlen(file_name));
    if (write(archive_fd, &metadata.name, my_strlen(metadata.name)) != my_strlen(metadata.name)) {
        perror("write file name");
        return;
    }

    write_mode(archive_fd, file_stat.st_mode, metadata.mode);

    // metadata.uid = file_stat.st_uid;
    write_uid(archive_fd, file_stat.st_uid, metadata.uid);

    // metadata.gid = file_stat.st_gid;
    metadata.size = file_stat.st_size;
    metadata.time = file_stat.st_mtim;

    //PRINTING FILE TPYE, FLAGS, AND PERMISSIONS IN OCTAL
    // mode_t file_type = file_stat.st_mode & S_IFMT;
    // printf("file type: %o\n", file_type);
    // mode_t flags = file_stat.st_mode & (S_ISUID | S_ISGID | S_ISVTX);
    // printf("flags: %o\n", flags);
    // printf("permissions: %o\n", file_stat.st_mode & 0777);

    // if (write(archive_fd, &metadata.uid, sizeof(metadata.uid)) != sizeof(metadata.uid)) {
    //     perror("write uid");
    //     return;
    // }

    // if (write(archive_fd, &metadata.gid, sizeof(metadata.gid)) != sizeof(metadata.gid)) {
    //     perror("write gid");
    //     return;
    // }

    // if (write(archive_fd, &metadata.size, sizeof(metadata.size)) != sizeof(metadata.size)) {
    //     perror("write size");
    //     return;
    // }

    // if (write(archive_fd, &metadata.time, sizeof(metadata.time)) != sizeof(metadata.time)) {
    //     perror("write time");
    //     return;
    // }
}

void write_file_content(int archive_fd, const char* file_name) {
    int file_fd = open(file_name, O_RDONLY);

    if (file_fd == -1) {
        printf("Error opening file\n"); //remove later
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(archive_fd, buffer, bytes_read) != bytes_read) {
            printf("Error writing file contents\n"); //remove later
            return;
        }
    }

    close(file_fd);
}

void mode_to_octal(mode_t mode, char* str, int start_index) {
    //convert the bits to octal representation manually
    str[start_index] = '0' + ((mode >> 6) & 0x7);
    str[start_index + 1] = '0' + ((mode >> 3) & 0x7);
    str[start_index + 2] = '0' + (mode & 0x7);

    //null terminates the end of the string
    if (start_index + 3 == 7) {
        str[start_index + 3] = '\0';
    }
}

void write_mode(int archive_fd, mode_t mode, char* octal_str) {
    // mode_t file_type = mode & S_IFMT; //extract file type bits
    // mode_t flags = mode & (S_ISUID | S_ISGID | S_ISVTX); //extract flags
    mode_t permissions = mode & (S_IRWXU | S_IRWXG | S_IRWXO); //extract file permissions

    int padding = 5;
    for (int i = 0; i < padding - 1; i++) {
        octal_str[i] = '0';
    }
    mode_to_octal(permissions, octal_str, padding - 1);

    // char file_type_octal = '0' + ((file_type >> 12) & 0x7);
    // octal_str[0] = file_type_octal;
    // mode_to_octal(flags, octal_str, 1);
    // mode_to_octal(permissions, octal_str, 4);

    write(archive_fd, octal_str, my_strlen(octal_str));
}

void uid_to_octal(uid_t uid, char* octal_str) {
    int i = 0;
    int end_index = my_strlen(octal_str) - 1;

    printf("uid: %d, uid octal: %o", uid, uid);

    while (uid > 0) {
        octal_str[end_index - i] = '0' + (uid % 8);
        uid /= 8;
        i++;
    }

    while (i < end_index) {
        octal_str[end_index - i] = '0';
        i++;
    }

    octal_str[end_index] = '\0';
}

void write_uid(int archive_fd, uid_t uid, char* octal_str) {
    uid_to_octal(uid, octal_str);
    write(archive_fd, octal_str, my_strlen(octal_str));
}