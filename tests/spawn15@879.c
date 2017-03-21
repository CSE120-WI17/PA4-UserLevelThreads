#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void iterone(int n)
{
  Printf("I am an iterone thread: %d, my argument is: %d\n", MyGetThread(), n);
}


void iterfour(int n)
{
  int count = 0;
  for (; count < 2; ++count)
    {
      Printf("I am an iterfour thread: %d, my argument is: %d\n", MyGetThread(), n);
      MySchedThread();
    }

  Printf("Spawning new iterone\n");
  MyCreateThread(iterone, n);
  MySchedThread();

  for (count = 0; count < 2; ++count)
    {
      Printf("I am an iterfour thread: %d, my argument is: %d\n", MyGetThread(), n);
      MySchedThread();
    }
}

void Main ()
{
  int count = 1;
  MyInitThreads();

  for(; count < MAXTHREADS; ++count)
    {
      if (count % 2 == 1)
        {
          Printf("Spawning thread %d as iterfour\n", count);
          MyCreateThread(iterfour, count);
        }
      else
        {
          Printf("Spawning thread %d as iterone\n", count);
          MyCreateThread(iterone, count);
        }
    }

  MyExitThread ();
}
