#include "systemc.h"

SC_MODULE(ram)
{
public:
	sc_in< sc_int<8> > addr;
	sc_in<bool> oen;
	sc_in<bool> wen;
	sc_inout< sc_lv<16> > data;

	//variable interne
	sc_lv<16> ram_data[256];
	sc_int<8> val;

	//Methods
	void process()
	{
		//Write
		if(oen && !wen) {
			val = addr.read();
			ram_data[val.to_int()] = data;
		}

		//Read
		else if(!oen && wen) {
			val = addr.read();
			data = ram_data[val.to_int()];
		} 
		
		else {
			data = "ZZZZZZZZZZZZZZZZ";
		}

	}
	
	//Constructor
	SC_CTOR(ram)
	{
		SC_METHOD(process);
		sensitive << addr << oen << wen;
	}
};




