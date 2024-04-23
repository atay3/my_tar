#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdlib.h> //utils.c
#include <unistd.h> //archive.c
#include <fcntl.h> //archive.c
#include <sys/stat.h> //archive.c
// #include <sys/types.h>
#include <stdint.h> //archive.c
#include <time.h> //archive.c

#include <stdio.h>

#define ERROR_MSG "my_tar: Insufficient number of arguments\n"
#define BUFFER_SIZE 1024
#define HEADER_SIZE 512

typedef struct {
    char name[256]; //file name
    char mode[8]; //file permissions
    char uid[8]; //user id
    char gid[8]; //group id
    char size[12]; //file size
    char time[12]; //modification time
    char checksum[7]; //checksum
    char typeflag[2]; //file type
}FileMetadata;

void print_message(char* message);
void print_error(char* message);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
char* my_strncpy(char* str_1, const char* str_2, int length);
int my_strcmp(const char* str_1, const char* str_2);
char* my_strncpy2(char* str_1, const char* str_2, int start_index, int length);

void create_archive(int argc, char** argv);
void write_metadata(int archive_fd, const char* file_name);
void write_file_content(int archive_fd, const char* file_name);
void append_archive(int argc, char** argv);
void mode_to_octal(mode_t mode, char* str, int start_index);
void write_mode(int archive_fd, mode_t mode, char* octal_str);
void uid_to_octal(uid_t uid, char* octal_str);
void write_uid(int archive_fd, uid_t uid, char* octal_str);
void gid_to_octal(gid_t gid, char* octal_str);
void write_gid(int archive_fd, gid_t gid, char* octal_str);
void size_to_octal(size_t size, char* octal_str);
void write_size(int archive_fd, size_t size, char* octal_str);
void write_time(int archive_fd, time_t sec, char* time_str);
// void populate_header(FileMetadata metadata, char* header);
void calculate_checksum(FileMetadata metadata, char* header);
void write_checksum();
void write_typeflag(int archive_fd, mode_t mode, char* ocatal_str);

void list_archive(char* archive_name);
void update_archive(int argc, char** argv);
void extract_archive(char* archive_name);

#endif