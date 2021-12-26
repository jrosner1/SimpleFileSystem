#ifndef bitmap
#define bitmap

//the bitmap that will be kept in memory - each int marks a contiguous block of memory as either open or closed
extern int bitMap[1024];

/* A function that will initialize the bitmap and mark all blocks
 * as open except for the first five.
 * Returns: 0 on success, -1 on failure.
*/
int initialize_bitmap();

/*A function to mark the int block as closed */
int close_block(int);

/*   A function to mark the int block as open
 *   Returns: 0 on success, -1 on failure
 */
int open_block(int);

/*  A function to write the bitmap to the disk
 *  Returns: 0 on success, -1 on failure
*/
int write_bitmap();

/*  A function to read the bitmap from the disk into memory.
 *  Returns: 0 on success, -1 on failure
 */
int read_bitmap()

/*
 * A function to allocate a block for writing data to
 * Returns: the index of the block allocated on success,
 * -1 on failure
 */
int allocate_block();
#endif