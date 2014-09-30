#include "appsupport.h"
#include "countersupport.h"

int main()
{
   counter_init();
   start_metric();
   counter_get();
  _printstrp("Hello World!");

   stop_metric();
  return 0;
}
