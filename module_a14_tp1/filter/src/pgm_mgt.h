#ifndef PGM_MGT_H
#define PGM_MGT_H

#include <stdlib.h>
#include <stdio.h>

#define MAXLENGTH 256

//pgmRead
inline
unsigned char 
** pgmRead (const char *fileName, unsigned int *size_x, unsigned int *size_y)
{
  FILE *filePointer;    // for file buffer
  char line[MAXLENGTH]; // for character input from file
  unsigned int maximumValue = 0; // max value from header
  int binary;           // flag to indicate if file is binary (P5)
  unsigned int numberRead = 0;  // counter for number of pixels read
  unsigned int i,j;     // (i,j) for loops
  int test,temp;        // for detecting EOF(test) and temp storage

  // Open the file, return an error if necessary.
  if ((filePointer = fopen(fileName,"r")) == NULL) {
   printf ("ERROR: cannot open file\n\n");
   fclose (filePointer);
   exit (0);
  }
    
  // Initialize columnsidth, and height
  *size_x = *size_y =0;

  // Check the file signature ("Magic Numbers" P2 and P5); skip comments and blank lines (CR with no spaces before it).
  fgets (line,MAXLENGTH,filePointer);
  while (line[0]=='#' || line[0]=='\n') fgets (line,MAXLENGTH,filePointer);

  if (line[0]=='P' && (line[1]=='2')) {
    binary = 0;
  }
  else if (line[0]=='P' && (line[1]=='5')) {
    binary = 1;
  }
  else {
    printf ("ERROR: incorrect file format\n\n");
    fclose (filePointer);
    exit (0);
  }          

  // Input the width, height and maximum value, skip comments and blank lines.
  fgets (line,MAXLENGTH,filePointer);
  while (line[0]=='#' || line[0]=='\n') fgets (line,MAXLENGTH,filePointer);
  sscanf (line,"%d %d",size_x,size_y);

  fgets (line,MAXLENGTH,filePointer);
  while (line[0]=='#' || line[0]=='\n') fgets(line,MAXLENGTH,filePointer);
  sscanf (line,"%d",&maximumValue);

  printf("size is %d %d and maximum value is %d\n", *size_x, *size_y, maximumValue);

  // Create buffer
  unsigned char ** image = new unsigned char* [*size_y];
  for (int i=0;i<*size_y;i++) image[i] = new unsigned char [*size_x];

  // Read in the data for binary (P5) or ascii (P2) PGM formats
  if (binary) {
    for (i = 0; i < (*size_y); i++) {
      numberRead += fread((void *)&(image[i][0]), sizeof(unsigned char),(*size_x),filePointer);
      if (feof(filePointer)) break;
    }
  }
  else {
    for (i= 0; i < (*size_y); i++) {
      for (j =0; j < (*size_x); j++) {
          test = fscanf (filePointer,"%d",&temp);
          if (test == EOF) break;
          image[i][j] = (unsigned char)temp;
          numberRead++;
       }
       if (test == EOF) break;
    }
  } 
 
  if (numberRead < ((*size_x)*(*size_y))) {
    printf ("ERROR: fewer pixels than rows*cols indicates\n\n");
  }
  else printf ("pgmRead ok.\n");
     
  // close the file and return 1 indicating success
  fclose (filePointer);
  return (image);
}

//pgmWrite
inline
int 
pgmWrite(const char* filename, unsigned int size_x,unsigned int size_y, unsigned char * image)
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

