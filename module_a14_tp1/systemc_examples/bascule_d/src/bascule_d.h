#include "systemc.h"

SC_MODULE(bascule_d)
{
	sc_in<bool> din;
	sc_in<bool> clock;
	sc_out<bool> dout;

	void sampling()
	{
		//dout.write(din.read());
		dout = din;
	}

	SC_CTOR(bascule_d)
	{
		SC_METHOD(sampling);
		sensitive << clock.pos();
	}
};




