#ifndef APPEND_H
#define APPEND_H

#define BUFFER_SIZE 1024
#define OLDGNU_MAGIC "ustar  "

#include <unistd.h>
#include <stdbool.h>
#include "archive.h"

bool is_end_of_archive(posix_header* file_data);
bool is_valid_header(const posix_header* header);

#endif