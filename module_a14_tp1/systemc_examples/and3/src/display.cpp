#include "display.h"

void display::entry()
{
	//Display results
	cout << "(e1="<<and3_e1.read()<<") & (e2="<<and3_e2.read()<<") & (e3="<<and3_e3.read()<<") = "<<and3_s.read()<<endl;
}

