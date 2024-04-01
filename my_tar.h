#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#define ERROR_MSG "my_tar: Insufficient number of arguments\n"

void print_message(char* message);
void print_error(char* message);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
int my_strcmp(const char* str_1, const char* str_2);
void create_archive(int argc, char** argv);
void append_archive(int argc, char** argv);
void list_archive(char* archive_name);
void update_archive(int argc, char** argv);
void extract_archive(char* archive_name);

#endif
