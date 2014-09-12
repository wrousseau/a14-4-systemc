#include "systemc.h"

SC_MODULE(stimulus) 
{
protected :
	//Local variables
	sc_int<8> val_addr;

public :
	sc_out<bool> oen;
	sc_out<bool> wen;
	sc_out< sc_int<8> > addr;
	sc_inout< sc_lv<16> > data;
	sc_in_clk clock;

	void process();
	
	SC_CTOR(stimulus):oen("oen"),wen("wen"),addr("addr"),data("data")
	{
		SC_THREAD(process);
		dont_initialize();
		sensitive << clock.pos();
		val_addr=0;
	}  
};

