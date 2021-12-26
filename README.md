# SimpleFileSystem
A simple Unix-like file system that works on top of a disk-write emulator.

# About/Specs
- The file system uses 1024 blocks of 1024 bytes meaning 1,048,576 maximum bytes of storage.
- Allows for a maximum 80 possible files open at once. 
- The directory is a single layer directory, so there cannot be nested folders.
- The file system contains iNodes with 12 direct block pointers and 1 indirect block pointer (with indirect blocks containing 256 block pointers) making for a maximum file size of 12 * 1024 + 12 * 256 = 15,360 bytes.

## Usage

- The file system api is available in the sfs_api.h file. In general, the file system supports creating files, iterating through files, reading and writing files, seeking to locations in files, and removing files.
- In order to compile your own code using the file system api:
  - write to a file with calls to the api
  - replace test.c with your own file in the Makefile
  - compile and run your code
