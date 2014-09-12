#include "stimulus.h"

void stimulus::process() 
{
	// sending values
	e1.write(val_e1);
	e2.write(val_e2);
	e3.write(val_e3);

	//Update values
	val_e2=val_e1;
	val_e1=!val_e3;
	val_e3=val_e2;
}

