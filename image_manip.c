/**
 * @file image_manip.c
 * @author Benjamin Chang (bchang26, 4414D5)/Timothy Lin (tlin56, 70941C)
 * @brief Image manipulation functions
 */

// Include header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"

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
unsigned char pixel_to_gray(const Pixel *p) {
  return (unsigned char)((0.3 * (double)p->r) + (0.59 * (double)p->g) + (0.11 * (double)p->b));
}

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
void grayscale(Image *im) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - grayscale given a bad image pointer\n");
    return;
  }

  // Loop through each pixel and convert to grayscale
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      // Get the grayscale intensity of the pixel
      unsigned char grayLevel = pixel_to_gray(&(im->data[(r*im->cols)+c]));
      // Adjust the pixel to be grayscale
      im->data[(r*im->cols)+c].r = grayLevel;
      im->data[(r*im->cols)+c].g = grayLevel;
      im->data[(r*im->cols)+c].b = grayLevel;
    }
  }
}


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
void swap(Image *im) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - swap given a bad image pointer\n");
    return;
  }

  // Loop through each pixel and swap the color channels
  for (int r = 0; r < im->rows; r++){
    for (int c = 0; c < im->cols; c++){
      // Swap green to red, swap blue to green, swap red to blue
      unsigned char temp = im->data[(r*im->cols)+c].r;
      im->data[(r*im->cols)+c].r = im->data[(r*im->cols)+c].g;
      im->data[(r*im->cols)+c].g = im->data[(r*im->cols)+c].b;
      im->data[(r*im->cols)+c].b = temp;
    }
  }
}

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
void invert(Image *im) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - invert given a bad image pointer\n");
    return;
  }

  // Loop through each pixel and invert the color channels
  for (int r = 0; r < im->rows; r++){
    for (int c = 0; c < im->cols; c++){
      // Invert the color channels by subtracting its value from 255
      im->data[(r*im->cols)+c].r = 255 - im->data[(r*im->cols)+c].r;
      im->data[(r*im->cols)+c].g = 255 - im->data[(r*im->cols)+c].g;
      im->data[(r*im->cols)+c].b = 255 - im->data[(r*im->cols)+c].b;
    }
  }
}

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
void zoom_out(Image *im) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - zoom_out given a bad image pointer\n");
    return;
  }
  /* 
  In order to perform a zoom out, we take a 2X2 square of pixels in the input 
  image and average each of the three color channels of the four pixels to make 
  a single pixel. This means a zoomed out picture has half as many rows and half 
  as many columns as the original image. However, note that the number of rows 
  and/or columns in the input image might be odd, in which case we lose info about 
  the bottom row and/or rightmost column.
  */
  // Create a new image with half the rows and columns of the original image
  Image *newImage = (Image *)malloc(sizeof(Image));
  newImage->rows = im->rows / 2;
  newImage->cols = im->cols / 2;
  newImage->data = (Pixel *)malloc(sizeof(Pixel) * newImage->rows * newImage->cols);

  // Loop through each pixel in the new image
  for (int r = 0; r < newImage->rows; r++) {
    for (int c = 0; c < newImage->cols; c++) {
      // Get the average of the four pixels in the original image
      unsigned char avgR = (im->data[((2*r)*im->cols)+(2*c)].r + im->data[((2*r)*im->cols)+(2*c)+1].r + im->data[((2*r)+1)*im->cols+(2*c)].r + im->data[((2*r)+1)*im->cols+(2*c)+1].r) / 4;
      unsigned char avgG = (im->data[((2*r)*im->cols)+(2*c)].g + im->data[((2*r)*im->cols)+(2*c)+1].g + im->data[((2*r)+1)*im->cols+(2*c)].g + im->data[((2*r)+1)*im->cols+(2*c)+1].g) / 4;
      unsigned char avgB = (im->data[((2*r)*im->cols)+(2*c)].b + im->data[((2*r)*im->cols)+(2*c)+1].b + im->data[((2*r)+1)*im->cols+(2*c)].b + im->data[((2*r)+1)*im->cols+(2*c)+1].b) / 4;
      // Set the pixel in the new image to the average of the four pixels
      newImage->data[(r*newImage->cols)+c].r = avgR;
      newImage->data[(r*newImage->cols)+c].g = avgG;
      newImage->data[(r*newImage->cols)+c].b = avgB;
    }
  }

  // Free the old image and set the pointer to the new image
  free(im->data);
  im->rows = newImage->rows;
  im->cols = newImage->cols;
  im->data = newImage->data;
  free(newImage);
}

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
void rotate_right(Image *im) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - rotate_right given a bad image pointer\n");
    return;
  }

  // First you should allocate a new image with reversed dimensions (width and height) of the input image
  Image *newImage = (Image *)malloc(sizeof(Image));
  newImage->rows = im->cols;
  newImage->cols = im->rows;
  newImage->data = (Pixel *)malloc(sizeof(Pixel)*newImage->rows*newImage->cols);

  // Loop through each pixel and rotate the image clockwise by 90 degrees
  for (int r = 0; r < im->rows; r++){
    for (int c = 0; c < im->cols; c++){
      // Use a loop to assign each new pixel value using the corresponding cell in the original image. 
      newImage->data[(c*newImage->cols)+(newImage->cols-1-r)].r = im->data[(r*im->cols)+c].r;
      newImage->data[(c*newImage->cols)+(newImage->cols-1-r)].g = im->data[(r*im->cols)+c].g;
      newImage->data[(c*newImage->cols)+(newImage->cols-1-r)].b = im->data[(r*im->cols)+c].b;
    }
  }

  // Free the old image and set the pointer to the new image
  free(im->data);
  im->rows = newImage->rows;
  im->cols = newImage->cols;
  im->data = newImage->data;
  free(newImage);
}

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
void swirl(Image *im, double cx, double cy, double s) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - swirl given a bad image pointer\n");
    return;
  }

  if(cx == -1){
    cx = im->cols/2;
  }
  if(cy == -1){
    cy = im->rows/2;
  }

  /*
  Is(x, y) is the pixel at location (x, y) in the swirled image and Io((x - cx) cos α - (y - cy) sin α + cx, (x - cx) sin α + (y - cy) cos α + cy) 
  s the pixel at coordinates (x - cx) cos α - (y - cy) sin α + cx and (x - cx) sin α + (y - cy) cos α + cy in the original image.
  Alpha is sqrt((x - cx)^2 + (y - cy)^2) / s
  Is (x, y) = Io((x - cx) cos α - (y - cy) sin α + cx, (x - cx) sin α + (y - cy) cos α + cy)
  */
  // First you should allocate a new image with the same dimensions as the input image
  Image *newImage = (Image *)malloc(sizeof(Image));
  newImage->rows = im->rows;
  newImage->cols = im->cols;
  newImage->data = (Pixel *)malloc(sizeof(Pixel)*newImage->rows*newImage->cols);

  // Loop through each pixel and swirl the image
  for (int r = 0; r < im->rows; r++){
    for (int c = 0; c < im->cols; c++){
      // Then, you use a loop to assign each new pixel value using the corresponding cell in the original image. 
      double alpha = sqrt(pow(((double)c - (double)cx), 2) + pow(((double)r - (double)cy), 2)) / s;
      int newC = (c - cx) * cos(alpha) - (r - cy) * sin(alpha) + cx;
      int newR = (c - cx) * sin(alpha) + (r - cy) * cos(alpha) + cy;
      // Check if the new coordinates are out of bounds
      if (newC < 0 || newC >= im->cols || newR < 0 || newR >= im->rows) {
        newImage->data[(r*newImage->cols)+c].r = 0;
        newImage->data[(r*newImage->cols)+c].g = 0;
        newImage->data[(r*newImage->cols)+c].b = 0;
      } else {
        newImage->data[(r*newImage->cols)+c].r = im->data[(newR*im->cols)+newC].r;
        newImage->data[(r*newImage->cols)+c].g = im->data[(newR*im->cols)+newC].g;
        newImage->data[(r*newImage->cols)+c].b = im->data[(newR*im->cols)+newC].b;
      }
    }
  }
  // Free the old image and set the pointer to the new image
  free(im->data);
  im->rows = newImage->rows;
  im->cols = newImage->cols;
  im->data = newImage->data;
  free(newImage);
}

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
void edges(Image *im, double threshold) {
  // Error check
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - edges given a bad image pointer\n");
    return;
  }

  // First convert the image to grayscale
  grayscale(im);

  int count = 0;

  Image * newImage = (Image *)malloc(sizeof(Image));
  newImage->rows = im->rows;
  newImage->cols = im->cols;
  newImage->data = (Pixel *)malloc(sizeof(Pixel)*newImage->rows*newImage->cols);

  // Compute the intensity gradient for each interior point (i.e. points not on the boundary) of the image in both the horizontal (x) and vertical (y) directions
  for (int r = 0; r < im->rows; r++){
    for (int c = 0; c < im->cols; c++){
      //edges
      if(r == 0 || c == 0 || r == im->rows-1 || c == im->cols-1){
        newImage->data[(r*im->cols)+c].r = im->data[(r*im->cols)+c].r ;
        newImage->data[(r*im->cols)+c].g = im->data[(r*im->cols)+c].r ;
        newImage->data[(r*im->cols)+c].b = im->data[(r*im->cols)+c].r ;
	continue;
      }

      // gradient x = (I(x + 1, y) - I(x - 1, y)) / 2
      // gradient y = (I(x, y + 1) - I(x, y - 1)) / 2
      // gradient magnitude = sqrt(gradient x^2 + gradient y^2)

      int intensityAt1Up, intensityAt1Down, intensityAt1Left, intensityAt1Right;

      intensityAt1Up = im->data[((r-1)*im->cols)+c].g;
      intensityAt1Down = im->data[((r+1)*im->cols)+c].g;
      intensityAt1Left = im->data[((r)*im->cols)+(c-1)].g;
      intensityAt1Right = im->data[((r+1)*im->cols)+(c+1)].g;


      double gradientX = (double)(intensityAt1Left - intensityAt1Right) / 2;
      double gradientY = (double)(intensityAt1Up - intensityAt1Down) / 2;
      double gradientMagnitude = sqrt(pow(gradientX, 2) + pow(gradientY, 2));
      // Threshold each pixel and classify it as an edge or not an edge
      // Set the values of all channels to 0 (black) if the magnitude exceeds the threshold, else set the values to 255 (white)
      // Ignore the boundary points and leave them as they are
      
      if (gradientMagnitude < threshold){
        newImage->data[(r*im->cols)+c].r = 255;
        newImage->data[(r*im->cols)+c].g = 255;
        newImage->data[(r*im->cols)+c].b = 255;
      } else {
        count++;
        newImage->data[(r*im->cols)+c].r = 0;
        newImage->data[(r*im->cols)+c].g = 0;
        newImage->data[(r*im->cols)+c].b = 0;
      }
    }
  }

  free(im->data);
  im->data = newImage->data;
  free(newImage);

  //Print count
} 

// Change threshold (edge detection) to double
// Finish Error Handling
// Run Valgrind again
// Test edge cases tomorrow