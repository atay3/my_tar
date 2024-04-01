#include "my_tar.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        print_error(ERROR_MSG);
        return EXIT_FAILURE;
    }

    char* option = argv[1];

    if (my_strcmp("-c", option) == 0 || my_strcmp("-cf", option) == 0) {
	    create_archive(argc, argv);
    }
    else if (my_strcmp("-r", option) == 0 || my_strcmp("-rf", option) == 0) {
	    //like -c but new entries are appended to the archive. the -f option is req"
    }
    else if (my_strcmp("-t", option) == 0 || my_strcmp("-tf", option) == 0) {
	    //list archive contents to stdout
    }
    else if (my_strcmp("-u", option) == 0 || my_strcmp("-uf", option) == 0) {
	    //like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. the -f option is req
    }
    else if (my_strcmp("-x", option) == 0 || my_strcmp("-xf", option) == 0) {
	    //extract to disk from the archive. if a file with the smae name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies
    }

    return 0;
}

void create_archive(int argc, char** argv) {
    char* archive_name = argv[2];

    int file_descriptor = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}
