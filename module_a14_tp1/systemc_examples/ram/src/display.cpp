#include "display.h"

void display::entry()
{
	//Display results
	cout << "addr= " << addr.read() <<" -> data= "<< data.read() << endl;
}

