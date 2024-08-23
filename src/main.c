#include "main.h"
#include "utils.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        print_error(ERROR_MSG);
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
	    //list archive contents to stdout
    }
    else if (my_strcmp("-uf", option) == 0) {
	    //new entries are added only if they have a modification date newer than the corresponding entry in the archive. the -f option is req
    }
    else if (my_strcmp("-xf", option) == 0) {
	    //extract to disk from the archive. if a file with the smae name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies
    }



    return status;
}