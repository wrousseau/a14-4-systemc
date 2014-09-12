#include "stimulus.h"

void stimulus::process() 
{
	while(1)
	{
		//---- WRITE DATA
		//1)Data and address ready
		addr = val_addr+1;
		data = 10*(val_addr+1);
		oen = true;
		wen = true;
		wait();
	
		//2)Write a data
		addr = val_addr+1;
		data = 10*(val_addr+1);
		oen = true;
		wen = false;
		wait();

		//---- READ DATA
		//1)Data and address ready
		addr = val_addr;
		data = "ZZZZZZZZZZZZZZZZ";
		oen = true;
		wen = true;
		wait();
	
		//2)Read a data
		addr = val_addr;
		data = "ZZZZZZZZZZZZZZZZ";
		oen = false;
		wen = true;
		wait();

		//Update values
		val_addr++;
		if(val_addr==256) val_addr=0;
	}
}

