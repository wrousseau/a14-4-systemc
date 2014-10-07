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

#define KERNEL_SIZE 3

unsigned char histogram[256] LOCAL_SHARED;


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
    unsigned int c, d, acc = half_kernel_size*size_x;

    unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

    int limit = KERNEL_SIZE*KERNEL_SIZE/2;

    unsigned char i = 0, sum = 0;

    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        for (d = 0; d < 256; d++)
            histogram[d] = 0;

        histogram[*(ImageIn+acc-size_x+half_kernel_size-1)]++;
        histogram[*(ImageIn+acc+half_kernel_size-1)]++;
        histogram[*(ImageIn+acc+size_x+half_kernel_size-1)]++;
        histogram[*(ImageIn+acc-size_x+half_kernel_size)]++;
        histogram[*(ImageIn+acc+half_kernel_size)]++;
        histogram[*(ImageIn+acc+size_x+half_kernel_size)]++;
        histogram[*(ImageIn+acc-size_x+half_kernel_size+1)]++;
        histogram[*(ImageIn+acc+half_kernel_size+1)]++;
        histogram[*(ImageIn+acc+size_x+half_kernel_size+1)]++;

        i = 0;
        sum = 0;

        while (sum < limit)
        {
            sum += histogram[i];
            i++;
        }

        *(ImageOut+acc+half_kernel_size) = i;

        for ( d=half_kernel_size+1; d<(size_x-half_kernel_size); d++ )
        {
            histogram[*(ImageIn+acc-size_x+d-2)]--;
            histogram[*(ImageIn+acc+d-2)]--;
            histogram[*(ImageIn+acc+size_x+d-2)]--;
            histogram[*(ImageIn+acc-size_x+d+1)]++;
            histogram[*(ImageIn+acc+d+1)]++;
            histogram[*(ImageIn+acc+size_x+d+1)]++;

            i = 0;
            sum = 0;
            while (sum < limit)
            {
                sum += histogram[i];
                i++;
            }

            *(ImageOut+acc+d) = i;
        }
        acc += size_x;
    }
}

//threshold_equ
void
inline
threshold_equ(unsigned char *imageIn, unsigned int size_x, unsigned int size_y, unsigned char max)
{
    unsigned int i;
    for(i=0; i<size_x*size_y; i++)
    {
      if (*(imageIn+i) < 100)
        *(imageIn+i) = 0;
    }
}

//Sobel
void
inline
sobel (unsigned char *ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables

    int half_kernel_size = (KERNEL_SIZE - 1) / 2;
    unsigned int c, d, acc = half_kernel_size*size_x;

    unsigned char topLeft, top, topRight, left, right, bottomLeft, bottom, bottomRight;
    //Compute
    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        for ( d=half_kernel_size; d<(size_x-half_kernel_size); d++ )
        {
            topLeft = *(ImageIn+acc-size_x+d-1);
            top = *(ImageIn+acc-size_x+d);
            topRight = *(ImageIn+acc-size_x+d+1);
            left = *(ImageIn+acc+d-1);
            right = *(ImageIn+acc+d+1);
            bottomLeft = *(ImageIn+acc+size_x+d-1);
            bottom = *(ImageIn+acc+size_x+d);
            bottomRight = *(ImageIn+acc+size_x+d+1);
            int horizontal = -topLeft-2*left-bottomLeft+topRight+2*right+bottomRight;
            int vertical = -topLeft-2*top-topRight+bottomLeft+2*bottom+bottomRight;
            int temp = abs(horizontal)+abs(vertical);
            unsigned char result = (temp > 255) ? 255 : temp;

            *(ImageOut+acc+d) = result;
        }
        acc += size_x;
    }
}

//Insertion d'un élément dans un tableau trié
void inline insert(unsigned char tab[], unsigned char x, short end) {
  register short i = end;
  while (i >= 0 && tab[i] > x) {
    tab[i+1] = tab[i];
    i--;
  }
  tab[i+1] = x;
}

//Décalage à gauche des colonnes d'une matrice 3*3
void inline dec(unsigned char tab[]) {
  tab[0] = tab[1];
  tab[1] = tab[2];
  tab[3] = tab[4];
  tab[4] = tab[5];
  tab[6] = tab[7];
  tab[7] = tab[8];
}

void allInTheSame (unsigned char *ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y) {
  unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];
  unsigned char thresholdValues[KERNEL_SIZE*KERNEL_SIZE];
  register short accPetit, k, l;
  register int horizontal, vertical;
  register unsigned int c, d, accGrand;

  //Coin en haut à gauche
  accGrand = size_x;
  accPetit = KERNEL_SIZE;
  thresholdValues[0] = (*(ImageIn) > 100) ? *(ImageIn) : 0;
  *(ImageOut) = thresholdValues[0];
  thresholdValues[1] = (*(ImageIn+1) > 100) ? *(ImageIn+1) : 0;
  *(ImageOut+1) = thresholdValues[1];
  thresholdValues[2] = (*(ImageIn+2) > 100) ? *(ImageIn+2) : 0;
  *(ImageOut+2) = thresholdValues[2];
  thresholdValues[3] = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
  thresholdValues[6] = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;
  for (k = 1; k < 3; k++) {
    for (l = 1; l < 3; l++) {
      inputBlock[0] = *(ImageIn+l-1+accGrand-size_x);
      insert(inputBlock, *(ImageIn+l+accGrand-size_x), 0);
      insert(inputBlock, *(ImageIn+l+1+accGrand-size_x), 1);
      insert(inputBlock, *(ImageIn+l-1+accGrand), 2);
      insert(inputBlock, *(ImageIn+l+accGrand), 3);
      insert(inputBlock, *(ImageIn+l+1+accGrand), 4);
      insert(inputBlock, *(ImageIn+l-1+accGrand+size_x), 5);
      insert(inputBlock, *(ImageIn+l+accGrand+size_x), 6);
      insert(inputBlock, *(ImageIn+l+1+accGrand+size_x), 7);
      thresholdValues[l+accPetit] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
    }
    accGrand += size_x;
    accPetit += KERNEL_SIZE;
  }
  horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
    -thresholdValues[6] + thresholdValues[8];
  vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
    + thresholdValues[7] + thresholdValues[8];
  *(ImageOut+size_x+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

  //Première ligne
  for (d = 2; d < size_x - 2; d++) {
    dec(thresholdValues);
    accGrand = size_x;
    accPetit = KERNEL_SIZE;
    thresholdValues[2] = (*(ImageIn+d+1) > 100) ? *(ImageIn+d+1) : 0;
    *(ImageOut+d+1) = thresholdValues[2];
    for (k = 1; k < 3; k++) {
      inputBlock[0] = *(ImageIn+d+accGrand-size_x);
      insert(inputBlock, *(ImageIn+d+1+accGrand-size_x), 0);
      insert(inputBlock, *(ImageIn+d+2+accGrand-size_x), 1);
      insert(inputBlock, *(ImageIn+d+accGrand), 2);
      insert(inputBlock, *(ImageIn+d+1+accGrand), 3);
      insert(inputBlock, *(ImageIn+d+2+accGrand), 4);
      insert(inputBlock, *(ImageIn+d+accGrand+size_x), 5);
      insert(inputBlock, *(ImageIn+d+1+accGrand+size_x), 6);
      insert(inputBlock, *(ImageIn+d+2+accGrand+size_x), 7);
      thresholdValues[accPetit+2] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
      accGrand += size_x;
      accPetit += KERNEL_SIZE;
    }
    horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
      -thresholdValues[6] + thresholdValues[8];
    vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
      + thresholdValues[7] + thresholdValues[8];
    *(ImageOut+size_x+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
  }

  //Coin en haut à droite
  dec(thresholdValues);
  thresholdValues[2] = (*(ImageIn+size_x-1) > 100) ? *(ImageIn+size_x-1) : 0;
  *(ImageOut+size_x-1) = thresholdValues[2];
  thresholdValues[5] = (*(ImageIn+size_x+size_x-1) > 100) ? *(ImageIn+size_x+size_x-1) : 0;
  thresholdValues[7] = (*(ImageIn+size_x+size_x+size_x-1) > 100) ? *(ImageIn+size_x+size_x+size_x-1) : 0;
  horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
    -thresholdValues[6] + thresholdValues[8];
  vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
    + thresholdValues[7] + thresholdValues[8];
  *(ImageOut+size_x+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

  //Au milieu
  accGrand = 2*size_x;
  for (c = 2; c < size_y - 2; c++) {
    thresholdValues[0] = (*(ImageIn+accGrand-size_x) > 100) ? *(ImageIn+accGrand-size_x) : 0;
    *(ImageOut+accGrand-size_x) = thresholdValues[0];
    thresholdValues[3] = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
    thresholdValues[6] = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;
    accPetit = 0;
    accGrand -= size_x;
    for (k = 0; k < 3; k++) {
      for (l = 1; l < 3; l++) {
        inputBlock[0] = *(ImageIn+l-1+accGrand-size_x);
        insert(inputBlock, *(ImageIn+l+accGrand-size_x), 0);
        insert(inputBlock, *(ImageIn+l+1+accGrand-size_x), 1);
        insert(inputBlock, *(ImageIn+l-1+accGrand), 2);
        insert(inputBlock, *(ImageIn+l+accGrand), 3);
        insert(inputBlock, *(ImageIn+l+1+accGrand), 4);
        insert(inputBlock, *(ImageIn+l-1+accGrand+size_x), 5);
        insert(inputBlock, *(ImageIn+l+accGrand+size_x), 6);
        insert(inputBlock, *(ImageIn+l+1+accGrand+size_x), 7);
        thresholdValues[l+accPetit] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
      }
      accPetit += KERNEL_SIZE;
      accGrand += size_x;
    }
    accGrand = accGrand - size_x - size_x;
    horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
      -thresholdValues[6] + thresholdValues[8];
    vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
      + thresholdValues[7] + thresholdValues[8];
    *(ImageOut+accGrand+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

    for (d = 2; d < size_x - 2; d++) {
      dec(thresholdValues);
      accPetit = 0;
      accGrand -= size_x;
      for (k = 0; k < 3; k++) {
        inputBlock[0] = *(ImageIn+d+accGrand-size_x);
        insert(inputBlock, *(ImageIn+d+1+accGrand-size_x), 0);
        insert(inputBlock, *(ImageIn+d+2+accGrand-size_x), 1);
        insert(inputBlock, *(ImageIn+d+accGrand), 2);
        insert(inputBlock, *(ImageIn+d+1+accGrand), 3);
        insert(inputBlock, *(ImageIn+d+2+accGrand), 4);
        insert(inputBlock, *(ImageIn+d+accGrand+size_x), 5);
        insert(inputBlock, *(ImageIn+d+1+accGrand+size_x), 6);
        insert(inputBlock, *(ImageIn+d+2+accGrand+size_x), 7);
        thresholdValues[accPetit+2] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
        accGrand += size_x;
        accPetit += KERNEL_SIZE;
      }
      accGrand = accGrand - size_x - size_x;
      horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
        -thresholdValues[6] + thresholdValues[8];
      vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
        + thresholdValues[7] + thresholdValues[8];
      *(ImageOut+accGrand+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
    }

    dec(thresholdValues);
    thresholdValues[2] = (*(ImageIn+accGrand-1) > 100) ? *(ImageIn+accGrand-1) : 0;
    *(ImageOut+accGrand-1) = thresholdValues[2];
    thresholdValues[5] = (*(ImageIn+accGrand+size_x-1) > 100) ? *(ImageIn+accGrand+size_x-1) : 0;
    thresholdValues[7] = (*(ImageIn+accGrand+size_x+size_x-1) > 100) ? *(ImageIn+accGrand+size_x+size_x-1) : 0;
    horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
      -thresholdValues[6] + thresholdValues[8];
    vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
      + thresholdValues[7] + thresholdValues[8];
    *(ImageOut+accGrand+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

    accGrand += size_x;
  }

  //Coin en bas à gauche
  accGrand = (size_y - 2) * size_x;
  accPetit = 0;
  thresholdValues[0] = (*(ImageIn+accGrand-size_x) > 100) ? *(ImageIn+accGrand-size_x) : 0;
  *(ImageOut+accGrand-size_x) = thresholdValues[0];
  thresholdValues[3] = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
  *(ImageOut+accGrand) = thresholdValues[3];
  thresholdValues[6] = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;
  *(ImageOut+accGrand+size_x) = thresholdValues[6];
  thresholdValues[7] = (*(ImageIn+accGrand+size_x+1) > 100) ? *(ImageIn+accGrand+size_x+1) : 0;
  *(ImageOut+accGrand+size_x+1) = thresholdValues[7];
  thresholdValues[8] = (*(ImageIn+accGrand+size_x+2) > 100) ? *(ImageIn+accGrand+size_x+2) : 0;
  *(ImageOut+accGrand+size_x+2) = thresholdValues[8];
  accGrand -= size_x;
  for (k = 0; k < 2; k++) {
    for (l = 1; l < 3; l++) {
      inputBlock[0] = *(ImageIn+l-1+accGrand-size_x);
      insert(inputBlock, *(ImageIn+l+accGrand-size_x), 0);
      insert(inputBlock, *(ImageIn+l+1+accGrand-size_x), 1);
      insert(inputBlock, *(ImageIn+l-1+accGrand), 2);
      insert(inputBlock, *(ImageIn+l+accGrand), 3);
      insert(inputBlock, *(ImageIn+l+1+accGrand), 4);
      insert(inputBlock, *(ImageIn+l-1+accGrand+size_x), 5);
      insert(inputBlock, *(ImageIn+l+accGrand+size_x), 6);
      insert(inputBlock, *(ImageIn+l+1+accGrand+size_x), 7);
      thresholdValues[l+accPetit] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
    }
    accGrand += size_x;
    accPetit += KERNEL_SIZE;
  }
  horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
    -thresholdValues[6] + thresholdValues[8];
  vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
    + thresholdValues[7] + thresholdValues[8];
  *(ImageOut+(size_y-2)*size_x+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

  //Dernière ligne
  for (d = 2; d < size_x - 2; d++) {
    dec(thresholdValues);
    accGrand = (size_y-2)*size_x;
    accPetit = 0;
    thresholdValues[8] = (*(ImageIn+accGrand+size_x+d+1) > 100) ? *(ImageIn+accGrand+size_x+d+1) : 0;
    *(ImageOut+accGrand+size_x+d+1) = thresholdValues[8];
    accGrand -= size_x;
    for (k = 0; k < 2; k++) {
      inputBlock[0] = *(ImageIn+d+accGrand-size_x);
      insert(inputBlock, *(ImageIn+d+1+accGrand-size_x), 0);
      insert(inputBlock, *(ImageIn+d+2+accGrand-size_x), 1);
      insert(inputBlock, *(ImageIn+d+accGrand), 2);
      insert(inputBlock, *(ImageIn+d+1+accGrand), 3);
      insert(inputBlock, *(ImageIn+d+2+accGrand), 4);
      insert(inputBlock, *(ImageIn+d+accGrand+size_x), 5);
      insert(inputBlock, *(ImageIn+d+1+accGrand+size_x), 6);
      insert(inputBlock, *(ImageIn+d+2+accGrand+size_x), 7);
      thresholdValues[accPetit+2] = (inputBlock[4] > 100) ? inputBlock[4] : 0;
      accGrand += size_x;
      accPetit += KERNEL_SIZE;
    }
    horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
      -thresholdValues[6] + thresholdValues[8];
    vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
      + thresholdValues[7] + thresholdValues[8];
    *(ImageOut+(size_y-2)*size_x+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
  }

   //Coin en bas à droite
   dec(thresholdValues);
   accGrand = (size_y-2)*size_x;
   thresholdValues[2] = (*(ImageIn+accGrand-1) > 100) ? *(ImageIn+accGrand-1) : 0;
   *(ImageOut+accGrand-1) = thresholdValues[2];
   thresholdValues[5] = (*(ImageIn+accGrand+size_x-1) > 100) ? *(ImageIn+accGrand+size_x-1) : 0;
   *(ImageOut+accGrand+size_x-1) = thresholdValues[5];
   thresholdValues[7] = (*(ImageIn+accGrand+size_x+size_x-1) > 100) ? *(ImageIn+accGrand+size_x+size_x-1) : 0;
   *(ImageOut+accGrand+size_x+size_x-1) = thresholdValues[7];
   horizontal = -thresholdValues[0] + thresholdValues[2] - thresholdValues[3] - thresholdValues[3] + thresholdValues[5] + thresholdValues[5]
     -thresholdValues[6] + thresholdValues[8];
   vertical = -thresholdValues[0] - thresholdValues[1] - thresholdValues[1] - thresholdValues[2] + thresholdValues[6] + thresholdValues[7]
     + thresholdValues[7] + thresholdValues[8];
   *(ImageOut+accGrand+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
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
