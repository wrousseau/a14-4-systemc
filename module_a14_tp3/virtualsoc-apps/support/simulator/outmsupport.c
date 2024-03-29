#include "outmsupport.h"

void outm_write_burst (unsigned char * data, unsigned int size_x, unsigned int size_y)
{
  //Local variables
  int i;
  unsigned int tmp1, tmp2, tmp3, tmp4;
  unsigned int data_c;

  //Set sizes
  *((uint32_t*)(OUTPUT_MEM_SIZEX_ADDR)) = size_x;
  *((uint32_t*)(OUTPUT_MEM_SIZEY_ADDR)) = size_y;

  //Write into memory
  for (i=0;i<(size_x*size_y);i+=4) {
    tmp1 = *(data+i);
    tmp2 = *(data+i+1);
    tmp3 = *(data+i+2);
    tmp4 = *(data+i+3);
    data_c = (tmp1 << 0) + (tmp2 << 8) + (tmp3 << 16) + (tmp4 << 24); 
    *((uint32_t*)(OUTPUT_MEM_WRITE_ADDR+i)) = data_c;
  }
}

void outm_write_file ()
{
	*((uint32_t*)(OUTPUT_MEM_WRITE_FILE_ADDR)) = true;
}


