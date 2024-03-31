#include "my_tar.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        print_message(ERROR_MSG);
        return EXIT_FAILURE;
    }

    printf("Hello world\n%d%s", argc, argv[0]);

    return 0;
}