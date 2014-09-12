#include "stimulus.h"
#include "pgm_mgt.h"
#include "input_image.h"


//Constructor
stimulus::stimulus(sc_module_name name):sc_module(name)
{
  ImageRes = new unsigned char [IMAGE_X*IMAGE_Y];
  state = INIT;
  next_state = INIT;

  SC_THREAD(do_process);
  dont_initialize();
  sensitive << clock.pos();
}


//Destructor
stimulus::~stimulus(){
  delete [] ImageRes;
}


//Main thread
void stimulus::do_process() 
{
  //Local variables
  static unsigned int i=0;

  while(1)
  {
    wait();
  
    //FSM
    switch(state)
    {
      //INIT
      case(INIT):{
        wen.write(true);
        oen.write(true);
        addr.write(0x20000000);

        next_state = SEND;
        cout<<"[stimulus]: send image...";
        i=0;
      }
      break;

      //SEND
      case(SEND):{
        data.write(MyImage[i]);
        wen.write(false);  
        i++;
        if(i<IMAGE_X*IMAGE_Y) next_state = SEND; 
        else {
          next_state = READY;
          cout<<"done."<<endl;
        }
      }
      break;

      //READY
      case(READY):{
        wen.write(true);
        data_out_ready.write(true);
        next_state = WAIT;
        cout<<"[stimulus]: wait for processing..."<<endl;
      }
      break;

      //WAIT
      case(WAIT):{
        data_out_ready.write(false);
        if(!data_in_ready.read()) next_state=WAIT;
        else {
          next_state = READ;
          cout<<"[stimulus]: done."<<endl;
          cout<<"[stimulus]: get new data...";
          oen.write(false);
          i=0;
        }
      }
      break;

      //READ
      case(READ):{
        ImageRes[i] = data;
        i++;
        if(i<IMAGE_X*IMAGE_Y) next_state = READ; 
        else {
          next_state = END;
          oen.write(true);
          cout<<"done."<<endl;
        }
      }
      break;

      //END
      case(END):{
        pgmWrite("output.pgm", IMAGE_X, IMAGE_Y, ImageRes);
        cout<<"Done."<<endl;
        sc_stop();
      }
      break;
    }

    state = next_state;
  }
}



