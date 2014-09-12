#include "systemc.h"
#include "stimulus.h"
#include "display.h"
#include "ram.h"

int sc_main (int argc , char *argv[]) 
{
	//Trace file
	vcd_trace_file *output_debug;
	
	//Signals
	sc_signal< sc_int<8> > addr;
	sc_signal<bool> oen;
	sc_signal<bool> wen;
	sc_signal < sc_lv<16> > data;
	sc_clock clock;

	//Trace file
	output_debug = (vcd_trace_file*) (sc_create_vcd_trace_file("./wave/waveform"));

	//Binding
	stimulus stimulus1("stimulus1");
	stimulus1.addr(addr); 
	stimulus1.data(data); 
	stimulus1.oen(oen); 
	stimulus1.wen(wen); 
	stimulus1.clock(clock);

	ram ram_module("ram_module");
	ram_module.addr(addr);
	ram_module.data(data); 
	ram_module.oen(oen); 
	ram_module.wen(wen); 

	display display1("display1",output_debug);
	display1.addr(addr);
	display1.data(data); 
	display1.oen(oen); 
	display1.wen(wen); 
	display1.clock(clock);

	//Simulation
	sc_start(100, SC_NS);
	
	sc_close_vcd_trace_file(output_debug);
	return 0;
}

