#include "systemc.h"

SC_MODULE(display) 
{
protected :
 
public :
	sc_in< sc_int<8> > addr;
	sc_in<bool> oen;
	sc_in<bool> wen;
	sc_inout< sc_lv<16> > data;
	sc_in_clk clock;

	void entry();
  
	display(sc_module_name name_, sc_trace_file *F):sc_module(name_)
	{
		SC_METHOD(entry);
		dont_initialize();
		sensitive << clock.pos();

		//trace
		sc_trace(F,addr,"addr");
		sc_trace(F,oen,"oen");
		sc_trace(F,wen,"wen");
		sc_trace(F,data,"data");
		sc_trace(F,clock,"clock");
	}
	SC_HAS_PROCESS(display);
};

