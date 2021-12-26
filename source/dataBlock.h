#include "Inode_sfs.h"
#include "fdescriptor.h"
#ifndef dataBlock
#define dataBlock
extern char currBlock[1024];

/**
 * @brief Writes data to the disk
 * 
 * @param buffer The data to write to the file
 * @param length The number of bytes to write to the file
 * @param this_entry the file descriptor entry of the file to write to
 * @param this_iNode The iNode of the file to write to
 * @return int 0 on success, -1 on failure
 */
int write_file(const char** buffer, int length, struct fd_entry* this_entry, struct iNode* this_iNode);

/**
 * @brief Reads data from a file to a buffer
 * 
 * @param buffer The buffer to read the data to
 * @param length The number of bytes to read
 * @param this_entry The file descriptor of the file to read from
 * @param this_iNode The iNode of the file to read from
 * @return int The number of bytes read from the file
 */
int read_file( char* buffer, int length, struct fd_entry* this_entry, struct iNode* this_iNode);

#endif