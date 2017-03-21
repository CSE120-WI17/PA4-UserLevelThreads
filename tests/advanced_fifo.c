#include "aux.h"
#include "umix.h"
#include "mythreads.h"

int count = 1;

void printName(int n)
{
  Printf("I am thread: %d, my argument is: %d\n", MyGetThread(), n);
  if (n == 3)
    MyExitThread();

  if (n == 6 || n == 9)
    MyCreateThread(printName, count++);
  // note that newly spawned threads should go on the end of the queue, so we should see
  // my argument is: 1,2,3,4,5,6,7,8,9   then  10,11,1,2, (skip 3), 4,5,(10!!),6,7,8,(11!!),9 then 10,11

  MySchedThread();
  if (n == 3) {
    Printf("Error! thread with n=3 should have exited!");
    Exit();
  }

  Printf("I am thread: %d, my argument is: %d\n", MyGetThread(), n);

}

void Main ()
{
  MyInitThreads();

  for (; count < MAXTHREADS; ++count)
  {
    Printf("Spawning thread %d\n", count);
    MyCreateThread(printName, count);
  }

  MyExitThread ();
}
