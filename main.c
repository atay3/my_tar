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

    //archive file descriptor
    int archive_fd = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    for (int i = 3; i < argc; i++) {
        write_metadata(archive_fd, argv[i]);
        write_file_content(archive_fd, argv[i]);
    }

    close(archive_fd);
}

void write_metadata(int archive_fd, const char* file_name) {
    struct stat file_stat;

    if (lstat(file_name, &file_stat) == -1) {
        printf("Error retrieving file stats\n"); //remove later
        return;
    }

    struct FileMetadata metadata;

    my_strncpy(metadata.name, file_name, my_strlen(file_name));
    mode_to_octal(file_stat.st_mode, metadata.mode);

    metadata.uid = file_stat.st_uid;
    metadata.gid = file_stat.st_gid;
    metadata.size = file_stat.st_size;
    metadata.time = file_stat.st_mtim;

    if (write(archive_fd, &metadata.name, my_strlen(metadata.name)) != my_strlen(metadata.name)) {
        perror("write file name");
        return;
    }

    write(archive_fd, metadata.mode, my_strlen(metadata.mode));

    // if (write(archive_fd, &metadata.mode, sizeof(metadata.mode)) != sizeof(metadata.mode)) {
    //     perror("write mode");
    //     return;
    // }

    // if (write(archive_fd, &metadata.uid, sizeof(metadata.uid)) != sizeof(metadata.uid)) {
    //     perror("write uid");
    //     return;
    // }

    // if (write(archive_fd, &metadata.gid, sizeof(metadata.gid)) != sizeof(metadata.gid)) {
    //     perror("write gid");
    //     return;
    // }

    // if (write(archive_fd, &metadata.size, sizeof(metadata.size)) != sizeof(metadata.size)) {
    //     perror("write size");
    //     return;
    // }

    // if (write(archive_fd, &metadata.time, sizeof(metadata.time)) != sizeof(metadata.time)) {
    //     perror("write time");
    //     return;
    // }
}

void write_file_content(int archive_fd, const char* file_name) {
    int file_fd = open(file_name, O_RDONLY);

    if (file_fd == -1) {
        printf("Error opening file\n"); //remove later
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(archive_fd, buffer, bytes_read) != bytes_read) {
            printf("Error writing file contents\n"); //remove later
            return;
        }
    }

    close(file_fd);
}

void mode_to_octal(mode_t mode, char* str) {
    //mode_t permissions = mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    //convert the permission bits to octal representation manually
    str[0] = '0' + ((mode >> 6) & 7); //owner permissions
    str[1] = '0' + ((mode >> 3) & 7); //group permissions
    str[2] = '0' + (mode & 7);        //other permissions
    str[3] = '\0';                    //null terminator
}