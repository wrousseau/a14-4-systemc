#include "systemc.h"

SC_MODULE(display) 
{
protected :
 
public :
	sc_in<bool> din;
	sc_in<bool> dout;
	sc_in_clk clock;

	void entry();
  
	display(sc_module_name name_, sc_trace_file *F):sc_module(name_)
	{
		SC_METHOD(entry);
		dont_initialize();
		sensitive << clock.pos();

		//trace
		sc_trace(F,din,"bascule_d_din");
		sc_trace(F,dout,"bascule_d_dout");
		sc_trace(F,clock,"clock");
	}
	SC_HAS_PROCESS(display);
};

