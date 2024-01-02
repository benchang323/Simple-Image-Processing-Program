/**
 * @file ppm_io.h
 * @author Benjamin Chang (bchang26, 4414D5)/Timothy Lin (tlin56, 70941C)
 * @brief Header file for reading and writing PPM images
 */

// If not defined, define the header file
#ifndef PPM_IO_H
#define PPM_IO_H

// Include the header files
#include <stdio.h>
#include <stdlib.h>

// Struct to store a point
typedef struct _point {
  int x;
  int y;
} Point;

// Struct to store an RGB pixel, one byte per channel
typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

// Struct to store an entire image
typedef struct _image {
  Pixel *data;
  int rows;
  int cols;
} Image;

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
Image *read_ppm(FILE *fp);

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
int read_num(FILE *fp);

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
int write_ppm(FILE* fp, const Image* img);

/**
 * Function: make_image
 * --------------------
 * Allocate a new image of the specified size, doesn't initialize pixel values
 * 
 * Parameters:
 *  int rows: number of rows in the image
 *  int cols: number of columns in the image
 * Returns:
 *  Image *: pointer to the image struct
 */
Image *make_image(int rows, int cols);

/**
 * Function: output_dims
 * ---------------------
 * Outputs the dimensions of the image to standard output.
 * 
 * Parameters:
 *  Image *orig: pointer to the image struct
 * Returns:
 *  void
 */
void output_dims(Image *orig);

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
void free_image(Image **im);

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
Image *make_copy(Image *orig);

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
int resize_image(Image **im, int rows, int cols);

// End the header file
#endif
