#include "stimulus.h"
#include "pgm_mgt.h"
#include "input_image.h"


// Constructor
stimulus::stimulus(sc_module_name name):sc_module(name)
{
  processedImage = new unsigned char [IMAGE_X*IMAGE_Y];
  sendProcessState = INIT;
  sendProcessNextState = INIT;
  readProcessState = INIT;
  readProcessNextState = INIT;

  SC_THREAD(sendProcess);
  dont_initialize();
  sensitive << clock.pos();

  SC_THREAD(readProcess);
  dont_initialize();
  sensitive << clock.pos();
}


// Destructor
stimulus::~stimulus(){
  delete [] processedImage;
}


// Send thread
void stimulus::sendProcess() 
{
  //Local variables
  static unsigned int i=0;

  while(1)
  {
    wait();

    // FSM
    switch(sendProcessState)
    {
      // INIT
      case(INIT):{
        sendProcessWen.write(true);
        sendProcessOen.write(true);
        sendProcessAddr.write(0x20000000);

        sendProcessNextState = SEND;
        cout<<"[stimulus]: send image...";
        i=0;
      }
      break;

      // SEND
      case(SEND):{
        originalData.write(MyImage[i]);
        sendProcessWen.write(false);  
        i++;
        if(i<IMAGE_X*IMAGE_Y) sendProcessNextState = SEND; 
        else {
          sendProcessWen.write(true);
          sendProcessNextState = END;
        }
      }
      break;

      // END
      case(END):{
        cout<<"Done."<<endl;
        //sc_stop();
      }
      break;

      
    }

    sendProcessState = sendProcessNextState;
  }
}

void stimulus::readProcess()
{
  //Local variables
  static unsigned int i=0;

  while (1)
  {
    wait();

    switch (readProcessState)
    {
      // INIT
      case(INIT):{
        readProcessWen.write(true);
        readProcessOen.write(true);
        readProcessAddr.write(0x80000000);

        readProcessNextState = WAIT;
        cout<<"[stimulus]: read image...";
        i=0;
      }
      break;

      /*// READY
      case(READY):{
        //dataOutReady.write(true);
        readProcessNextState = WAIT;
        cout<<"[stimulus]: wait for processing..."<<endl;
      }
      break;
*/
      // WAIT
      case(WAIT):{
        //dataOutReady.write(false);
        if(!dataInReady.read()) readProcessNextState=WAIT;
        else {
          readProcessNextState = READ;
          cout << "[stimulus]: done." << endl;
          cout << "[stimulus]: get new data...";
          readProcessOen.write(false);
          i=0;
        }
      }
      break;

      // READ
      case(READ):{
        processedImage[i] = processedData;
        i++;
        if(i<IMAGE_X*IMAGE_Y) readProcessNextState = READ; 
        else {
          readProcessNextState = END;
          readProcessOen.write(true);
        }
      }
      break;

      // END
      case(END):{
        cout << "Ending..." << endl;
        pgmWrite("output.pgm", IMAGE_X, IMAGE_Y, processedImage);
        cout<<"Done."<<endl;
        cout << sc_time_stamp() << endl;
        sc_stop();
      }
      break;
    }

    readProcessState = readProcessNextState;


  }
  
}



