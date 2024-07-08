// #include "append.h"
// #include "archive.h"
// #include "main.h"

// void append_archive(int argc, char** argv) {
//     char* archive_name = argv[2]; //file to write in
//     char* file = argv[3]; //might have to add loop for mnutiple files
//     int bytes_read;
//     char* buffer;
//     int archive_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     int position = 0;

//     while (bytes_read != 0) {
//         bytes_read = read(archive_fd, buffer, sizeof(bytes_read));
//         if (archive_fd / 512 == 0) { //buffer[i] -> i = position?
//             position = archive_fd; //use to add padding later
//         }
//     }

//     //open the archive that is being written into
//     //add padding starting at position (loop for multiple files)
//     //write file content into archive
//     //
// }