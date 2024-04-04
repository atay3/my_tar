#include "my_tar.h"

void print_message(char* message) {
    write(1, message, my_strlen(message));
}

void print_error(char* message) {
    write(2, message, my_strlen(message));
}

int my_strlen(const char* str_1) {
    int length = 0;

    while (*str_1 != '\0') {
        length++;
        str_1++;
    }

    return length;
}

// char* my_strdup(const char* str_1) {
//     int length = my_strlen(str_1);
//     char* string = (char*) malloc((length + 1) * sizeof(char));

//     for (int i = 0; i < length; i++) {
//         string[i] = str_1[i];
//     }

//     string[length] = '\0';

//     return string;
// }

char* my_strncpy(char* str_1, const char* str_2, int length) {
    int i;
    for (i = 0; i < length && str_2[i] != '\0'; i++) {
        str_1[i] = str_2[i];
    }
    
    str_1[i] = '\0';

    return str_1;
}

int my_strcmp(const char* str_1, const char* str_2) {
    while (*str_1 != '\0' || *str_2 != '\0') {
        if (*str_1 < *str_2) {
            return -1;
        }
        else if (*str_1 > *str_2) {
            return 1;
        }
        str_1++;
        str_2++;
    }
    return 0;
}