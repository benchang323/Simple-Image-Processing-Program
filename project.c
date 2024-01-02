/**
 * @file project.c
 * @author Benjamin Chang (bchang26, 4414D5)/Timothy Lin (tlin56, 70941C)
 * @brief The main execution file for the project
 */

// Include the header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes

// No errors detected AC
#define RC_SUCCESS            0

// Wrong usage (i.e. mandatory arguments are not provided) AC
#define RC_MISSING_FILENAME   1

// Input file I/O error AC
#define RC_OPEN_FAILED        2

// The Input file cannot be read as a PPM file AC
#define RC_INVALID_PPM        3

// Unsupported image processing operations AC
#define RC_INVALID_OPERATION  4

// 	Incorrect number of arguments for the specified operation AC
#define RC_INVALID_OP_ARGS    5

// 	Invalid arguments for the specified operation
#define RC_OP_ARGS_RANGE_ERR  6

// Output file I/O error AC
#define RC_WRITE_FAILED       7

// Other errors not specified above
#define RC_UNSPECIFIED_ERR    8

void print_usage();

int main(int argc, char* argv[]) {
    // Less than 2 command line args means that input or output filename wasn't specified
    if (argc < 3) {
        fprintf(stderr, "Missing input/output filenames\n");
        print_usage();
        return RC_MISSING_FILENAME;
    }


    // Open the input PPM image file
    FILE * inputF = fopen(argv[1], "r");
    // Error checking
    if (inputF == NULL) {
        fprintf(stderr, "Error: Failed to open input file %s for reading\n", argv[1]);
        return RC_OPEN_FAILED;
    }
    Image *input = read_ppm(inputF);
    // Error checking
    if (input == NULL) {
        fprintf(stderr, "Error: Failed to read input file %s as a PPM image file\n", argv[1]);
        return RC_INVALID_PPM;
    }
    // Open the output PPM image file
    FILE *output = fopen(argv[2], "w");


    // Error checking
    if (output == NULL) {
        fprintf(stderr, "Failed to open output file %s for writing\n", argv[2]);
        return RC_WRITE_FAILED; 
    }


    // Check which operation to perform, conduct error checking
    // Swap
    if (strcmp(argv[3], "swap") == 0) {
        if(argc != 4){
            return RC_INVALID_OP_ARGS;
        }
        // Implement swap function
        swap(input);
    }
    // Invert
    else if (strcmp(argv[3], "invert") == 0) {
        if(argc != 4){
            return RC_INVALID_OP_ARGS;
        }
        // Implement invert function
        invert(input);
    }
    // Zoom-out
    else if (strcmp(argv[3], "zoom-out") == 0) {
        if(argc != 4){
            return RC_INVALID_OP_ARGS;
        }
        // Implement zoom-out function
        zoom_out(input);
    }
    // Rotate-right
    else if (strcmp(argv[3], "rotate-right") == 0) {
        if(argc != 4){
            return RC_INVALID_OP_ARGS;
        }
        // Implement rotate-right function
        rotate_right(input);
        // Copy input to output
        write_ppm(output, input);
    }
    // Swirl
    else if (strcmp(argv[3], "swirl") == 0) {
        // Check if the number of arguments is correct
        if (argc != 7) {
            fprintf(stderr, "Error: Incorrect number of arguments for swirl operation (must be 6)\n");
            return RC_INVALID_OP_ARGS;
        }
        // Check if arguments are valid

        

        // the minimum value allowed for the coordinates should be -1
        if (atoi(argv[4]) < -1 || atoi(argv[5]) < -1) {
            fprintf(stderr, "Error: Invalid arguments for swirl operation (must be >= -1)\n");
            return RC_OP_ARGS_RANGE_ERR;
        }

        //check if atoi returns a valid value
        if(atoi(argv[4]) == 0 /* returns 0 upon invalid read*/ && (strcmp(argv[4],"0") != 0)){
            return RC_OP_ARGS_RANGE_ERR;
        }
        if(atoi(argv[5]) == 0 /* returns 0 upon invalid read*/ && (strcmp(argv[5],"0") != 0)){
            return RC_OP_ARGS_RANGE_ERR;
        }
        if(atoi(argv[6]) == 0 /* returns 0 upon invalid read*/ && (strcmp(argv[6],"0") != 0)){
            return RC_OP_ARGS_RANGE_ERR;
        }

        // Get cx, cy, s from command line argument
        double cx = atoi(argv[4]);
        double cy = atoi(argv[5]);
        double s = atoi(argv[6]);

        // Implement swirl function
        swirl(input, cx, cy, s);
    }
    // Edge-detection
    else if (strcmp(argv[3], "edge-detection") == 0) {
        // Check if number of arguments is correct
        if (argc != 5) {
            fprintf(stderr, "Error: Incorrect number of arguments for edge-detection operation (must be 5)\n");
            return RC_INVALID_OP_ARGS;
        }

        //check if atoi returns a valid value
        if(atoi(argv[4]) == 0 /* returns 0 upon invalid read*/ && (strcmp(argv[4],"0") != 0)){
            return RC_OP_ARGS_RANGE_ERR;
        }
        //Get the threshold number from command line
        double threshold = atoi(argv[4]);
        // Implement edge-detection function, ignore error
        edges(input, threshold);

    }
    else  {
        // Error checking
        fprintf(stderr, "Error: Unsupported image processing operation %s specified\n", argv[3]);
        return RC_INVALID_OPERATION;
    }
    

    // Close the input and output files
    fclose(output);
    fclose(inputF);
    free_image(&input);

    // Return success
    return RC_SUCCESS;
}

void print_usage() {
    printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
    printf("SUPPORTED COMMANDS:\n");
    printf("   swap\n");
    printf("   invert\n");
    printf("   zoom-out\n");
    printf("   rotate-right\n");
    printf("   swirl <cx> <cy> <strength>\n");
    printf("   edge-detection <threshold>\n");
}
