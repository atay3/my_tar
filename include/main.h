#ifndef MAIN_H
#define MAIN_H


#define ERROR_MSG "my_tar: Insufficient number of arguments\n"

void create_archive(int argc, char** argv);
void append_archive(int argc, char** argv);
void list_archive(char* archive_name);
void update_archive(int argc, char** argv);
void extract_archive(char* archive_name);

#endif