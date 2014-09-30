
#include <stdlib.h>
#include "../images/input_image.h"
#include "image_functions.h"
#include <time.h>

//Global variables in local shared
static unsigned char ImageOut1 [IMAGE_Y*IMAGE_X];
static unsigned char ImageOut2 [IMAGE_Y*IMAGE_X];

int main ()
{
    //--------------------------------------------------
    //---- MEDIAN FILTER
    //--------------------------------------------------
    printf("Start median filter\n");

    clock_t tic = clock();

    median(MyImage,
           ImageOut1,
           IMAGE_X,
           IMAGE_Y);

    clock_t toc = clock();

    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    //--------------------------------------------------
    //---- THRESHOLD FILTER
    //--------------------------------------------------
    printf("Start threshold filter\n");

    tic = clock();

    threshold_equ(ImageOut1,
                  IMAGE_X, 
                  IMAGE_Y, 
                  100);

    toc = clock();

    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    //--------------------------------------------------
    //---- SOBEL FILTER
    //--------------------------------------------------
    printf("Start sobel filter\n");

    tic = clock();

    sobel(ImageOut1,
          ImageOut2,
          IMAGE_X,
          IMAGE_Y);

        toc = clock();

    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    //End
    //Generate output image
    pgmWrite ("results.pgm", IMAGE_X, IMAGE_Y, ImageOut2);

    //End
    return(0);
}

