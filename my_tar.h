#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#define ERROR_MSG "my_tar: Insufficient number of arguments\n"

void print_message(char* message);
int my_strlen(const char* str_1);
char* my_strdup(const char* str_1);
int my_strcmp(const char* str_1, const char* str_2);

#endif