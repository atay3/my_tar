#include "main.h"
#include "utils.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        print_error(ERROR_USG);
        return EXIT_FAILURE;
    }

    char* option = argv[1];
    int status = 0;

    if (my_strcmp("-cf", option) == 0) {
	    status = create_archive(argc, argv);
    }
    else if (my_strcmp("-rf", option) == 0) {
	    status = append_archive(argc, argv);
    }
    else if (my_strcmp("-tf", option) == 0) {
	    status = list_archive(argv[2]);
    }
    else if (my_strcmp("-uf", option) == 0) {
	    status = update_archive(argc, argv);
    }
    else if (my_strcmp("-xf", option) == 0) {
        status = extract_archive(argv[2]);
    }

    return status;
}