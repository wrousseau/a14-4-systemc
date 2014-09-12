#include "systemc.h"

SC_MODULE(shifter)
{
public:
	sc_in<bool> clock;

	sc_in<bool> din;
	sc_out<bool> dout;

	//variable interne
	sc_signal<bool> regs[8];

	//Methods
	void shift()
	{
		for(int i=0; i<7; i++)
			regs[i+1] = regs[i];
		regs[0] = din.read();
		dout.write(regs[7]);
	}

	//Constructor
	shifter(sc_module_name name):sc_module(name)
	{
		SC_METHOD(shift);
		dont_initialize();
		sensitive << clock.pos();
	}
	SC_HAS_PROCESS(shifter);
};




