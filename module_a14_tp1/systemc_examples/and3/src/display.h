#include "systemc.h"

SC_MODULE(display) 
{
protected :
 
public :
	sc_in<bool> and3_e1;
	sc_in<bool> and3_e2;
	sc_in<bool> and3_e3;
	sc_in<bool> and3_s;
	sc_in_clk clock;

	void entry();
  
	display(sc_module_name name_, sc_trace_file *F):sc_module(name_)
	{
		SC_METHOD(entry);
		dont_initialize();
		sensitive << clock.pos();

		//trace
		sc_trace(F,and3_e1,"and3_e1");
		sc_trace(F,and3_e2,"and3_e2");
		sc_trace(F,and3_e3,"and3_e3");
		sc_trace(F,and3_s,"and3_s");
		sc_trace(F,clock,"clock");
	}
	SC_HAS_PROCESS(display);
};

