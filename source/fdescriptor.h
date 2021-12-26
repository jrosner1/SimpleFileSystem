#ifndef fdescriptor
#define fdescriptor
struct fd_entry{
    int isAvailable;
    int readPtr;
    int writePtr;
    int  i_node_index;
};

//Every file in directory can be open at once
extern struct fd_entry open_file[80];

//Initializes the file descriptor table
int initialize_descriptor();

//Adds an entry to the descriptor block
int add_descriptor_entry(int i_node_index);

//Removes an entry from the descriptor block
int close_descriptor_entry(int descriptor_index);

//Gets the file descriptor of the FD
struct fd_entry* get_fd_entry(int FD);

//Gets the iNode from the FD
int get_iNode_index(int FD);
#endif