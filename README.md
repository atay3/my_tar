# Welcome to My Tar
***

## Task
Write a program `my_tar` that functions similarly to the `tar` command. `my_tar` is a command to manipulate tape archive. The first option to tar is a mode indicator, which can be -a, -r, -t, -u, or -x. There options are applicable using -f.In these modes each specified file or directory is also added to the archive in the order specified on the command line.

## Description
This project is a custom implementation of the `tar` command. The implementation supports several common tar options:

- `-c` for creating a new archive.
- `-t` for listing the contents of an archive.
- `-r` for appending files to an existing archive.
- `-u` for updating files within an archive, appending if the file doesn't exist.
- `-x` for extracting files from an archive.

The `tar` archive format follows the POSIX standard. This project focuses on manually managing file metadata, such as file names, sizes, and timestamps, to effectively manipulate tar archives. These features were implemented without the use of dynamic memory allocation.

## Installation
Users can compile the program using the provided Makefile. Once the repository is cloned, navigate to the project directory and run the following command:
```
make
```

## Usage
After compiling the code, run it by using one of the following:
```
./my_tar -c archive.tar file1 file2 ...
./my_tar -t archive.tar
./my_tar -r archive.tar new_file
./my_tar -u archive.tar existing_file
./my_tar -x archive.tar
```


### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
