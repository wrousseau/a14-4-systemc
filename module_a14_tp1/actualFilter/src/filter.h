#ifndef FILTER_H
#define FILTER_H

#include "systemc.h"

SC_MODULE(filter)
{
public:
  sc_in <bool> data_in_ready;
  sc_out<bool> data_out_ready;
  sc_in <bool> oen;
  sc_in <bool> wen;
  sc_in < int > addr;
  sc_inout< unsigned char > data;
  sc_in_clk clock;

  //variable interne
  sc_fifo<unsigned char> * mem;

  //Methods
  void do_filter();
	
  //Constructor
  filter(sc_module_name name);
  SC_HAS_PROCESS(filter);

  //Destructor
  ~filter();
};

#endif



