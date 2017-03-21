#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS 5

void func(int k) {

}

void createNext() {
  int tid = MyCreateThread(func, 0);
  if (tid == -1) {
    Printf("thread creation failed, exiting\n");
    Exit();
  }
  Printf("thread %d created\n", tid);
  createNext();
}

void Main () // written by me
{
  int i, t, me;
  MyInitThreads ();
  createNext();
  MyExitThread ();
}
