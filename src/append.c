#include "append.h"
#include "main.h"
#include "utils.h"

int append_archive(int argc, char** argv) {
    char* archive_name = argv[2];
    int archive_fd = open(archive_name, O_RDWR);
    if (archive_fd < 0) {
        print_tar_error(archive_name);
        return -1;
    }

    off_t pos = lseek(archive_fd, -(15 * BLOCK_SIZE), SEEK_END);
    printf("lseek pos: %ld\n", pos);
    
    posix_header* file_data = NULL;
    // char buffer[BLOCK_SIZE] = {'\0'};
    for (int i = 3; i < argc; i++) {
        // check if file is already in archive
        char* file_name = argv[i];
        int file_fd = open(file_name, O_RDONLY);
        if (file_fd < 0) {
            print_tar_error(file_name);
            return -1;
        }

        file_data = write_file_data_and_return(file_fd, file_name);
        // off_t l = lseek(archive_fd, 0, SEEK_CUR);
        // printf("lseek: %ld\n", l);
        write_stats(archive_fd, *file_data);
        write_file_content(archive_fd, file_name);
        free(file_data);
    }

    // append end of tar file
    print_end_block(archive_fd);
    close(archive_fd);
    return 0;
}

bool is_end_of_archive(posix_header* file_data) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (i == 6) continue;
        if (((const char*)file_data)[i] != '\0') {
            printf("Non-null byte found: %d\n", ((const char*)file_data)[i]);
            return false;
        }
    }
    return true;
}

posix_header* write_file_data_and_return(int archive_fd, const char* file_name) {
    struct stat file_stat;

    if (lstat(file_name, &file_stat) != 0) {
        return NULL;
    }

    posix_header* file_data = malloc(sizeof(posix_header));
    if (file_data == NULL) {
        return NULL;
    }
    file_data->checksum_num = 0;

    if (S_ISLNK(file_stat.st_mode)) {
        handle_symlink(file_name, *file_data);
    } else pad_symlink(0, file_data->linkname);
    
    get_typeflag(file_stat.st_mode, file_data->typeflag, &file_data->checksum_num);
    get_name(file_name, file_data->name, file_data->typeflag[0], &file_data->checksum_num);
    get_prefix(file_name, file_data->prefix, &file_data->checksum_num);
    get_mode(file_stat.st_mode, file_data->mode, &file_data->checksum_num);
    get_uid(file_stat.st_uid, file_data->uid, &file_data->checksum_num);
    get_gid(file_stat.st_gid, file_data->gid, &file_data->checksum_num);
    get_size(file_stat.st_size, file_data->typeflag[0], file_data->size, &file_data->checksum_num);
    get_time(file_stat.st_mtim.tv_sec, file_data->time, &file_data->checksum_num);
    get_version(file_data->version, &file_data->checksum_num);
    checksum(&file_data->checksum_num, MAGIC);
    get_user_name(file_stat.st_uid, file_data->user, &file_data->checksum_num);
    get_group_name(file_stat.st_gid, file_data->group, &file_data->checksum_num);
    get_devs(file_stat, file_data->devmajor, file_data->devminor, &file_data->checksum_num);
    set_offset(file_data->offset);

    if (file_data->typeflag[0] == DIRTYPE) {
        get_dir(archive_fd, file_name);
    }

    return file_data;
}

bool is_valid_header(const posix_header* header) {
    for (int i = 0; i < 100; ++i) {
        if (header->name[i] != '\0') {
            return true;
        }
    }
    return false;
}