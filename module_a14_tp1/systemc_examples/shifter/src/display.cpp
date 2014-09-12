#include "display.h"

void display::entry()
{
	//Display results
	cout << "din= " << din.read() <<" -> dout= "<< dout.read() << endl;
}

