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

#define KERNEL_SIZE 3

void
inline
swap(unsigned char array[], int i, int j)
{
    unsigned char temp = array[j];
    array[j] = array[i];
    array[i] = temp;
}

void
inline
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

unsigned char
inline
computeMedian(unsigned char array[])
{
    quickSort(array, 0, 8);
    return array[4];
}

//median
void
inline
median (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    unsigned int c,d;
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;

    unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        for ( d=half_kernel_size; d<(size_x-half_kernel_size); d++ )
        {     
            inputBlock[0] = *(ImageIn+(c-1)*size_x+d-1);
            inputBlock[1] = *(ImageIn+c*size_x+d-1);      
            inputBlock[2] = *(ImageIn+(c+1)*size_x+d-1);      
            inputBlock[3] = *(ImageIn+(c-1)*size_x+d);      
            inputBlock[4] = *(ImageIn+c*size_x+d);      
            inputBlock[5] = *(ImageIn+(c+1)*size_x+d);      
            inputBlock[6] = *(ImageIn+(c-1)*size_x+d+1);      
            inputBlock[7] = *(ImageIn+c*size_x+d+1);      
            inputBlock[8] = *(ImageIn+(c+1)*size_x+d+1);      

            *(ImageOut+c*size_x+d) = computeMedian(inputBlock);
        }
    }
}

//threshold_equ
void
inline
threshold_equ(unsigned char *imageIn, unsigned int size_x, unsigned int size_y, unsigned char max)
{
    //Does nothing
    int i;
    for(i=0; i<size_x*size_y; i++)
    {
        unsigned char pixel = *(imageIn+i);
        *(imageIn+i)=(pixel>=100) ? pixel : 0; 
    } 
}

//Sobel
void
inline
sobel (unsigned char *ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    unsigned int c,d;
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;

    unsigned char topLeft, top, topRight, left, right, bottomLeft, bottom, bottomRight;
    //Compute
    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        topLeft = *(ImageIn+(c-1)*size_x+half_kernel_size-1);
        top = *(ImageIn+(c-1)*size_x+half_kernel_size);
        topRight = *(ImageIn+(c-1)*size_x+half_kernel_size+1);
        left = *(ImageIn+c*size_x+half_kernel_size-1);
        right = *(ImageIn+c*size_x+half_kernel_size+1);
        bottomLeft = *(ImageIn+(c+1)*size_x+half_kernel_size-1);
        bottom = *(ImageIn+(c+1)*size_x+half_kernel_size);
        bottomRight = *(ImageIn+(c+1)*size_x+half_kernel_size+1);
        for ( d=half_kernel_size+1; d<(size_x-half_kernel_size); d++ )
        {
            topLeft = top;
            top = topRight;
            bottomLeft = bottom;
            bottom = bottomRight;
            topRight = *(ImageIn+(c-1)*size_x+d+1);
            left = *(ImageIn+c*size_x+d-1);
            right = *(ImageIn+c*size_x+d+1);
            bottomRight = *(ImageIn+(c+1)*size_x+d+1);
            unsigned char horizontal = -topLeft-2*left-bottomLeft+topRight+2*right+bottomRight;
            unsigned char vertical = -topLeft-2*top-topRight+bottomLeft+2*bottom+bottomRight;
            unsigned char result = abs(horizontal)+abs(vertical);
            result = (result > 255) ? 255 : result;
            result = (result < 0) ? 0 : result;
            *(ImageOut+c*size_x+d) = result;
        }
    }
}

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
    if ((file = fopen(filename, "w")) == NULL)	{
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
