//
//  image_functions.h
//  
//
//  Created by Nicolas Ventroux on 22/08/14.
//
//

#ifndef _IMAGE_FUNCTIONS_H
#define _IMAGE_FUNCTIONS_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "appsupport.h"
#include "omp.h"


#define KERNEL_SIZE 3

unsigned char medianHistogram[256] LOCAL_SHARED;

void
inline
swap(unsigned char array[], int i, int j)
{
    unsigned char temp = array[j];
    array[j] = array[i];
    array[i] = temp;
}

void
quickSort(unsigned char array[], int begin, int end)
{
    if (begin >= end)
        return;
    unsigned char key = array[begin];
    int i = begin+1;
    int j = end;
    while (i < j)
    {
        while (i < j && array[j] >= key)
            --j;
        while (i < j && array[i] <= key)
            ++i;
        if (i < j)
            swap(array, i, j);
    }
    if (array[begin] > array[i])
    {
        swap(array, begin, i);
        quickSort(array, begin, i-1);
        quickSort(array, i+1, end);
    }
    else
    {
        quickSort(array, begin+1, end);
    }
}

void insertSort(unsigned char array[], short begin, short end) {
  short i, k;
  for (i = begin+1; i <= end; i++) {
    k = i;
    while (k > begin && array[k-1] > array[k]) {
      swap(array, k, k-1);
      k--;
  }
}
}

unsigned char
inline
computeMedian(unsigned char array[])
{
  //quickSort(array, 0, 8);
  insertSort(array, 0, 8);
  return array[4];
}


//median
void
inline
median (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
  //Local variables
  int half_kernel_size = (KERNEL_SIZE - 1) / 2;
  unsigned int c, d, acc = half_kernel_size*size_x;

  unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

  for ( c=half_kernel_size; c < (size_y-half_kernel_size); c++ )
  {
    for ( d=half_kernel_size; d<(size_x-half_kernel_size); d++ )
    {
        inputBlock[0] = *(ImageIn+acc-size_x+d-1);
        inputBlock[1] = *(ImageIn+acc+d-1);
        inputBlock[2] = *(ImageIn+acc+size_x+d-1);
        inputBlock[3] = *(ImageIn+acc-size_x+d);
        inputBlock[4] = *(ImageIn+acc+d);
        inputBlock[5] = *(ImageIn+acc+size_x+d);
        inputBlock[6] = *(ImageIn+acc-size_x+d+1);
        inputBlock[7] = *(ImageIn+acc+d+1);
        inputBlock[8] = *(ImageIn+acc+size_x+d+1);

        *(ImageOut+acc+d) = computeMedian(inputBlock);
    }
    acc += size_x;
}
}

// histogram median
void
inline
histMedian (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;
    unsigned int c, d, acc;
    int limit = KERNEL_SIZE*KERNEL_SIZE/2;
    unsigned char i = 0, sum = 0;

    #pragma omp parallel for private(d, acc, i, sum, medianHistogram)
    for ( c=0; c<size_y-2*half_kernel_size; c++ )
    {
        for (d = 0; d < 256; d++)
            medianHistogram[d] = 0;
        acc = half_kernel_size*size_x + c*size_x;

        medianHistogram[*(ImageIn+acc-size_x+half_kernel_size-1)]++;
        medianHistogram[*(ImageIn+acc+half_kernel_size-1)]++;
        medianHistogram[*(ImageIn+acc+size_x+half_kernel_size-1)]++;
        medianHistogram[*(ImageIn+acc-size_x+half_kernel_size)]++;
        medianHistogram[*(ImageIn+acc+half_kernel_size)]++;
        medianHistogram[*(ImageIn+acc+size_x+half_kernel_size)]++;
        medianHistogram[*(ImageIn+acc-size_x+half_kernel_size+1)]++;
        medianHistogram[*(ImageIn+acc+half_kernel_size+1)]++;
        medianHistogram[*(ImageIn+acc+size_x+half_kernel_size+1)]++;
        i = 0;
        sum = 0;
        if (sum < limit)
        {
            do
            {
                sum += medianHistogram[i];
                i++;
            } while (sum < limit);
        }
        *(ImageOut+acc+half_kernel_size) = i;
        for ( d=half_kernel_size+1; d<(size_x-half_kernel_size); d++ )
        {
            medianHistogram[*(ImageIn+acc-size_x+d-2)]--;
            medianHistogram[*(ImageIn+acc+d-2)]--;
            medianHistogram[*(ImageIn+acc+size_x+d-2)]--;
            medianHistogram[*(ImageIn+acc-size_x+d+1)]++;
            medianHistogram[*(ImageIn+acc+d+1)]++;
            medianHistogram[*(ImageIn+acc+size_x+d+1)]++;
            i = 0;
            sum = 0;
            if (sum < limit)
            {
                do
                {
                    sum += medianHistogram[i];
                    i++;
                } while (sum < limit);
            }
            *(ImageOut+acc+d) = i;
        }
    }
}



/*
 *
 */
void
inline
histNoJumpMedian (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    register int half_kernel_size = (KERNEL_SIZE - 1) / 2;
    register unsigned int c,d, acc = half_kernel_size*size_x;

    unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

    register int limit = KERNEL_SIZE*KERNEL_SIZE/2;

    register unsigned char i = 0, sum = 0;

    // Initializing the histogram using the top left block
    for (d = 0; d < 256; d++)
        medianHistogram[d] = 0;

    medianHistogram[*(ImageIn+acc-size_x+half_kernel_size-1)]++;
    medianHistogram[*(ImageIn+acc+half_kernel_size-1)]++;
    medianHistogram[*(ImageIn+acc+size_x+half_kernel_size-1)]++;
    medianHistogram[*(ImageIn+acc-size_x+half_kernel_size)]++;
    medianHistogram[*(ImageIn+acc+half_kernel_size)]++;
    medianHistogram[*(ImageIn+acc+size_x+half_kernel_size)]++;
    medianHistogram[*(ImageIn+acc-size_x+half_kernel_size+1)]++;
    medianHistogram[*(ImageIn+acc+half_kernel_size+1)]++;
    medianHistogram[*(ImageIn+acc+size_x+half_kernel_size+1)]++;

    i = 0;
    sum = 0;

    while (sum < limit)
    {
        sum += medianHistogram[i];
        i++;
    } 

    *(ImageOut+acc+half_kernel_size) = i;

    // The outer loop processes two rows
    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c+=2 )
    {
        // Processing the first row
        for ( d=half_kernel_size+1; d<(size_x-half_kernel_size); d++ )
        {
            medianHistogram[*(ImageIn+acc-size_x+d-2)]--;
            medianHistogram[*(ImageIn+acc+d-2)]--;
            medianHistogram[*(ImageIn+acc+size_x+d-2)]--;
            medianHistogram[*(ImageIn+acc-size_x+d+1)]++;
            medianHistogram[*(ImageIn+acc+d+1)]++;
            medianHistogram[*(ImageIn+acc+size_x+d+1)]++;

            i = 0;
            sum = 0;
            while (sum < limit)
            {
                sum += medianHistogram[i];
                i++;
            } 

            *(ImageOut+acc+d) = i;
        }
        // Moving down one row
        acc += size_x;

        d = size_x-half_kernel_size-1;
        medianHistogram[*(ImageIn+acc-2*size_x+d-1)]--;
        medianHistogram[*(ImageIn+acc-2*size_x+d)]--;
        medianHistogram[*(ImageIn+acc-2*size_x+d+1)]--;
        medianHistogram[*(ImageIn+acc+size_x+d-1)]++;
        medianHistogram[*(ImageIn+acc+size_x+d)]++;
        medianHistogram[*(ImageIn+acc+size_x+d+1)]++;
        i = 0;
        sum = 0;
        while (sum < limit)
        {
            sum += medianHistogram[i];
            i++;
        } 

        *(ImageOut+acc+d) = i;

        // Processing the second row
        for ( d=size_x-half_kernel_size-2 ; d>half_kernel_size-1; d-- )
        {
            medianHistogram[*(ImageIn+acc-size_x+d+2)]--;
            medianHistogram[*(ImageIn+acc+d+2)]--;
            medianHistogram[*(ImageIn+acc+size_x+d+2)]--;
            medianHistogram[*(ImageIn+acc-size_x+d-1)]++;
            medianHistogram[*(ImageIn+acc+d-1)]++;
            medianHistogram[*(ImageIn+acc+size_x+d-1)]++;

            i = 0;
            sum = 0;
            while (sum < limit)
            {
                sum += medianHistogram[i];
                i++;
            } 

            *(ImageOut+acc+d) = i;
        }

        // Moving down one row
        acc += size_x;

        d = half_kernel_size;
        medianHistogram[*(ImageIn+acc-2*size_x+d-1)]--;
        medianHistogram[*(ImageIn+acc-2*size_x+d)]--;
        medianHistogram[*(ImageIn+acc-2*size_x+d+1)]--;
        medianHistogram[*(ImageIn+acc+size_x+d-1)]++;
        medianHistogram[*(ImageIn+acc+size_x+d)]++;
        medianHistogram[*(ImageIn+acc+size_x+d+1)]++;
        i = 0;
        sum = 0;
        while (sum < limit)
        {
            sum += medianHistogram[i];
            i++;
        }

        *(ImageOut+acc+d) = i;
    }
}

/*
Constant Time Histogram Median

Execution length is 77072152 ns
Processor 0 - Start threshold filter 
Execution length is 170374 ns
Processor 0 - Start sobel filter 
Execution length is 728226 ns
*/

//threshold_equ
void
inline
threshold_equ(unsigned char *imageIn, unsigned int size_x, unsigned int size_y, unsigned char max)
{
    int i;
    #pragma omp parallel
    {
        #pragma omp for
        for (i = 0; i < size_x*size_y; i++)
        {
            if (*(imageIn+i) < max)
                *(imageIn+i)=0;    
        }
    }
}


//Sobel
void
inline
sobel (unsigned char *ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    register unsigned char s0, s1, s2, s3, s4, s5, s6, s7, s8;
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;
    unsigned int c, d, acc = half_kernel_size*size_x;
    int horizontal, vertical, result;

    //Compute
    #pragma omp parallel for private(s0, s1, s2, s3, s4, s5, s6, s7, acc, d, horizontal, vertical, result)
    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        acc = (half_kernel_size+c)*size_x;  
        s0 = *(ImageIn+acc-size_x+half_kernel_size-1);
        s1 = *(ImageIn+acc-size_x+half_kernel_size);
        s2 = *(ImageIn+acc-size_x+half_kernel_size+1);
        s3 = *(ImageIn+acc+half_kernel_size-1);
        s4 = *(ImageIn+acc+half_kernel_size+1);
        s5 = *(ImageIn+acc+size_x+half_kernel_size-1);
        s6 = *(ImageIn+acc+size_x+half_kernel_size);
        s7 = *(ImageIn+acc+size_x+half_kernel_size+1);
        for ( d=half_kernel_size; d<(size_x-half_kernel_size); d++ )
        {
            horizontal = -s0-2*s3-s5+s2+2*s4+s7;
            vertical = -s0-2*s1-s2+s5+2*s6+s7;
            result = abs(horizontal)+abs(vertical);
            if (result > 255)
                result = 255;

            *(ImageOut+acc+d) = result;

            s0 = s1;
            s1 = s2;
            s5 = s6;
            s6 = s7;
            s2 = *(ImageIn+acc-size_x+d+2);
            s3 = *(ImageIn+acc+half_kernel_size);
            s4 = *(ImageIn+acc+half_kernel_size+1);
            s7 = *(ImageIn+acc+size_x+half_kernel_size+2);

        }
    }
}

#ifdef X86
//pgmWrite
int 
inline
pgmWrite(char* filename, unsigned int size_x,unsigned int size_y, unsigned char * image)
{
    //Local variables
    FILE* file;
    long nwritten = 0;
    int i;
    
    // open the file
    if ((file = fopen(filename, "w")) == NULL)  {
        printf("ERROR: file open failed\n");
        return(0);
    }
    fprintf(file,"P5\n");
    
    //write the dimensions of the image
    fprintf(file,"%d %d \n", size_x, size_y);
    
    //write MAXIMUM VALUE
    fprintf(file, "%d\n", (int)255);
    
    //Write data
    for (i=0; i < size_y; i++)
        nwritten += fwrite((void*)&(image[i*size_x]),sizeof(unsigned char), size_x, file);
    
    //Close file
    fclose(file);
    
    return(1);
}
#endif

#endif
