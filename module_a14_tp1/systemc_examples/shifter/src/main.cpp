#include "systemc.h"
#include "stimulus.h"
#include "display.h"
#include "shifter.h"

int sc_main (int argc , char *argv[]) 
{
	//Trace file
	vcd_trace_file *output_debug;
	
	//Signal AND3
	sc_signal<bool> din;
	sc_signal<bool> dout;
	sc_clock clock;

	//Trace file
	output_debug = (vcd_trace_file*) (sc_create_vcd_trace_file("./wave/waveform"));

	//Binding
	stimulus stimulus1("stimulus1");
	stimulus1.din(din); 
	stimulus1.clock(clock);

	shifter shifter_module("shifter_module");
	shifter_module.din(din);
	shifter_module.dout(dout);
	shifter_module.clock(clock);

	display display1("display1",output_debug);
	display1.din(din); 
	display1.dout(dout); 
	display1.clock(clock);

	//Simulation
	sc_start(100, SC_NS);
	
	sc_close_vcd_trace_file(output_debug);
	return 0;
}

