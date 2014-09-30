#include "systemc.h"
#include "stimulus.h"
#include "display.h"
#include "filter.h"
#include "shifter.h"

int sc_main (int argc , char *argv[]) 
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

  //Trace file
  vcd_trace_file *output_debug;
	
  //Signals
  sc_signal <bool> dataInReady;
  sc_signal <bool> dataOutReady;	
  sc_signal <bool> sendProcessOen;
  sc_signal <bool> sendProcessWen;
  sc_signal <int> sendProcessAddr;
  sc_signal <unsigned char> originalData;
  sc_signal <bool> readProcessOen;
  sc_signal <bool> readProcessWen;
  sc_signal <int> readProcessAddr;
  sc_signal <unsigned char> processedData;

  sc_signal <int> verticalSum;
  sc_signal <int> horizontalSum;
  sc_signal <int> center;

  sc_clock clock;

  //Trace file
  output_debug = (vcd_trace_file*) (sc_create_vcd_trace_file("./wave/waveform"));

  //Binding
  stimulus stimulus("stimulus");
  stimulus.dataInReady(dataInReady);
  stimulus.dataOutReady(dataOutReady);
  stimulus.sendProcessAddr(sendProcessAddr); 
  stimulus.sendProcessOen(sendProcessOen); 
  stimulus.sendProcessWen(sendProcessWen);
  stimulus.readProcessAddr(readProcessAddr); 
  stimulus.readProcessOen(readProcessOen); 
  stimulus.readProcessWen(readProcessWen);
  stimulus.originalData(originalData);
  stimulus.processedData(processedData); 
  stimulus.clock(clock);

  shifter<unsigned char, int> shifterModule("shifterModule", 2*IMAGE_X+3);
  shifterModule.din(originalData);
  shifterModule.verticalSumDout(verticalSum);
  shifterModule.horizontalSumDout(horizontalSum);
  shifterModule.centerDout(center);
  shifterModule.clock(clock);

  filter filterModule("filterModule");
  filterModule.dataOutReady(dataInReady);
  filterModule.dataInReady(dataOutReady);
  filterModule.verticalSumDin(verticalSum);
  filterModule.horizontalSumDin(horizontalSum);
  filterModule.centerDin(center);
  filterModule.readProcessAddr(readProcessAddr); 
  filterModule.processedData(processedData); 
  filterModule.clock(clock);

  display displayModule("displayModule",output_debug);
  //displayModule.dataOutReady(dataOutReady);
  //displayModule.dataInReady(dataInReady);
  //displayModule.addr(addr);
  displayModule.originalData(originalData); 
  displayModule.processedData(processedData); 
  //displayModule.oen(oen); 
  //displayModule.wen(wen); 
  displayModule.clock(clock);

  //Simulation
  sc_start(-1);
	
  //Done.
  sc_close_vcd_trace_file(output_debug);
  return 0;
}

