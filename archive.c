#include "main.h"
#include "archive.h"
#include "utils.h"

void create_archive(int argc, char** argv) {
    char* archive_name = argv[2];

    //archive file descriptor
    int archive_fd = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    for (int i = 3; i < argc; i++) {
        write_file_data(archive_fd, argv[i]);
        write_file_content(archive_fd, argv[i]);
    }

    close(archive_fd);
}

void write_file_data(int archive_fd, const char* file_name) {
    struct stat file_stat;

    if (lstat(file_name, &file_stat) == -1) {
        printf("Error retrieving file stats\n"); //remove later
        return;
    }

    file_header file_data;
    file_data.checksum_num = 0;

    get_name(file_name, file_data.name, &file_data.checksum_num);
    get_mode(file_stat.st_mode, file_data.mode, &file_data.checksum_num);
    get_uid(file_stat.st_uid, file_data.uid, &file_data.checksum_num);
    get_uid(file_stat.st_gid, file_data.gid, &file_data.checksum_num);
    get_size(file_stat.st_size, file_data.size, &file_data.checksum_num);
    get_time(file_stat.st_mtim.tv_sec, file_data.time, &file_data.checksum_num);
    get_typeflag(file_stat.st_mode, file_data.typeflag, &file_data.checksum_num);
    checksum(&file_data.checksum_num, USTAR);

    write_stats(archive_fd, file_data);

    // write_mode(archive_fd, file_stat.st_mode, file_data.mode, &file_data.checksum_num);
    // write_uid(archive_fd, file_stat.st_uid, file_data.uid);
    // write_gid(archive_fd, file_stat.st_gid, file_data.gid);
    // write_size(archive_fd, file_stat.st_size, file_data.size);
    // write_time(archive_fd, file_stat.st_mtim.tv_sec, file_data.time);
    
    // write_checksum(archive_fd, file_data, file_data.checksum_str);

    // write_typeflag(archive_fd, file_stat.st_mode, file_data.typeflag);
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

void write_stats(int archive_fd, file_header file_data) {
    write(archive_fd, file_data.name, my_strlen(file_data.name));
    write(archive_fd, file_data.mode, my_strlen(file_data.mode));
    write(archive_fd, file_data.uid, my_strlen(file_data.uid));
    write(archive_fd, file_data.gid, my_strlen(file_data.gid));
    write(archive_fd, file_data.size, my_strlen(file_data.size));
    write(archive_fd, file_data.time, my_strlen(file_data.time));
    write_checksum(archive_fd, file_data.checksum_num, file_data.checksum_str);
    write(archive_fd, file_data.typeflag, my_strlen(file_data.typeflag));
    write(archive_fd, USTAR, my_strlen(USTAR));
}

void get_name(const char* file_name, char* name, unsigned int* sum) {
    my_strncpy(name, file_name, my_strlen(file_name));
    checksum(sum, name);
}

void get_mode(mode_t mode, char* octal_str, unsigned int* sum) {
    mode_t permissions = mode & (S_IRWXU | S_IRWXG | S_IRWXO); //extract file permissions

    int padding = 5;
    for (int i = 0; i < padding - 1; i++) {
        octal_str[i] = '0';
    }
    mode_to_octal(permissions, octal_str, padding - 1);
    checksum(sum, octal_str);
}

void checksum(unsigned int* sum, char* field) {
    printf("field: %s, length: %d ", field, my_strlen(field));

    while (*field) {
        *sum += *field;
        field++;
    }

    printf("checksum = %u\n", *sum);
}

void checksum_to_octal(int checksum, char* octal_str) {
    int end_index = 6;//change to 9 or 10?
    octal_str[end_index--] = '\0';

    while (checksum != 0) {
        uint8_t octal_digit = checksum & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        checksum >>= 3;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
    }

    printf("chksum: %s\n", octal_str);
}

void write_checksum(int archive_fd, unsigned int sum, char* octal_str) {
    checksum_to_octal((int)sum, octal_str);
    write(archive_fd, octal_str, my_strlen(octal_str));
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

// void write_mode(int archive_fd, mode_t mode, char* octal_str) {
//     mode_t permissions = mode & (S_IRWXU | S_IRWXG | S_IRWXO); //extract file permissions

//     int padding = 5;
//     for (int i = 0; i < padding - 1; i++) {
//         octal_str[i] = '0';
//     }
//     mode_to_octal(permissions, octal_str, padding - 1);
//     checksum(sum, octal_str);

//     write(archive_fd, octal_str, my_strlen(octal_str));
// }

void get_uid(uid_t uid, char* octal_str, unsigned int* sum) {
    uid_to_octal(uid, octal_str);
    checksum(sum, octal_str);
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

// void write_uid(int archive_fd, uid_t uid, char* octal_str) {
//     uid_to_octal(uid, octal_str);

//     if (write(archive_fd, octal_str, my_strlen(octal_str)) == -1) {
//         printf("error writing uid\n");
//         return;
//     }
// }

void get_gid(gid_t gid, char* octal_str, unsigned int* sum) {
    gid_to_octal(gid, octal_str);
    checksum(sum, octal_str);
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

// void write_gid(int archive_fd, gid_t gid, char* octal_str) {
//     gid_to_octal(gid, octal_str);
//     write(archive_fd, octal_str, my_strlen(octal_str));
// }

void get_size(size_t size, char* octal_str, unsigned int* sum) {
    size_to_octal(size, octal_str);
    checksum(sum, octal_str);
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

// void write_size(int archive_fd, size_t size, char* octal_str) {
//     size_to_octal(size, octal_str);
//     write(archive_fd, octal_str, my_strlen(octal_str));
// }

void time_to_octal(time_t time, char* octal_str) {
    int end_index = 11;
    octal_str[end_index--] = '\0';

    while (time != 0) {
        uint8_t octal_digit = time & 0b111;
        octal_str[end_index--] = '0' + octal_digit;
        time >>= 3;
    }

    while (end_index >= 0) {
        octal_str[end_index--] = '0';
    }
}

void get_time(time_t time, char* octal_str, unsigned int* sum) {
    time_to_octal(time, octal_str);
    checksum(sum, octal_str);
}

// void write_time(int archive_fd, time_t time, char* octal_str) {
//     time_to_octal(time, octal_str);
//     write(archive_fd, octal_str, my_strlen(octal_str));
// }

void get_typeflag(mode_t mode, char* octal_str, unsigned int* sum) {
    mode_t typeflag = mode & S_IFMT; //extract file type bits

    octal_str[0] = ' ';
    octal_str[1] = '0' + typeflag;
    octal_str[2] = '\0';

    checksum(sum, octal_str);
}

// void write_typeflag(int archive_fd, mode_t mode, char* octal_str) {
//     mode_t typeflag = mode & S_IFMT; //extract file type bits

//     octal_str[0] = ' ';
//     octal_str[1] = '0' + typeflag;
//     octal_str[2] = '\0';

//     write(archive_fd, octal_str, my_strlen(octal_str));
// }