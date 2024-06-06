#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define BUFFER_SIZE 1024
// #define BLOCK_SIZE 512
#define USTAR "ustar  \0"

typedef struct {
    char name[256]; // file name
    char mode[8]; // file permissions
    char uid[8]; // user id
    char gid[8]; // group id
    char size[12]; // file size
    char time[12]; // modification time
    char checksum_str[8]; // checksum
    unsigned int checksum_num;
    char typeflag[2]; // file type
    char user[32]; // user name
    char group[32]; // group name
}file_header;

void write_file_data(int archive_fd, const char* file_name);
void write_file_content(int archive_fd, const char* file_name);
void write_stats(int archive_fd, file_header file_data);
void get_name(const char* file_name, char* name, unsigned int* sum);
void get_mode(mode_t mode, char* octal_str, unsigned int* sum);
void mode_to_octal(mode_t mode, char* str, int start_index);

void get_uid(uid_t uid, char* octal_str, unsigned int* sum);
void uid_to_octal(uid_t uid, char* octal_str);
void get_gid(gid_t gid, char* octal_str, unsigned int* sum);
void gid_to_octal(gid_t gid, char* octal_str);
void get_size(size_t size, char* octal_str, unsigned int* sum);
void size_to_octal(size_t size, char* octal_str);
void get_time(time_t time, char* octal_str, unsigned int* sum);
void time_to_octal(time_t time, char* octal_str);
void checksum(unsigned int* sum, char* field);
void checksum_to_octal(int checksum, char* octal_str);
void write_checksum(int archive_fd, unsigned int sum, char* octal_str);
void get_typeflag(mode_t mode, char* octal_str, unsigned int* sum);
void get_user_name(uid_t uid, char* str, unsigned int* sum);
void get_group_name(gid_t gid, char* str, unsigned int* sum);

#endif