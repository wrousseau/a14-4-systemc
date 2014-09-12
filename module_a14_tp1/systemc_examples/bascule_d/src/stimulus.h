#include "systemc.h"

SC_MODULE(stimulus) 
{
protected :
	//Local variables
	bool val_din;

public :
	sc_out<bool> din;
	sc_in_clk clock;

	void process();
	
	SC_CTOR(stimulus)
	{
		SC_METHOD(process);
		dont_initialize();
		sensitive << clock.pos();
		val_din=false;
	}  
};

