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
    char* name;
    size_t name_length;
    size_t size;
    struct timespec time; //modification time
    mode_t mode; //permissions
    uid_t uid; //user id
    gid_t gid; //group id
};

void print_message(char* message);
void print_error(char* message);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
int my_strcmp(const char* str_1, const char* str_2);
void create_archive(int argc, char** argv);
void write_metadata(int archive_file_des, const char* file_name);
void write_file_content(int archive_file_des, const char* file_name);
void append_archive(int argc, char** argv);
void list_archive(char* archive_name);
void update_archive(int argc, char** argv);
void extract_archive(char* archive_name);

#endif
