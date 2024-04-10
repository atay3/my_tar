#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>

#define ERROR_MSG "my_tar: Insufficient number of arguments\n"
#define BUFFER_SIZE 1024

struct FileMetadata {
    char name[256];
    char mode[8];
    char uid[8]; //user id
    char gid[8]; //group id
    size_t size; //file size
    struct timespec time; //modification time
};

void print_message(char* message);
void print_error(char* message);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
char* my_strncpy(char* str_1, const char* str_2, int length);
int my_strcmp(const char* str_1, const char* str_2);
void create_archive(int argc, char** argv);
void write_metadata(int archive_fd, const char* file_name);
void write_file_content(int archive_fd, const char* file_name);
void append_archive(int argc, char** argv);
void mode_to_octal(mode_t mode, char* str, int start_index);
void write_mode(int archive_fd, mode_t mode, char* octal_str);
void uid_to_octal(uid_t uid, char* octal_str);
void write_uid(int archive_fd, uid_t uid, char* octal_str);
void list_archive(char* archive_name);
void update_archive(int argc, char** argv);
void extract_archive(char* archive_name);

#endif
