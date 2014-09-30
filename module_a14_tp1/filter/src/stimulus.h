#ifndef STIMULUS_H
#define STIMULUS_H

#include "systemc.h"

SC_MODULE(stimulus) 
{
protected :
  unsigned char * processedImage;

public :
  enum State {INIT, SEND, READY, WAIT, READ, END};

  // Send Process
  sc_in <bool> dataInReady;
  sc_out<bool> sendProcessOen;
  sc_out<bool> sendProcessWen;
  sc_out<int> sendProcessAddr;
  sc_out<unsigned char> originalData;

  State sendProcessState;
  State sendProcessNextState;

  // Read Process
  sc_out<bool> dataOutReady;  
  sc_out<bool> readProcessOen;
  sc_out<bool> readProcessWen;
  sc_out<int> readProcessAddr;
  sc_in<unsigned char> processedData;

  State readProcessState;
  State readProcessNextState;

  sc_in_clk clock;

  void sendProcess();
  void readProcess(); 
	
  stimulus(sc_module_name name);
  SC_HAS_PROCESS(stimulus);

  ~stimulus();
};

#endif

