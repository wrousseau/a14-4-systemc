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
#include <string.h> /* memset */
#include <unistd.h> /* close */

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

// histogram median
void
inline
histMedian (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    unsigned int c,d;
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;

    unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

    unsigned char histogram[256] = {0};

    int limit = KERNEL_SIZE*KERNEL_SIZE/2;

    unsigned char i = 0, sum = 0;

    for ( c=half_kernel_size; c<(size_y-half_kernel_size); c++ )
    {
        memset(histogram,0,256*sizeof(unsigned char));
        histogram[*(ImageIn+(c-1)*size_x+half_kernel_size-1)]++;
        histogram[*(ImageIn+c*size_x+half_kernel_size-1)]++;
        histogram[*(ImageIn+(c+1)*size_x+half_kernel_size-1)]++;
        histogram[*(ImageIn+(c-1)*size_x+half_kernel_size)]++;
        histogram[*(ImageIn+c*size_x+half_kernel_size)]++;
        histogram[*(ImageIn+(c+1)*size_x+half_kernel_size)]++;
        histogram[*(ImageIn+(c-1)*size_x+half_kernel_size+1)]++;
        histogram[*(ImageIn+c*size_x+half_kernel_size+1)]++;
        histogram[*(ImageIn+(c+1)*size_x+half_kernel_size+1)]++;

        i = 0;
        sum = 0;

        while (sum < limit)
        {
            sum += histogram[i];
            i++;
        } 

        *(ImageOut+c*size_x+half_kernel_size) = i;

        for ( d=half_kernel_size+1; d<(size_x-half_kernel_size); d++ )
        {
            histogram[*(ImageIn+(c-1)*size_x+d-2)]--;
            histogram[*(ImageIn+c*size_x+d-2)]--;
            histogram[*(ImageIn+(c+1)*size_x+d-2)]--;
            histogram[*(ImageIn+(c-1)*size_x+d+1)]++;
            histogram[*(ImageIn+c*size_x+d+1)]++;
            histogram[*(ImageIn+(c+1)*size_x+d+1)]++;

            i = 0;
            sum = 0;
            while (sum < limit)
            {
                sum += histogram[i];
                i++;
            } 

            *(ImageOut+c*size_x+d) = i;
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
        for ( d=half_kernel_size; d<(size_x-half_kernel_size); d++ )
        {
            topLeft = *(ImageIn+(c-1)*size_x+d-1);
            top = *(ImageIn+(c-1)*size_x+d);
            topRight = *(ImageIn+(c-1)*size_x+d+1);
            left = *(ImageIn+c*size_x+d-1);
            right = *(ImageIn+c*size_x+d+1);
            bottomLeft = *(ImageIn+(c+1)*size_x+d-1);
            bottom = *(ImageIn+(c+1)*size_x+d);
            bottomRight = *(ImageIn+(c+1)*size_x+d+1);
            int horizontal = -topLeft-2*left-bottomLeft+topRight+2*right+bottomRight;
            int vertical = -topLeft-2*top-topRight+bottomLeft+2*bottom+bottomRight;
            int temp = abs(horizontal)+abs(vertical);
            unsigned char result = (temp > 255) ? 255 : temp;
            *(ImageOut+c*size_x+d) = result;
        }
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

void inline insert1 (unsigned char x, unsigned char i0, unsigned char i1) {
  if (x > i0)
    i1 = x;
  else {
    i1 = i0;
    i0 = x;
  }
}

void inline insert2 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2) {
   if (x > i1)
     i2 = x;
   else {
     i2 = i1;
     if (x > i0)
       i1 = x;
     else {
       i1 = i0;
       i0 = x;
     }
   }
}

void inline insert3 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3) {
  if (x > i2)
    i3 = x;
  else {
    i3 = i2;
    if (x > i1)
      i2 = x;
    else {
      i2 = i1;
      if (x > i0)
        i1 = x;
      else {
        i1 = i0;
        i0 = x;
      }
    }
  }
}

void inline insert4 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4) {
  if (x > i3)
    i4 = x;
  else {
    i4 = i3;
    if (x > i2)
      i3 = x;
    else {
      i3 = i2;
      if (x > i1)
        i2 = x;
      else {
        i2 = i1;
        if (x > i0)
          i1 = x;
        else {
          i1 = i0;
          i0 = x;
        }
      }
    }
  }
}

void inline insert5 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4, unsigned char i5) {
  if (x > i4)
    i5 = x;
  else {
    i5 = i4;
    if (x > i3)
      i4 = x;
    else {
      i4 = i3;
      if (x > i2)
        i3 = x;
      else {
        i3 = i2;
        if (x > i1)
          i2 = x;
        else {
          i2 = i1;
          if (x > i0)
            i1 = x;
          else {
            i1 = i0;
            i0 = x;
          }
        }
      }
    }
  }
}

void inline insert6 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4, unsigned char i5,
                     unsigned char i6) {
  if (x > i5)
    i6 = x;
  else {
    i6 = i5;
    if (x > i4)
      i5 = x;
    else {
      i5 = i4;
      if (x > i3)
        i4 = x;
      else {
        i4 = i3;
        if (x > i2)
          i3 = x;
        else {
          i3 = i2;
          if (x > i1)
            i2 = x;
          else {
            i2 = i1;
            if (x > i0)
              i1 = x;
            else {
              i1 = i0;
              i0 = x;
            }
          }
        }
      }
    }
  }
}

void inline insert7 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4, unsigned char i5,
                     unsigned char i6, unsigned char i7) {
  if (x > i6)
    i7 = x;
  else {
    i7 = i6;
    if (x > i5)
      i6 = x;
    else {
      i6 = i5;
      if (x > i4)
        i5 = x;
      else {
        i5 = i4;
        if (x > i3)
          i4 = x;
        else {
          i4 = i3;
          if (x > i2)
            i3 = x;
          else {
            i3 = i2;
            if (x > i1)
              i2 = x;
            else {
              i2 = i1;
              if (x > i0)
                i1 = x;
              else {
                i1 = i0;
                i0 = x;
              }
            }
          }
        }
      }
    }
  }
}

void inline insert8 (unsigned char x, unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4, unsigned char i5,
                     unsigned char i6, unsigned char i7, unsigned char i8) {
  if (x > i7)
    i8 = x;
  else {
    i8 = i7;
    if (x > i6)
      i7 = x;
    else {
      i7 = i6;
      if (x > i5)
        i6 = x;
      else {
        i6 = i5;
        if (x > i4)
          i5 = x;
        else {
          i5 = i4;
          if (x > i3)
            i4 = x;
          else {
            i4 = i3;
            if (x > i2)
              i3 = x;
            else {
              i3 = i2;
              if (x > i1)
                i2 = x;
              else {
                i2 = i1;
                if (x > i0)
                  i1 = x;
                else {
                  i1 = i0;
                  i0 = x;
                }
              }
            }
          }
        }
      }
    }
  }
}

void inline decr(unsigned char t0, unsigned char t1, unsigned char t2, unsigned char t3, unsigned char t4, unsigned char t5, unsigned char t6,
                 unsigned char t7, unsigned char t8) {
  t0 = t1;
  t1 = t2;
  t3 = t4;
  t4 = t5;
  t6 = t7;
  t7 = t8;
}

void allInTheSame (unsigned char *ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y) {
  register unsigned char i0, i1, i2, i3, i4, i5, i6, i7, i8; //inputBlock[KERNEL_SIZE*KERNEL_SIZE]
  register unsigned char t0, t1, t2, t3, t4, t5, t6, t7, t8; //thresholdValues[KERNEL_SIZE*KERNEL_SIZE]
  register short l;
  register int horizontal, vertical;
  register unsigned int c, d, accGrand;

  //Coin en haut à gauche
  accGrand = size_x;
  t0 = (*(ImageIn) > 100) ? *(ImageIn) : 0;
  *(ImageOut) = t0;
  t1 = (*(ImageIn+1) > 100) ? *(ImageIn+1) : 0;
  *(ImageOut+1) = t1;
  t2 = (*(ImageIn+2) > 100) ? *(ImageIn+2) : 0;
  *(ImageOut+2) = t2;
  t3 = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
  t6 = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;

  //k=1, l=1
  l=1;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t4 = (i4 > 100) ? i4 : 0;

  //k=1, l=2
  l=2;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t5 = (i4 > 100) ? i4 : 0;

  accGrand += size_x;

  //k=2, l=1
  l=1;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t7 = (i4 > 100) ? i4 : 0;

  //k=2, l=2
  l=2;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t8 = (i4 > 100) ? i4 : 0;

  accGrand += size_x;

  horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
  vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
  *(ImageOut+size_x+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;


  //Première ligne
  for (d = 2; d < size_x - 2; d++) {
    decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
    accGrand = size_x;
    t2 = (*(ImageIn+d+1) > 100) ? *(ImageIn+d+1) : 0;
    *(ImageOut+d+1) = t2;

    //k=1
    i0 = *(ImageIn+d+accGrand-size_x);
    insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t5 = (i4 > 100) ? i4 : 0;
    accGrand += size_x;

    //k=2
    i0 = *(ImageIn+d+accGrand-size_x);
    insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t8 = (i4 > 100) ? i4 : 0;
    accGrand += size_x;

    horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
    vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
    *(ImageOut+size_x+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
  }


  //Coin en haut à droite
  decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
  t2 = (*(ImageIn+size_x-1) > 100) ? *(ImageIn+size_x-1) : 0;
  *(ImageOut+size_x-1) = t2;
  t5 = (*(ImageIn+size_x+size_x-1) > 100) ? *(ImageIn+size_x+size_x-1) : 0;
  t7 = (*(ImageIn+size_x+size_x+size_x-1) > 100) ? *(ImageIn+size_x+size_x+size_x-1) : 0;
  horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
  vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
  *(ImageOut+size_x+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;


  //Au milieu
  accGrand = 2*size_x;
  for (c = 2; c < size_y - 2; c++) {
    t0 = (*(ImageIn+accGrand-size_x) > 100) ? *(ImageIn+accGrand-size_x) : 0;
    *(ImageOut+accGrand-size_x) = t0;
    t3 = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
    t6 = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;
    accGrand -= size_x;

    //k=0, l=1
    l=1;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t1 = (i4 > 100) ? i4 : 0;

    //k=0, l=2
    l=2;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t2 = (i4 > 100) ? i4 : 0;

    accGrand += size_x;

    //k=1, l=1
    l=1;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t4 = (i4 > 100) ? i4 : 0;

    //k=1, l=2
    l=2;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t5 = (i4 > 100) ? i4 : 0;

    accGrand += size_x;

    //k=2, l=1
    l=1;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t7 = (i4 > 100) ? i4 : 0;

    //k=2, l=2
    l=2;
    i0 = *(ImageIn+l-1+accGrand-size_x);
    insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t8 = (i4 > 100) ? i4 : 0;

    accGrand = accGrand - size_x;
    horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
    vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
    *(ImageOut+accGrand+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

    for (d = 2; d < size_x - 2; d++) {
      //decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
      t0 = t1;
      t1 = t2;
      t3 = t4;
      t4 = t5;
      t6 = t7;
      t7 = t8;
      accGrand -= size_x;

      //k=0
      i0 = *(ImageIn+d+accGrand-size_x);
      /*insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
      insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
      insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
      insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
      insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
      insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
      insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
      insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);*/
      if (*(ImageIn+d+1+accGrand-size_x) > i0)
        i1 = *(ImageIn+d+1+accGrand-size_x);
      else {
        i1 = i0;
        i0 = *(ImageIn+d+1+accGrand-size_x);
      }
      if (*(ImageIn+d+2+accGrand-size_x) > i1)
        i2 = *(ImageIn+d+2+accGrand-size_x);
      else {
        i2 = i1;
        if (*(ImageIn+d+2+accGrand-size_x) > i0)
          i1 = *(ImageIn+d+2+accGrand-size_x);
        else {
          i1 = i0;
          i0 = *(ImageIn+d+2+accGrand-size_x);
        }
      }
      if (*(ImageIn+d+accGrand) > i2)
        i3 = *(ImageIn+d+accGrand);
      else {
        i3 = i2;
        if (*(ImageIn+d+accGrand) > i1)
          i2 = *(ImageIn+d+accGrand);
        else {
          i2 = i1;
          if (*(ImageIn+d+accGrand) > i0)
            i1 = *(ImageIn+d+accGrand);
          else {
            i1 = i0;
            i0 = *(ImageIn+d+accGrand);
          }
        }
      }
      if (*(ImageIn+d+1+accGrand) > i3)
        i4 = *(ImageIn+d+1+accGrand);
      else {
        i4 = i3;
        if (*(ImageIn+d+1+accGrand) > i2)
          i3 = *(ImageIn+d+1+accGrand);
        else {
          i3 = i2;
          if (*(ImageIn+d+1+accGrand) > i1)
            i2 = *(ImageIn+d+1+accGrand);
          else {
            i2 = i1;
            if (*(ImageIn+d+1+accGrand) > i0)
              i1 = *(ImageIn+d+1+accGrand);
            else {
              i1 = i0;
              i0 = *(ImageIn+d+1+accGrand);
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand) > i4)
        i5 = *(ImageIn+d+2+accGrand);
      else {
        i5 = i4;
        if (*(ImageIn+d+2+accGrand) > i3)
          i4 = *(ImageIn+d+2+accGrand);
        else {
          i4 = i3;
          if (*(ImageIn+d+2+accGrand) > i2)
            i3 = *(ImageIn+d+2+accGrand);
          else {
            i3 = i2;
            if (*(ImageIn+d+2+accGrand) > i1)
              i2 = *(ImageIn+d+2+accGrand);
            else {
              i2 = i1;
              if (*(ImageIn+d+2+accGrand) > i0)
                i1 = *(ImageIn+d+2+accGrand);
              else {
                i1 = i0;
                i0 = *(ImageIn+d+2+accGrand);
              }
            }
          }
        }
      }
      if (*(ImageIn+d+accGrand+size_x) > i5)
        i6 = *(ImageIn+d+accGrand+size_x);
      else {
        i6 = i5;
        if (*(ImageIn+d+accGrand+size_x) > i4)
          i5 = *(ImageIn+d+accGrand+size_x);
        else {
          i5 = i4;
          if (*(ImageIn+d+accGrand+size_x) > i3)
            i4 = *(ImageIn+d+accGrand+size_x);
          else {
            i4 = i3;
            if (*(ImageIn+d+accGrand+size_x) > i2)
              i3 = *(ImageIn+d+accGrand+size_x);
            else {
              i3 = i2;
              if (*(ImageIn+d+accGrand+size_x) > i1)
                i2 = *(ImageIn+d+accGrand+size_x);
              else {
                i2 = i1;
                if (*(ImageIn+d+accGrand+size_x) > i0)
                  i1 = *(ImageIn+d+accGrand+size_x);
                else {
                  i1 = i0;
                  i0 = *(ImageIn+d+accGrand+size_x);
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+1+accGrand+size_x) > i6)
        i7 = *(ImageIn+d+1+accGrand+size_x);
      else {
        i7 = i6;
        if (*(ImageIn+d+1+accGrand+size_x) > i5)
          i6 = *(ImageIn+d+1+accGrand+size_x);
        else {
          i6 = i5;
          if (*(ImageIn+d+1+accGrand+size_x) > i4)
            i5 = *(ImageIn+d+1+accGrand+size_x);
          else {
            i5 = i4;
            if (*(ImageIn+d+1+accGrand+size_x) > i3)
              i4 = *(ImageIn+d+1+accGrand+size_x);
            else {
              i4 = i3;
              if (*(ImageIn+d+1+accGrand+size_x) > i2)
                i3 = *(ImageIn+d+1+accGrand+size_x);
              else {
                i3 = i2;
                if (*(ImageIn+d+1+accGrand+size_x) > i1)
                  i2 = *(ImageIn+d+1+accGrand+size_x);
                else {
                  i2 = i1;
                  if (*(ImageIn+d+1+accGrand+size_x) > i0)
                    i1 = *(ImageIn+d+1+accGrand+size_x);
                  else {
                    i1 = i0;
                    i0 = *(ImageIn+d+1+accGrand+size_x);
                  }
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand+size_x) > i7)
        i8 = *(ImageIn+d+2+accGrand+size_x);
      else {
        i8 = i7;
        if (*(ImageIn+d+2+accGrand+size_x) > i6)
          i7 = *(ImageIn+d+2+accGrand+size_x);
        else {
          i7 = i6;
          if (*(ImageIn+d+2+accGrand+size_x) > i5)
            i6 = *(ImageIn+d+2+accGrand+size_x);
          else {
            i6 = i5;
            if (*(ImageIn+d+2+accGrand+size_x) > i4)
              i5 = *(ImageIn+d+2+accGrand+size_x);
            else {
              i5 = i4;
              if (*(ImageIn+d+2+accGrand+size_x) > i3)
                i4 = *(ImageIn+d+2+accGrand+size_x);
              else {
                i4 = i3;
                if (*(ImageIn+d+2+accGrand+size_x) > i2)
                  i3 = *(ImageIn+d+2+accGrand+size_x);
                else {
                  i3 = i2;
                  if (*(ImageIn+d+2+accGrand+size_x) > i1)
                    i2 = *(ImageIn+d+2+accGrand+size_x);
                  else {
                    i2 = i1;
                    if (*(ImageIn+d+2+accGrand+size_x) > i0)
                      i1 = *(ImageIn+d+2+accGrand+size_x);
                    else {
                      i1 = i0;
                      i0 = *(ImageIn+d+2+accGrand+size_x);
                    }
                  }
                }
              }
            }
          }
        }
      }
      t2 = (i4 > 100) ? i4 : 0;
      accGrand += size_x;

      //k=1
      i0 = *(ImageIn+d+accGrand-size_x);
      /*insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
      insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
      insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
      insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
      insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
      insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
      insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
      insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);*/
      if (*(ImageIn+d+1+accGrand-size_x) > i0)
        i1 = *(ImageIn+d+1+accGrand-size_x);
      else {
        i1 = i0;
        i0 = *(ImageIn+d+1+accGrand-size_x);
      }
      if (*(ImageIn+d+2+accGrand-size_x) > i1)
        i2 = *(ImageIn+d+2+accGrand-size_x);
      else {
        i2 = i1;
        if (*(ImageIn+d+2+accGrand-size_x) > i0)
          i1 = *(ImageIn+d+2+accGrand-size_x);
        else {
          i1 = i0;
          i0 = *(ImageIn+d+2+accGrand-size_x);
        }
      }
      if (*(ImageIn+d+accGrand) > i2)
        i3 = *(ImageIn+d+accGrand);
      else {
        i3 = i2;
        if (*(ImageIn+d+accGrand) > i1)
          i2 = *(ImageIn+d+accGrand);
        else {
          i2 = i1;
          if (*(ImageIn+d+accGrand) > i0)
            i1 = *(ImageIn+d+accGrand);
          else {
            i1 = i0;
            i0 = *(ImageIn+d+accGrand);
          }
        }
      }
      if (*(ImageIn+d+1+accGrand) > i3)
        i4 = *(ImageIn+d+1+accGrand);
      else {
        i4 = i3;
        if (*(ImageIn+d+1+accGrand) > i2)
          i3 = *(ImageIn+d+1+accGrand);
        else {
          i3 = i2;
          if (*(ImageIn+d+1+accGrand) > i1)
            i2 = *(ImageIn+d+1+accGrand);
          else {
            i2 = i1;
            if (*(ImageIn+d+1+accGrand) > i0)
              i1 = *(ImageIn+d+1+accGrand);
            else {
              i1 = i0;
              i0 = *(ImageIn+d+1+accGrand);
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand) > i4)
        i5 = *(ImageIn+d+2+accGrand);
      else {
        i5 = i4;
        if (*(ImageIn+d+2+accGrand) > i3)
          i4 = *(ImageIn+d+2+accGrand);
        else {
          i4 = i3;
          if (*(ImageIn+d+2+accGrand) > i2)
            i3 = *(ImageIn+d+2+accGrand);
          else {
            i3 = i2;
            if (*(ImageIn+d+2+accGrand) > i1)
              i2 = *(ImageIn+d+2+accGrand);
            else {
              i2 = i1;
              if (*(ImageIn+d+2+accGrand) > i0)
                i1 = *(ImageIn+d+2+accGrand);
              else {
                i1 = i0;
                i0 = *(ImageIn+d+2+accGrand);
              }
            }
          }
        }
      }
      if (*(ImageIn+d+accGrand+size_x) > i5)
        i6 = *(ImageIn+d+accGrand+size_x);
      else {
        i6 = i5;
        if (*(ImageIn+d+accGrand+size_x) > i4)
          i5 = *(ImageIn+d+accGrand+size_x);
        else {
          i5 = i4;
          if (*(ImageIn+d+accGrand+size_x) > i3)
            i4 = *(ImageIn+d+accGrand+size_x);
          else {
            i4 = i3;
            if (*(ImageIn+d+accGrand+size_x) > i2)
              i3 = *(ImageIn+d+accGrand+size_x);
            else {
              i3 = i2;
              if (*(ImageIn+d+accGrand+size_x) > i1)
                i2 = *(ImageIn+d+accGrand+size_x);
              else {
                i2 = i1;
                if (*(ImageIn+d+accGrand+size_x) > i0)
                  i1 = *(ImageIn+d+accGrand+size_x);
                else {
                  i1 = i0;
                  i0 = *(ImageIn+d+accGrand+size_x);
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+1+accGrand+size_x) > i6)
        i7 = *(ImageIn+d+1+accGrand+size_x);
      else {
        i7 = i6;
        if (*(ImageIn+d+1+accGrand+size_x) > i5)
          i6 = *(ImageIn+d+1+accGrand+size_x);
        else {
          i6 = i5;
          if (*(ImageIn+d+1+accGrand+size_x) > i4)
            i5 = *(ImageIn+d+1+accGrand+size_x);
          else {
            i5 = i4;
            if (*(ImageIn+d+1+accGrand+size_x) > i3)
              i4 = *(ImageIn+d+1+accGrand+size_x);
            else {
              i4 = i3;
              if (*(ImageIn+d+1+accGrand+size_x) > i2)
                i3 = *(ImageIn+d+1+accGrand+size_x);
              else {
                i3 = i2;
                if (*(ImageIn+d+1+accGrand+size_x) > i1)
                  i2 = *(ImageIn+d+1+accGrand+size_x);
                else {
                  i2 = i1;
                  if (*(ImageIn+d+1+accGrand+size_x) > i0)
                    i1 = *(ImageIn+d+1+accGrand+size_x);
                  else {
                    i1 = i0;
                    i0 = *(ImageIn+d+1+accGrand+size_x);
                  }
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand+size_x) > i7)
        i8 = *(ImageIn+d+2+accGrand+size_x);
      else {
        i8 = i7;
        if (*(ImageIn+d+2+accGrand+size_x) > i6)
          i7 = *(ImageIn+d+2+accGrand+size_x);
        else {
          i7 = i6;
          if (*(ImageIn+d+2+accGrand+size_x) > i5)
            i6 = *(ImageIn+d+2+accGrand+size_x);
          else {
            i6 = i5;
            if (*(ImageIn+d+2+accGrand+size_x) > i4)
              i5 = *(ImageIn+d+2+accGrand+size_x);
            else {
              i5 = i4;
              if (*(ImageIn+d+2+accGrand+size_x) > i3)
                i4 = *(ImageIn+d+2+accGrand+size_x);
              else {
                i4 = i3;
                if (*(ImageIn+d+2+accGrand+size_x) > i2)
                  i3 = *(ImageIn+d+2+accGrand+size_x);
                else {
                  i3 = i2;
                  if (*(ImageIn+d+2+accGrand+size_x) > i1)
                    i2 = *(ImageIn+d+2+accGrand+size_x);
                  else {
                    i2 = i1;
                    if (*(ImageIn+d+2+accGrand+size_x) > i0)
                      i1 = *(ImageIn+d+2+accGrand+size_x);
                    else {
                      i1 = i0;
                      i0 = *(ImageIn+d+2+accGrand+size_x);
                    }
                  }
                }
              }
            }
          }
        }
      }
      t5 = (i4 > 100) ? i4 : 0;
      accGrand += size_x;

      //k=2
      i0 = *(ImageIn+d+accGrand-size_x);
      /*insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
      insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
      insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
      insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
      insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
      insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
      insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
      insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);*/
      if (*(ImageIn+d+1+accGrand-size_x) > i0)
        i1 = *(ImageIn+d+1+accGrand-size_x);
      else {
        i1 = i0;
        i0 = *(ImageIn+d+1+accGrand-size_x);
      }
      if (*(ImageIn+d+2+accGrand-size_x) > i1)
        i2 = *(ImageIn+d+2+accGrand-size_x);
      else {
        i2 = i1;
        if (*(ImageIn+d+2+accGrand-size_x) > i0)
          i1 = *(ImageIn+d+2+accGrand-size_x);
        else {
          i1 = i0;
          i0 = *(ImageIn+d+2+accGrand-size_x);
        }
      }
      if (*(ImageIn+d+accGrand) > i2)
        i3 = *(ImageIn+d+accGrand);
      else {
        i3 = i2;
        if (*(ImageIn+d+accGrand) > i1)
          i2 = *(ImageIn+d+accGrand);
        else {
          i2 = i1;
          if (*(ImageIn+d+accGrand) > i0)
            i1 = *(ImageIn+d+accGrand);
          else {
            i1 = i0;
            i0 = *(ImageIn+d+accGrand);
          }
        }
      }
      if (*(ImageIn+d+1+accGrand) > i3)
        i4 = *(ImageIn+d+1+accGrand);
      else {
        i4 = i3;
        if (*(ImageIn+d+1+accGrand) > i2)
          i3 = *(ImageIn+d+1+accGrand);
        else {
          i3 = i2;
          if (*(ImageIn+d+1+accGrand) > i1)
            i2 = *(ImageIn+d+1+accGrand);
          else {
            i2 = i1;
            if (*(ImageIn+d+1+accGrand) > i0)
              i1 = *(ImageIn+d+1+accGrand);
            else {
              i1 = i0;
              i0 = *(ImageIn+d+1+accGrand);
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand) > i4)
        i5 = *(ImageIn+d+2+accGrand);
      else {
        i5 = i4;
        if (*(ImageIn+d+2+accGrand) > i3)
          i4 = *(ImageIn+d+2+accGrand);
        else {
          i4 = i3;
          if (*(ImageIn+d+2+accGrand) > i2)
            i3 = *(ImageIn+d+2+accGrand);
          else {
            i3 = i2;
            if (*(ImageIn+d+2+accGrand) > i1)
              i2 = *(ImageIn+d+2+accGrand);
            else {
              i2 = i1;
              if (*(ImageIn+d+2+accGrand) > i0)
                i1 = *(ImageIn+d+2+accGrand);
              else {
                i1 = i0;
                i0 = *(ImageIn+d+2+accGrand);
              }
            }
          }
        }
      }
      if (*(ImageIn+d+accGrand+size_x) > i5)
        i6 = *(ImageIn+d+accGrand+size_x);
      else {
        i6 = i5;
        if (*(ImageIn+d+accGrand+size_x) > i4)
          i5 = *(ImageIn+d+accGrand+size_x);
        else {
          i5 = i4;
          if (*(ImageIn+d+accGrand+size_x) > i3)
            i4 = *(ImageIn+d+accGrand+size_x);
          else {
            i4 = i3;
            if (*(ImageIn+d+accGrand+size_x) > i2)
              i3 = *(ImageIn+d+accGrand+size_x);
            else {
              i3 = i2;
              if (*(ImageIn+d+accGrand+size_x) > i1)
                i2 = *(ImageIn+d+accGrand+size_x);
              else {
                i2 = i1;
                if (*(ImageIn+d+accGrand+size_x) > i0)
                  i1 = *(ImageIn+d+accGrand+size_x);
                else {
                  i1 = i0;
                  i0 = *(ImageIn+d+accGrand+size_x);
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+1+accGrand+size_x) > i6)
        i7 = *(ImageIn+d+1+accGrand+size_x);
      else {
        i7 = i6;
        if (*(ImageIn+d+1+accGrand+size_x) > i5)
          i6 = *(ImageIn+d+1+accGrand+size_x);
        else {
          i6 = i5;
          if (*(ImageIn+d+1+accGrand+size_x) > i4)
            i5 = *(ImageIn+d+1+accGrand+size_x);
          else {
            i5 = i4;
            if (*(ImageIn+d+1+accGrand+size_x) > i3)
              i4 = *(ImageIn+d+1+accGrand+size_x);
            else {
              i4 = i3;
              if (*(ImageIn+d+1+accGrand+size_x) > i2)
                i3 = *(ImageIn+d+1+accGrand+size_x);
              else {
                i3 = i2;
                if (*(ImageIn+d+1+accGrand+size_x) > i1)
                  i2 = *(ImageIn+d+1+accGrand+size_x);
                else {
                  i2 = i1;
                  if (*(ImageIn+d+1+accGrand+size_x) > i0)
                    i1 = *(ImageIn+d+1+accGrand+size_x);
                  else {
                    i1 = i0;
                    i0 = *(ImageIn+d+1+accGrand+size_x);
                  }
                }
              }
            }
          }
        }
      }
      if (*(ImageIn+d+2+accGrand+size_x) > i7)
        i8 = *(ImageIn+d+2+accGrand+size_x);
      else {
        i8 = i7;
        if (*(ImageIn+d+2+accGrand+size_x) > i6)
          i7 = *(ImageIn+d+2+accGrand+size_x);
        else {
          i7 = i6;
          if (*(ImageIn+d+2+accGrand+size_x) > i5)
            i6 = *(ImageIn+d+2+accGrand+size_x);
          else {
            i6 = i5;
            if (*(ImageIn+d+2+accGrand+size_x) > i4)
              i5 = *(ImageIn+d+2+accGrand+size_x);
            else {
              i5 = i4;
              if (*(ImageIn+d+2+accGrand+size_x) > i3)
                i4 = *(ImageIn+d+2+accGrand+size_x);
              else {
                i4 = i3;
                if (*(ImageIn+d+2+accGrand+size_x) > i2)
                  i3 = *(ImageIn+d+2+accGrand+size_x);
                else {
                  i3 = i2;
                  if (*(ImageIn+d+2+accGrand+size_x) > i1)
                    i2 = *(ImageIn+d+2+accGrand+size_x);
                  else {
                    i2 = i1;
                    if (*(ImageIn+d+2+accGrand+size_x) > i0)
                      i1 = *(ImageIn+d+2+accGrand+size_x);
                    else {
                      i1 = i0;
                      i0 = *(ImageIn+d+2+accGrand+size_x);
                    }
                  }
                }
              }
            }
          }
        }
      }
      t7 = (i4 > 100) ? i4 : 0;

      accGrand = accGrand - size_x;
      horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
      vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
      *(ImageOut+accGrand+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
    }

    decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
    t2 = (*(ImageIn+accGrand-1) > 100) ? *(ImageIn+accGrand-1) : 0;
    *(ImageOut+accGrand-1) = t2;
    t5 = (*(ImageIn+accGrand+size_x-1) > 100) ? *(ImageIn+accGrand+size_x-1) : 0;
    t7 = (*(ImageIn+accGrand+size_x+size_x-1) > 100) ? *(ImageIn+accGrand+size_x+size_x-1) : 0;
    horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
    vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
    *(ImageOut+accGrand+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

    accGrand += size_x;
  }


  //Coin en bas à gauche
  accGrand = (size_y - 2) * size_x;
  t0 = (*(ImageIn+accGrand-size_x) > 100) ? *(ImageIn+accGrand-size_x) : 0;
  *(ImageOut+accGrand-size_x) = t0;
  t3 = (*(ImageIn+accGrand) > 100) ? *(ImageIn+accGrand) : 0;
  *(ImageOut+accGrand) = t3;
  t6 = (*(ImageIn+accGrand+size_x) > 100) ? *(ImageIn+accGrand+size_x) : 0;
  *(ImageOut+accGrand+size_x) = t6;
  t7 = (*(ImageIn+accGrand+size_x+1) > 100) ? *(ImageIn+accGrand+size_x+1) : 0;
  *(ImageOut+accGrand+size_x+1) = t7;
  t8 = (*(ImageIn+accGrand+size_x+2) > 100) ? *(ImageIn+accGrand+size_x+2) : 0;
  *(ImageOut+accGrand+size_x+2) = t8;
  accGrand -= size_x;

  //k=0, l=1
  l=1;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t1 = (i4 > 100) ? i4 : 0;

  //k=0, l=2
  l=2;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t2 = (i4 > 100) ? i4 : 0;

  accGrand += size_x;

  //k=1, l=1
  l=1;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t4 = (i4 > 100) ? i4 : 0;

  //k=1, l=2
  l=2;
  i0 = *(ImageIn+l-1+accGrand-size_x);
  insert1(*(ImageIn+l+accGrand-size_x), i0, i1);
  insert2(*(ImageIn+l+1+accGrand-size_x), i0, i1, i2);
  insert3(*(ImageIn+l-1+accGrand), i0, i1, i2, i3);
  insert4(*(ImageIn+l+accGrand), i0, i1, i2, i3, i4);
  insert5(*(ImageIn+l+1+accGrand), i0, i1, i2, i3, i4, i5);
  insert6(*(ImageIn+l-1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
  insert7(*(ImageIn+l+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
  insert8(*(ImageIn+l+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
  t5 = (i4 > 100) ? i4 : 0;

  horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
  vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
  *(ImageOut+(size_y-2)*size_x+1) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;

  //Dernière ligne
  for (d = 2; d < size_x - 2; d++) {
    decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
    accGrand = (size_y-2)*size_x;
    t8 = (*(ImageIn+accGrand+size_x+d+1) > 100) ? *(ImageIn+accGrand+size_x+d+1) : 0;
    *(ImageOut+accGrand+size_x+d+1) = t8;
    accGrand -= size_x;

    //k=0
    i0 = *(ImageIn+d+accGrand-size_x);
    insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t2 = (i4 > 100) ? i4 : 0;
    accGrand += size_x;

    //k=1
    i0 = *(ImageIn+d+accGrand-size_x);
    insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t5 = (i4 > 100) ? i4 : 0;
    accGrand += size_x;

    //k=2
    i0 = *(ImageIn+d+accGrand-size_x);
    insert1(*(ImageIn+d+1+accGrand-size_x), i0, i1);
    insert2(*(ImageIn+d+2+accGrand-size_x), i0, i1, i2);
    insert3(*(ImageIn+d+accGrand), i0, i1, i2, i3);
    insert4(*(ImageIn+d+1+accGrand), i0, i1, i2, i3, i4);
    insert5(*(ImageIn+d+2+accGrand), i0, i1, i2, i3, i4, i5);
    insert6(*(ImageIn+d+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6);
    insert7(*(ImageIn+d+1+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7);
    insert8(*(ImageIn+d+2+accGrand+size_x), i0, i1, i2, i3, i4, i5, i6, i7, i8);
    t7 = (i4 > 100) ? i4 : 0;

    horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
    vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
    *(ImageOut+(size_y-2)*size_x+d) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
  }

   //Coin en bas à droite
   decr(t0, t1, t2, t3, t4, t5, t6, t7, t8);
   accGrand = (size_y-2)*size_x;
   t2 = (*(ImageIn+accGrand-1) > 100) ? *(ImageIn+accGrand-1) : 0;
   *(ImageOut+accGrand-1) = t2;
   t5 = (*(ImageIn+accGrand+size_x-1) > 100) ? *(ImageIn+accGrand+size_x-1) : 0;
   *(ImageOut+accGrand+size_x-1) = t5;
   t7 = (*(ImageIn+accGrand+size_x+size_x-1) > 100) ? *(ImageIn+accGrand+size_x+size_x-1) : 0;
   *(ImageOut+accGrand+size_x+size_x-1) = t7;
   horizontal = -t0 + t2 - t3 - t3 + t5 + t5 - t6 + t8;
   vertical = -t0 - t1 - t1 - t2 + t6 + t7 + t7 + t8;
   *(ImageOut+accGrand+size_x-2) = (abs(horizontal)+abs(vertical) < 256) ? abs(horizontal)+abs(vertical) : 255;
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
