
#include <stdlib.h>
#include "../images/input_image.h"
#include "image_functions.h"
#include <sys/time.h>
#include <sys/types.h>

//Global variables in local shared
static unsigned char ImageOut1 [IMAGE_Y*IMAGE_X];
static unsigned char ImageOut2 [IMAGE_Y*IMAGE_X];

int main ()
{
    //--------------------------------------------------
    //---- MEDIAN FILTER
    //--------------------------------------------------
    printf("Start median filter\n");

    //Time
    struct timeval tp;
    double sec, usec, start, end;
    // Time stamp before the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    start = sec + usec;

    histMedian(MyImage,
           ImageOut1,
           IMAGE_X,
           IMAGE_Y);

    // Time stamp after the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    end  = sec + usec;
    // Time calculation (in seconds)
    double time = end - start;

    //Print time
    printf("Total execution time is %f\n", time);

    //--------------------------------------------------
    //---- THRESHOLD FILTER
    //--------------------------------------------------
    printf("Start threshold filter\n");

    // Time stamp before the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    start = sec + usec;
/*
   threshold_equ(ImageOut1,
                  IMAGE_X, 
                  IMAGE_Y, 
                  100); */

    // Time stamp after the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    end  = sec + usec;
    // Time calculation (in seconds)
    time = end - start;

    printf("Total execution time is %f\n", time);

    //--------------------------------------------------
    //---- SOBEL FILTER
    //--------------------------------------------------
    printf("Start sobel filter\n");

        // Time stamp before the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    start = sec + usec;

    sobel(ImageOut1,
          ImageOut2,
          IMAGE_X,
          IMAGE_Y);

    // Time stamp after the computations
    gettimeofday( &tp, NULL );
    sec   = (double) tp.tv_sec;
    usec  = (double) tp.tv_usec /1E6;
    end  = sec + usec;
    // Time calculation (in seconds)
    time = end - start;

    printf("Total execution time is %f\n", time);

    //End
    //Generate output image
    pgmWrite ("results.pgm", IMAGE_X, IMAGE_Y, ImageOut1);

    //End
    return(0);
}

