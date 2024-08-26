#ifndef MAIN_H
#define MAIN_H

#define ERROR_USG "my_tar: You must provide one of the '-Acdtrux', '--delete' or '--test-label' options\n"

int create_archive(int argc, char** argv);
int append_archive(int argc, char** argv);
int list_archive(char* archive_name);
int update_archive(int argc, char** argv);
int extract_archive(char* archive_name);

#endif