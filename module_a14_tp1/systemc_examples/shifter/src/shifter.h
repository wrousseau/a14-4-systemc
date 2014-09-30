#include "systemc.h"

SC_MODULE(shifter)
{
public:
	sc_in<bool> clock;

	sc_in<bool> din;
	sc_out<bool> dout;

	//variable interne
	int size;
	sc_signal<bool> *regs;

	//Methods
	void shift()
	{
		for(int i=0; i<size-1; i++)
			regs[i+1] = regs[i];
		regs[0] = din.read();
		dout.write(regs[size-1]);
	}

	//Constructor
	shifter(sc_module_name name, int size):sc_module(name), size(size)
	{
		regs = new sc_signal<bool>[size];
		SC_METHOD(shift);
		dont_initialize();
		sensitive << clock.pos();
	}
	SC_HAS_PROCESS(shifter);

	//Destructor
	~shifter() {
  		delete regs;
	}
};




