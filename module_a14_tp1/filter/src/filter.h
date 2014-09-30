#ifndef FILTER_H
#define FILTER_H

#include "systemc.h"
#include "input_image.h"

SC_MODULE(filter)
{
public:
  // Send Process
  sc_in<int> verticalSumDin;
  sc_in<int> horizontalSumDin;
  sc_in<int> centerDin;
  sc_in<int> readProcessAddr;
  sc_out<unsigned char> processedData;

  sc_in <bool> dataInReady;
  sc_out <bool> dataOutReady;

  sc_in_clk clock;

  sc_fifo<unsigned char> * mem;
  
  //Methods
  void doFilter();
	
  //Constructor
  filter(sc_module_name name);
  SC_HAS_PROCESS(filter);

  //Destructor
  ~filter();
};

#endif



