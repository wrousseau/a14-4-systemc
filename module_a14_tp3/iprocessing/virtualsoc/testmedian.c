#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define IMAGE_X 10
#define IMAGE_Y 10
#define KERNEL_SIZE 3


void swap(unsigned char array[], int i, int j)
{
    unsigned char temp = array[j];
    array[j] = array[i];
    array[i] = temp;
}

void quickSort(unsigned char array[], int begin, int end)
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

unsigned char computeMedian(unsigned char array[])
{
    quickSort(array, 0, 8);
    return array[4];
}


void medianFilter (unsigned char * ImageIn, unsigned char *ImageOut, unsigned int size_x, unsigned int size_y)
{
    //Local variables
    unsigned int c,d;
    int half_kernel_size = (KERNEL_SIZE - 1) / 2;

    unsigned char inputBlock[KERNEL_SIZE*KERNEL_SIZE];

    //Leave the first line unchanged
    for ( d=0; d<size_x; d++ )
        *(ImageOut+d) = *(ImageIn+d);

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

    printf("\n");
}

int main() {
    srand(time(NULL));

    unsigned char *input;
    input = malloc(IMAGE_X*IMAGE_Y*sizeof(unsigned char));
    unsigned char *output;
    output = malloc(IMAGE_X*IMAGE_Y*sizeof(unsigned char));

    int i, j;
    for (i = 0; i < IMAGE_X*IMAGE_Y; i++)
        input[i] = rand()%20;
    for (i = 0; i < IMAGE_Y; i++)
    {
        for (j=0; j < IMAGE_X; j++)
            printf("%u ", input[IMAGE_Y*i+j]);
        printf("\n");
    }

    medianFilter(input, output, IMAGE_X, IMAGE_Y);

    for (i = 0; i < IMAGE_Y; i++)
    {
        for (j=0; j < IMAGE_X; j++)
            printf("%u ", output[IMAGE_Y*i+j]);
        printf("\n");
    }

    free(input);
    free(output);
}