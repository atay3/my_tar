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

    FileMetadata metadata;

    my_strncpy(metadata.name, file_name, my_strlen(file_name));
    if (write(archive_fd, &metadata.name, my_strlen(metadata.name)) != my_strlen(metadata.name)) {
        perror("write file name");
        return;
    }

    write_mode(archive_fd, file_stat.st_mode, metadata.mode);
    write_uid(archive_fd, file_stat.st_uid, metadata.uid);
    write_gid(archive_fd, file_stat.st_gid, metadata.gid);
    write_size(archive_fd, file_stat.st_size, metadata.size);
    // printf("Modification time: %lld\n", (long long)file_stat.st_mtim.tv_sec);

    write_time(archive_fd, file_stat.st_mtim.tv_sec, metadata.time);
    char** header;
    calculate_checksum(metadata, header);

    write_typeflag(archive_fd, file_stat.st_mode, metadata.typeflag);
}

void populate_header(FileMetadata metadata, char** header) {
    // my_strncpy2(header, metadata.name, 0, my_strlen(metadata.name));
    // my_strncpy2(header, metadata.mode, my_strlen(metadata.name), 8);
    // my_strncpy2(header, metadata.uid, my_strlen(metadata.name) + 8, 8);
    // my_strncpy2(header, metadata.gid, my_strlen(metadata.name) + 16, 8);

    /* malloc header; header[i] = my_strdup(metadata.name), etc*/

}


void calculate_checksum(FileMetadata metadata, char** header) {
    populate_header(metadata, header);
    printf("header: %s\n", header);
    uint32_t sum = 0;
    for (int i = 0; i < HEADER_SIZE; i++) {
        if (i >= 148 && i < 155) {
            header[i] = ' ';
            continue;
        }
        sum += header[i];
    }

    printf("checksum = %d\n", sum);
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
    int end_index = 7;
    octal_str[end_index--] = '\0'; //null terminate the string

    while (uid != 0) {
        uint8_t octal_digit = uid & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        uid >>= 3;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
    }
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
    int end_index = 7;
    octal_str[end_index--] = '\0';

    while (gid != 0) {
        uint8_t octal_digit = gid & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        gid >>= 3;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
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

void write_time(int archive_fd, time_t sec, char* time_str) {
    //convert time to microsceconds
    long long modification_time = (long long)sec;
    printf("Modification time: %lld\n", modification_time);

    //convert time to string
    int length = 0;
    int max_length = 12;
    while (modification_time > 0 && length < max_length - 1) {
        time_str[length++] = '0' + modification_time % 10;
        modification_time /= 10;
    }
    time_str[max_length - 1] = '\0';

    for (int i = 0; i < length / 2; i++) {
        char temp = time_str[i];
        time_str[i] = time_str[length - i - 1];
        time_str[length - i - 1] = temp;
    }

    write(archive_fd, time_str, length);
}

void write_typeflag(int archive_fd, mode_t mode, char* ocatal_str) {
    mode_t typeflag = mode & S_IFMT; //extract file type bits

    ocatal_str[0] = '0' + typeflag;
    ocatal_str[1] = '\0';

    char whitespace = ' ';
    write(archive_fd, &whitespace, 1);
    write(archive_fd, ocatal_str, my_strlen(ocatal_str));
}