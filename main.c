#include "my_tar.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        print_message(ERROR_MSG);
        return EXIT_FAILURE;
    }

    //printf("Hello world\n%d%s", argc, argv[0]);

    if (my_strcmp("-c", argv[1]) {
	//create new archive containing the specified items
    }
    else if (my_strcmp("-r", argv[1]) {
	//like -c but new entries are appended to the archive. the -f option is req"
    }
    else if (my_strcmp("-t", argv[1]) {
	//list archive contents to stdout
    }
    else if (my_strcmp("-u", argv[1]) {
	//like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. the -f option is req
    }
    else if (my_strcmp("-x", argv[1]) {
	//extract to disk from the archive. if a file with the smae name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies
    }


    return 0;
}
