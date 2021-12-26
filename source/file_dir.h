#ifndef file_dir
#define file_dir
struct directory_entry{
    char filename[56];
    int available;
    int i_node_index;
};
extern struct directory_entry dir_block[80];
extern int currfile;

// initializes the directory blocks
int initialize_directory();

// gets the next file in the directory
int get_next_file(char **);

//Writes the directory blocks to the disk
int write_dir();

//Gets the inode corresponding to the file name
int get_inode_from_filename(char *);

//Creates a directory entry with iNode at the specified int index
int create_entry(char *, int);

//Gets the iNode index from the file name
int get_inode_from_filename(char *);

//Finds the next directory entry that is not allocated
int find_next_free_dir();

//Returns the size of the file
int get_file_size(char **);

//Removes the file from the directory and returns the iNode index
int remove_from_dir_get_iNode(char *);

#endif