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

    char end_block[BLOCK_SIZE] = {0};
    write(archive_fd, end_block, BLOCK_SIZE);
    // write(archive_fd, end_block, BLOCK_SIZE);

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

    if (S_ISLNK(file_stat.st_mode)) {
        handle_symlink(file_name, file_data);
    } else pad_symlink(0, file_data.linkname);
    

    get_name(file_name, file_data.name, &file_data.checksum_num);
    get_prefix(file_name, file_data.prefix, &file_data.checksum_num);
    get_mode(file_stat.st_mode, file_data.mode, &file_data.checksum_num);
    get_uid(file_stat.st_uid, file_data.uid, &file_data.checksum_num);
    get_gid(file_stat.st_gid, file_data.gid, &file_data.checksum_num);
    get_size(file_stat.st_size, file_data.size, &file_data.checksum_num);
    get_time(file_stat.st_mtim.tv_sec, file_data.time, &file_data.checksum_num);
    get_typeflag(file_stat.st_mode, file_data.typeflag, &file_data.checksum_num);
    // get_version(file_stat.st_mode, file_data.version, &file_data.checksum_num);
    checksum(&file_data.checksum_num, MAGIC);
    get_user_name(file_stat.st_uid, file_data.user, &file_data.checksum_num);
    get_group_name(file_stat.st_gid, file_data.group, &file_data.checksum_num);
    get_devs(file_stat, file_data.devmajor, file_data.devminor, &file_data.checksum_num);


    write_stats(archive_fd, file_data);
}

void write_file_content(int archive_fd, const char* file_name) {
    int file_fd = open(file_name, O_RDONLY);

    if (file_fd == -1) {
        printf("Error opening file\n"); //remove later
        return;
    }

    char buffer[BLOCK_SIZE];
    ssize_t bytes_read;
    // ssize_t total_bytes_written = 0;

    while ((bytes_read = read(file_fd, buffer, BLOCK_SIZE)) > 0) {
        // write(archive_fd, buffer, bytes_read) != bytes_read
        if (write(archive_fd, buffer, bytes_read) < BLOCK_SIZE) {
            printf("Error writing file contents\n"); //remove later
            return;
        }
        // total_bytes_written += bytes_read;
    }

    close(file_fd);

    // ssize_t padding = BLOCK_SIZE - (total_bytes_written % BLOCK_SIZE);
    // if (padding < BLOCK_SIZE) {
    //     // Manually pad with null bytes
    //     for (ssize_t i = 0; i < padding; i++) {
    //         if (write(archive_fd, "\0", 1) != 1) {
    //             perror("Error writing padding bytes");
    //             close(file_fd);
    //             return;
    //         }
    //     }
    // }
}

void write_stats(int archive_fd, file_header file_data) {
    write(archive_fd, file_data.name, 100);
    write(archive_fd, file_data.mode, 8);
    write(archive_fd, file_data.uid, 8);
    write(archive_fd, file_data.gid, 8);
    write(archive_fd, file_data.size, 12);
    write(archive_fd, file_data.time, 12);
    write_checksum(archive_fd, file_data.checksum_num, file_data.checksum_str);
    write(archive_fd, file_data.typeflag, 1);    
    write(archive_fd, file_data.linkname, 100);
    // write(archive_fd, file_data.version, 2);
    write(archive_fd, MAGIC, 8);
    write(archive_fd, file_data.user, 32);
    write(archive_fd, file_data.group, 32);
    write(archive_fd, file_data.devmajor, 8);
    write(archive_fd, file_data.devminor, 8);
    write(archive_fd, file_data.prefix, 155);
    char pad[12] = {0};
    write(archive_fd, pad, 12);
}

void handle_symlink(const char* file_name, file_header file_data) {
    char link_target[100]; //buffer
    ssize_t length = readlink(file_name, link_target, sizeof(link_target) - 1);

    if (length == -1) {
        printf("Error reading symbolic link\n"); //remove later
        return;
    }
    link_target[length] = '\0';
    // my_strncpy(file_data.linkname, link_target, 100);
    linkname_to_octal(link_target, file_data.linkname);
    if (length < 99) pad_symlink(length, file_data.linkname);
    checksum(&file_data.checksum_num, file_data.linkname);

    // Set the typeflag to indicate a symbolic link?
}

void pad_symlink(int length, char* linkname) {
    for (int i = length; i < 100; i++) {
        linkname[i] = '\0';
    }
}

void linkname_to_octal(char* linkname, char* octal_str) {
    int length = my_strlen(linkname);
    int num = 0;
    int factor = 1;

    //Convert the string to a number
    for (int i = length - 1; i >= 0; i--) {
        num += (linkname[i] - '0') * factor;
        factor *= 10;
    }

    //Convert the number to octal
    int index = 0;
    while (num > 0) {
        octal_str[index++] = '0' + (num % 8);
        num /= 8;
    }

    //Reverse the octal string
    for (int i = 0; i < index / 2; i++) {
        char temp = octal_str[i];
        octal_str[i] = octal_str[index - i - 1];
        octal_str[index - i - 1] = temp;
    }

    octal_str[index] = '\0';
}

void get_name(const char* file_name, char* name, unsigned int* sum) {
    my_strncpy(name, file_name, my_strlen(file_name));
    checksum(sum, name);
    if (my_strlen(name) < 99) {
        for (int i = my_strlen(name); i < 100; i++) {
            name[i] = '\0';
        }
    }
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
    int end_index = 7;
    octal_str[end_index--] = '\0';
    octal_str[end_index--] = ' ';

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
    checksum(&sum, "        \0");
    checksum_to_octal((int)sum, octal_str);
    write(archive_fd, octal_str, 8);
}

void mode_to_octal(mode_t mode, char* str, int start_index) {
    //convert the bits to octal representation manually
    str[start_index] = '0' + ((mode >> 6) & 0b111);
    str[start_index + 1] = '0' + ((mode >> 3) & 0b111);
    str[start_index + 2] = '0' + (mode & 0b111);

    //null terminates the end of the string
    if (start_index + 3 == 7) {
        str[start_index + 3] = '\0';
    }
}

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

void get_typeflag(mode_t mode, char* octal_str, unsigned int* sum) {
    mode_t typeflag = mode & S_IFMT; //extract file type bits
    octal_str[0] = '0' + typeflag;
    checksum(sum, octal_str);
}

// void get_version(mode_t mode, char* octal_str, unsigned int* sum) {
//     octal_str[0] = '0' + ((mode >> 6) & 0b111);
//     octal_str[1] = '0' + ((mode >> 3) & 0b111);
//     // printf("ver : %s", version);
//     checksum(sum, octal_str);
// }

void get_user_name(uid_t uid, char* str, unsigned int* sum) {
    struct passwd* pw;
    pw = getpwuid(uid);
    my_strncpy(str, pw->pw_name, my_strlen(pw->pw_name));
    checksum(sum, str);

    if (my_strlen(str) < 31) {
        for (int i = my_strlen(str); i < 32; i++) {
            str[i] = '\0';
        }
    }
}

void get_group_name(gid_t gid, char* str, unsigned int* sum) {
    struct group* gr;
    gr = getgrgid(gid);
    my_strncpy(str, gr->gr_name, my_strlen(gr->gr_name));
    checksum(sum, str);

    if (my_strlen(str) < 31) {
        for (int i = my_strlen(str); i < 32; i++) {
            str[i] = '\0';
        }
    }
}

void get_devs(struct stat st, char* devmajor, char* devminor, unsigned int* sum) {
    if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
        int_to_octal((st.st_rdev >> 8) & 0xfff, devmajor, 8);
        int_to_octal(st.st_rdev & 0xff, devminor, 8);
        checksum(sum, devmajor);
        checksum(sum, devminor);
    } else {
        for (int i = 0; i < 8; i++) {
            devmajor[i] = '\0';
            devminor[i] = '\0';
        }
        // printf("devmajor: %s\n", devmajor);
        // printf("devminor: %s\n", devminor);
    }
}

void int_to_octal(int value, char* octal_str, int size) {
    for (int i = size - 2; i >= 0; i--) {
        octal_str[i] = '0' + (value & 7); //convert the last 3 bits to an octal digit
        value >>= 3; //shift right by 3 bits (equivalent to dividing by 8)
    }
    octal_str[size - 1] = '\0';
}

void get_prefix(const char* file_name, char* prefix, unsigned int* sum) {
    int file_name_length = my_strlen(file_name);
    if (file_name_length > 100) {
        int prefix_length = file_name_length - 100;
        my_strncpy(prefix, file_name, prefix_length);
        prefix[prefix_length] = '\0';
        checksum(sum, prefix);
    } else {
        for (int i = 0; i < 155; i++) {
            prefix[i] = '\0';
        }
    }
}