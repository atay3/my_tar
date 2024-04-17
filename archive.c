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
    write_uid(archive_fd, file_stat.st_uid, metadata.uid);
    write_gid(archive_fd, file_stat.st_gid, metadata.gid);
    write_size(archive_fd, file_stat.st_size, metadata.size);

    // metadata.time = file_stat.st_mtime;
    write_time(archive_fd, file_stat.st_mtim, metadata.time);


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
    str[start_index] = '0' + ((mode >> 6) & 0b111);
    str[start_index + 1] = '0' + ((mode >> 3) & 0b111); //0x7 similar to 0b111
    str[start_index + 2] = '0' + (mode & 0b111);

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
    // int i = 1;
    int end_index = 7;
    octal_str[end_index--] = '\0'; //null terminate the string
    // printf("end index: %d\n", end_index);
    // printf("uid: %d, uid octal: %o\n", uid, uid);

    while (uid != 0) {
        uint8_t octal_digit = uid & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        // printf("octal digit: %c\n", octal_str[end_index - i]);
        uid >>= 3;
        // printf("i in current iteration (index is %d): %d\n", end_index - i, i);
        // i++;
    }

    // printf("i before padding: %d\n", i);
    while (end_index >= 0) {
        // printf("padding index %d\n", end_index - i);
        octal_str[end_index--] = '0';
        // i++;
        // printf("octal digit: %c\n", octal_str[end_index - i]);
    }

    // printf("uid in octal: %s\n", octal_str);
}

void write_uid(int archive_fd, uid_t uid, char* octal_str) {
    uid_to_octal(uid, octal_str);
    // write(archive_fd, octal_str, my_strlen(octal_str));

    if (write(archive_fd, octal_str, my_strlen(octal_str)) == -1) {
        printf("error writing uid\n");
        return;
    }
}

void gid_to_octal(gid_t gid, char* octal_str) {
    // int i = 1;
    int end_index = 7;
    octal_str[end_index--] = '\0';

    while (gid != 0) {
        uint8_t octal_digit = gid & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        gid >>= 3;
        // i++;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
        // i++;
    }
}

void write_gid(int archive_fd, gid_t gid, char* octal_str) {
    gid_to_octal(gid, octal_str);
    write(archive_fd, octal_str, my_strlen(octal_str));
}

void size_to_octal(size_t size, char* octal_str) {
    int end_index = 11;
    octal_str[end_index--] = '\0';

    while (size != 0) {
        uint8_t octal_digit = size & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        size >>= 3;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
    }
}

void write_size(int archive_fd, size_t size, char* octal_str) {
    size_to_octal(size, octal_str);
    write(archive_fd, octal_str, my_strlen(octal_str));
}

void write_time(int archive_fd, time_t time, char* time_str) {
    // char* time_str = ctime(&time);
    // if (time_str != NULL) {
    //     write(archive_fd, time_str, my_strlen(time_str));
    // }
    // else {
    //     printf("time str is null\n");
    // }

    //convert time to microsceconds
    long long modification_time = (long long)(time.tv_sec) * MICROSECONDS_PER_SECOND + (long long)(time.tv_nsec) / 1000;

    //convert time to string
    int length = 0;
    int max_length = 12;
    while (modification_time > 0 && length < max_length - 1) {
        time_str[length++] = '0' + modification_time % 10;
    }
    time_str[max_length - 1] = '\0';

    for (int i = 0; i < length / 2; i++) {
        char temp = time_str[i];
        time_str[i] = time_str[length - i - 1];
        time_str[length - i - 1] = temp;
    }

    write(archive_fd, time_str, length);
}