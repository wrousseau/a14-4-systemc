#include "systemc.h"
#include "stimulus.h"
#include "display.h"
#include "filter.h"

int sc_main (int argc , char *argv[]) 
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

  //Trace file
  vcd_trace_file *output_debug;
	
  //Signals
  sc_signal <bool> data_in_ready;
  sc_signal <bool> data_out_ready;	
  sc_signal <bool> oen;
  sc_signal <bool> wen;
  sc_signal <int > addr;
  sc_signal < unsigned char > data;
  sc_clock clock;

  //Trace file
  output_debug = (vcd_trace_file*) (sc_create_vcd_trace_file("./wave/waveform"));

  //Binding
  stimulus stimulus1("stimulus1");
  stimulus1.data_in_ready(data_in_ready);
  stimulus1.data_out_ready(data_out_ready);
  stimulus1.addr(addr); 
  stimulus1.data(data); 
  stimulus1.oen(oen); 
  stimulus1.wen(wen); 
  stimulus1.clock(clock);

  filter filter_module("filter_module");
  filter_module.data_out_ready(data_in_ready);
  filter_module.data_in_ready(data_out_ready);
  filter_module.addr(addr);
  filter_module.data(data); 
  filter_module.oen(oen); 
  filter_module.wen(wen); 
  filter_module.clock(clock);

  display display1("display1",output_debug);
  display1.data_out_ready(data_out_ready);
  display1.data_in_ready(data_in_ready);
  display1.addr(addr);
  display1.data(data); 
  display1.oen(oen); 
  display1.wen(wen); 
  display1.clock(clock);

  //Simulation
  sc_start(-1);
	
  //Done.
  sc_close_vcd_trace_file(output_debug);
  return 0;
}

