#ifndef STIMULUS_H
#define STIMULUS_H

#include "systemc.h"

SC_MODULE(stimulus) 
{
protected :
  unsigned char * ImageRes;

public :
  sc_in <bool> data_in_ready;
  sc_out<bool> data_out_ready;	
  sc_out<bool> oen;
  sc_out<bool> wen;
  sc_out< int > addr;
  sc_inout< unsigned char > data;
  sc_in_clk clock;

  enum State {INIT, SEND, READY, WAIT, READ, END};
  State state;
  State next_state;

  void do_process(); 
	
  stimulus(sc_module_name name);
  SC_HAS_PROCESS(stimulus);

  ~stimulus();
};

#endif

