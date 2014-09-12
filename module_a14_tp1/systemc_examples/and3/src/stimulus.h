#include "systemc.h"

SC_MODULE(stimulus) 
{
protected :
	//Local variables
	bool val_e1,val_e2,val_e3;

public :
	sc_out<bool> e1;
	sc_out<bool> e2;
	sc_out<bool> e3;
	sc_in_clk clock;

	void process();
	
	SC_CTOR(stimulus)
	{
		SC_METHOD(process);
		dont_initialize();
		sensitive << clock.pos();
		val_e1=val_e2=val_e3=false;
	}  
};

