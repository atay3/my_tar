#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void print_message(char* message);
void print_error(char* message);
void print_file_dne(char* file_name);
void print_tar_error(char* archive_name);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
char* my_strncpy(char* str_1, const char* str_2, int length);
int my_strcmp(const char* str_1, const char* str_2);
// int my_memcmp(const void *dest, const void *src, size_t n);

#endif