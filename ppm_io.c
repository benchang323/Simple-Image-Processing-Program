/**
 * @file ppm_io.c
 * @author Benjamin Chang (bchang26, 4414D5)/Timothy Lin (tlin56, 70941C)
 * @brief Source file for reading and writing PPM images
 */

// Include the header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "ppm_io.h"

/**
 * Function: read_num
 * ------------------
 * helper function for read_ppm, takes a filehandle and reads a number, but detects and skips comment lines
 * 
 * Parameters:
 *  FILE *fp: file pointer
 * Returns:
 *  -1: If failed to read number from file
 *  val: value of the number
 */
int read_num(FILE *fp) {
    assert(fp);

    int ch;
    while ((ch = fgetc(fp)) == '#') { // # marks a comment line
        while (((ch = fgetc(fp)) != '\n') && ch != EOF) {
            /* discard characters til end of line */
        }
    }
    ungetc(ch, fp); // put back the last thing we found

    int val;
    if (fscanf(fp, "%d", &val) == 1) { // try to get an int
        while (isspace(ch = fgetc(fp))) {
            // drop trailing whitespace
        }
        ungetc(ch, fp);
        return val; // we got a value, so return it
    } else {
        fprintf(stderr, "Error:ppm_io - failed to read number from file\n");
        return -1;
    }
}

/**
 * Function: read_ppm
 * ------------------
 * Read a PPM image from a file pointer and return an Image struct pointer
 * 
 * Parameters:
 *  FILE *fp: file pointer
 * Returns:
 *  Image *: image pointer
 */
Image *read_ppm(FILE *fp) {
    /* confirm that we received a good file handle */
    assert(fp != NULL);

    /* allocate image (but not space to hold pixels -- yet) */
    Image *im = malloc(sizeof(Image));
    if (!im) {
        fprintf(stderr, "Error:ppm_io - failed to allocate memory for image!\n");
        return NULL;
    }

    /* initialize fields to error codes, in case we have to bail out early */
    im->rows = im->cols = -1;

    /* read in tag; fail if not P6 */
    char tag[20];
    tag[19] = '\0';
    fscanf(fp, "%19s\n", tag);
    if (strncmp(tag, "P6", 20)) {
        fprintf(stderr, "Error:ppm_io - not a PPM (bad tag)\n");
        free(im);
        return NULL;
    }

    /* read image dimensions */
    //read in columns
    im->cols = read_num(fp); // NOTE: cols, then rows (i.e. X size followed by Y size)
    //read in rows
    im->rows = read_num(fp);

    //read in colors; fail if not 255
    int colors = read_num(fp);
    if (colors != 255) {
        fprintf(stderr, "Error:ppm_io - PPM file with colors different from 255\n");
        free(im);
        return NULL;
    }

    //confirm that dimensions are positive
    if (im->cols <= 0 || im->rows <= 0) {
        fprintf(stderr, "Error:ppm_io - PPM file with non-positive dimensions\n");
        free(im);
        return NULL;
    }

    /* finally, read in Pixels */
    /* allocate the right amount of space for the Pixels */
    im->data = malloc(sizeof(Pixel) * (im->rows) * (im->cols));

    if (!im->data) {
        fprintf(stderr, "Error:ppm_io - failed to allocate memory for image pixels!\n");
        free(im);
        return NULL;
    }

    /* read in the binary Pixel data */
    if (fread(im->data, sizeof(Pixel), (im->rows) * (im->cols), fp) !=
        (size_t) ((im->rows) * (im->cols))) {
        fprintf(stderr, "Error:ppm_io - failed to read data from file!\n");
        free(im);
        return NULL;
    }

    //return the image struct pointer
    return im;
}

/**
 * Function: write_ppm
 * -------------------
 * Writes the image to the file specified by fp.
 * 
 * Parameters:
 *  FILE* fp: the file to write to
 *  Image* im: the image to write
 * Returns:
 *  -1: faliure occurs
 *  0: success
 */
int write_ppm(FILE *fp, const Image *im) {
    // TODO: IMPLEMENT THIS FUNCTION
    /* initialize fields to error codes, in case we have to bail out early */
    // write tag
    fprintf(fp,"P6\n%d %d\n255\n", im->cols, im->rows);
    
    //if the number of elements printed in the file is not equal to the number of elements we wanted, return -1
    if (fwrite(im->data, sizeof(Pixel), (im->rows) * (im->cols), fp) != (size_t) ((im->rows) * (im->cols))) {
        return -1;
    }

    return 0; // REPLACE THIS STUB
}

/**
 * Function: make_image
 * --------------------
 * Allocate a new image of the specified size, doesn't initialize pixel values
 * 
 * Parameters:
 *  int rows: number of rows in the image
 *  int cols: number of columns in the image
 * Returns:
 *  Image *im: pointer to the image struct
 */
Image *make_image(int rows, int cols) {

    // allocate space
    Image *im = malloc(sizeof(Image));
    if (!im) {
        return NULL;
    }

    // set size 
    im->rows = rows;
    im->cols = cols;

    // allocate pixel array
    im->data = malloc((im->rows * im->cols) * sizeof(Pixel));
    if (!im->data) {
        free(im);
        return NULL;
    }
    return im;
}

/**
 * Function: output_dims
 * ---------------------
 * Outputs the dimensions of the image to standard output.
 * 
 * Parameters:
 *  Image *im: pointer to the image struct
 * Returns:
 *  void
 */
void output_dims(Image *im) {
    printf("cols = %d, rows = %d", im->cols, im->rows);
}

/**
 * Function: free_image
 * --------------------
 * utility function to free inner and outer pointers and set to null 
 * 
 * Parameters:
 *  Image **im: pointer to the image to be freed
 * Returns:
 *  void
 */
void free_image(Image **im) {
    // TODO: IMPLEMENT THIS FUNCTION
    // Remove each Pixel inside the image
    Image * rmv = *im; //we get the value of our current image
    Pixel * currDataPix = rmv->data;
    free(currDataPix);
    free(rmv);
}

/**
 * Function: resize_image
 * ----------------------
 * utility function to reallocate an image to the specified size; doesn't initialize pixel values
 * 
 * Parameters:
 *  Image **im: pointer to the image to be resized
 *  int rows: number of rows
 *  int cols: number of columns
 * Returns:
 *  -1: Error
 *  0: Success
 */
int resize_image(Image **im, int rows, int cols) {
    // Case where invalid grid size
    if (rows < 0 || cols < 0) {
        return -1;
    }
    //reallocates the size of image
    *im = realloc(*im, sizeof(Pixel) * (rows) * (cols));

    //if change size fails, then the program returns error
    if(*im == NULL){
        return -1;
    }

    //case success
    return 0;
}

/**
 * Function: make_copy
 * -------------------
 * copies the image in the parameter
 * 
 * Parameters:
 *  Image *orig: the image to be copied
 * Returns:
 *  Image *: the copied image
 */
Image *make_copy(Image *orig) {
    // Allocate space
    Image *copy = make_image(orig->rows, orig->cols);

    // If we got space, copy pixel values
    if (copy) {
        memcpy(copy->data, orig->data, (copy->rows * copy->cols) * sizeof(Pixel));
    }

    return copy;
}
