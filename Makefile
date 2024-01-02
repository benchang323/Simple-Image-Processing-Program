# Makefile
# Benjamin Chang, bchang26, 4414D5
# Timothy Lin, tlin56, 70941C

# Flags for the compiler
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

# Links files needed to create the main executable
project: ppm_io.o project.o image_manip.o
	$(CC) -o project ppm_io.o project.o image_manip.o -lm

# Create the checkerboard executable
checkerboard: checkerboard.o
	$(CC) -lm -o checkerboard.o

# Create img_comp executable
img_cmp: img_cmp.o
	$(CC) -lm -o img_cmp.o

# Create the object file for image_manip.c
image_manip.o: image_manip.c
	$(CC) $(CFLAGS) -c image_manip.c

# Create the object file for ppm_io.c
ppm_io.o: ppm_io.c
	$(CC) $(CFLAGS) -c ppm_io.c

# Create the object file for project.c
project.o: project.c 
	$(CC) $(CFLAGS) -c project.c

# Create the object file for checkerboard.c
checkerboard.o: checkerboard.c 
	$(CC) $(CFLAGS) -c checkerboard.c

# Create the object file for img_cmp.c
img_cmp.o: img_cmp.c 
	$(CC) $(CFLAGS) -c img_cmp.c

# Removes all object files and the executable
clean:
	rm -f *.o project