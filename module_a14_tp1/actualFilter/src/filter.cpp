#include "filter.h"

//do_filter
void 
filter::do_filter()
  {
    while(1)
    {
      wait();

      //Check the address
      if(addr==0x20000000) 
      {
        //Write
        if(oen && !wen) {
          if(!mem->nb_write(data)) {
	    cout<<"[filter]: internal fifo is too small (size="<<mem->num_free()<<")."<<endl;
            exit(-1);
          }
        }

        //Read
        else if(!oen && wen) {
          unsigned char value;
          if(!mem->nb_read(value)) {
	    cout<<"[filter]: internal fifo is empty."<<endl; 
	  }
          else data = value;
        }
        
        //Main processing
        else if(data_in_ready.read())
        {
          //We have received all the data.
          //Processing can start
          cout<<"[filter]: start processing."<<endl;
          wait(100);
          cout<<"[filter]: done."<<endl;
          data_out_ready.write(true);
        }

        //Init
        else if(!data_in_ready.read())
        {
          data_out_ready.write(false);
        }
      }
      else {
        //Not concerned
      }
    }
  }

	
//Constructor
filter::filter(sc_module_name name):sc_module(name)
{
  mem = new sc_fifo<unsigned char> ("mem", 600*600);
  SC_THREAD(do_filter);
  dont_initialize();
  sensitive << clock.pos();
}


//Destructor
filter::~filter() {
  delete mem;
}



