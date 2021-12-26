CFLAGS = -c -g -ansi -pedantic -Wall -std=gnu99 `pkg-config fuse --cflags --libs`

LDFLAGS = `pkg-config fuse --cflags --libs`

SOURCES= disk_emu.c indirectBlock.c dataBlock.c file_dir.c fdescriptor.c Inode_sfs.c sfs_api.c Super_block.c bitmap.c sfs_test.c sfs_api.h Inode_sfs.h Super_block.h bitmap.h file_dir.h fdescriptor.h dataBlock.h indirectBlock.h

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sfs

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~ $(EXECUTABLE)
	rm sfs.FILE
