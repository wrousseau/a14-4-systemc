#include "stimulus.h"

void stimulus::process() 
{
	// sending values
	din.write(val_din);

	//Update values
	val_din=!val_din;
}

