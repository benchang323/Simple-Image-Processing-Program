/**
 * @file image_manip.h
 * @author Benjamin Chang (bchang26, 4414D5)/Timothy Lin (tlin56, 70941C)
 * @brief Header file for image manipulation functions
 */

// If not defined, define IMAGE_MANIP_H
#ifndef IMAGE_MANIP_H
#define IMAGE_MANIP_H

// Include header files
#include "ppm_io.h"

// store PI as a constant
#define PI 3.14159265358979323846

// macro to square a number
#define sq(X) ((X) * (X))

// macro to find the max of a number
#define MAX(a,b) ((a > b) ? (a) : (b))

/**
 * Function: pixel_to_gray
 * -----------------------
 * Convert a RGB pixel to a single grayscale intensity using NTSC standard conversion
 * 
 * Parameters:
 *  const pixel *p: the pixel to be converted
 * Return:
 *  the grayscale intensity of the pixel (unsigned char)
 */
unsigned char pixel_to_gray(const Pixel *p);

/**
 * Function: grayscale
 * -------------------
 * Convert an image to grayscale (pixels are still RGB, but the three values will be equal)
 * 
 * Parameters:
 *  Image *im: the image to be grayscaled
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void grayscale(Image *im);

/** 
 * Function: swap
 * --------------
 * Swap color of an image (R <-> G, G <-> B, B <-> R)
 * 
 * Parameters:
 *  Image *im: the image to be swapped
 * Return:
 *  void (image itself is already modified since it is a pointer)
*/
void swap(Image *im);

/**
 * Function: invert
 * ----------------
 * Invert the intensity of each color channel
 * 
 * Parameters:
 *  Image *im: the image to be inverted
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void invert(Image *im);

/**
 * Function: zoom_out
 * ------------------
 * Zoom out the image by a factor of 2
 * 
 * Parameters:
 *  Image *im: the image to be zoomed out
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void zoom_out(Image *im);

/**
 * Function: rotate_right
 * ----------------------
 * Rotate an image clockwise by 90 degrees
 * 
 * Parameters:
 *  Image *im: the image to be rotated
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void rotate_right(Image *im);

/**
 * Function: swirl
 * ---------------------
 * Swirl the image using the given formula 
 * 
 * Parameters:
 *  Image *im: the image to be swirled
 *  int cx: the x coordinate of the center of the swirl
 *  int cy: the y coordinate of the center of the swirl
 *  int s: the strength of the swirl
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void swirl(Image *im, double cx, double cy, double s);

/**
 * Function: edges
 * ---------------
 * The function detects edges in the image 
 * 
 * Parameters:
 *  Image *im: the image to be swirled
 *  int threshold: the threshold to be used in the swirl
 * Return:
 *  void (image itself is already modified since it is a pointer)
 */
void edges(Image *im, double threshold);

// End of header file
#endif
