#include "filter.h"

//doFilter
void 
filter::doFilter()
{
  while(1)
  {
    wait();

    //Check the address
    if(readProcessAddr==0x80000000) 
    {
      int res = centerDin.read()-verticalSumDin.read()-horizontalSumDin.read();
      if (res < 0)
        res = 0;
      else if (res > 255)
        res = 255;
      processedData.write(res);
      dataOutReady.write(true);
    }
    else 
    {
      //Not concerned
    }
  }
}


//Constructor
filter::filter(sc_module_name name):sc_module(name)
{
  //mem = new sc_fifo<unsigned char> ("mem", 600*600);

  SC_THREAD(doFilter);
  dont_initialize();
  sensitive << clock.pos();
}


//Destructor
filter::~filter() {
  //delete mem;
}



