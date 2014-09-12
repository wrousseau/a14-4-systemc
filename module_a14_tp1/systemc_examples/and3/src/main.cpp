#include "systemc.h"
#include "stimulus.h"
#include "display.h"
#include "and3.h"

int sc_main (int argc , char *argv[]) 
{
	//Trace file
	vcd_trace_file *output_debug;
	
	//Signal AND3
	sc_signal<bool> e1;
	sc_signal<bool> e2;
	sc_signal<bool> e3;
	sc_signal<bool> s;
	sc_clock clock;

	//Trace file
	output_debug = (vcd_trace_file*) (sc_create_vcd_trace_file("./wave/waveform"));

	//Binding
	stimulus stimulus1("stimulus1");
	stimulus1.e1(e1); 
	stimulus1.e2(e2); 
	stimulus1.e3(e3); 
	stimulus1.clock(clock);

	and3 and3_module("and3_module");
	and3_module.e1(e1);
	and3_module.e2(e2);
	and3_module.e3(e3);
	and3_module.s(s);

	display display1("display1",output_debug);
	display1.and3_e1(e1); 
	display1.and3_e2(e2); 
	display1.and3_e3(e3);
	display1.and3_s(s);
	display1.clock(clock);

	//Simulation
	sc_start(100, SC_NS);
	
	sc_close_vcd_trace_file(output_debug);
	return 0;
}

