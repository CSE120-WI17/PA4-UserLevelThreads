#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void yieldTo(int n) {
  int stackvar = n * 2;
  Printf("%d: [0] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
  Printf("    Yielding: %d -> %d\n", MyGetThread(), n);
  Printf("    Yielded: %d -> %d\n", MyYieldThread(n), MyGetThread());
  MyExitThread ();
  Printf("%d: [1] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
  Printf("    Scheding: %d -> ?\n", MyGetThread());
  MySchedThread();
  Printf("%d: [2] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
}

void Main() {
  int stackvar = -5;
  MyInitThreads();

  MyCreateThread(yieldTo, 1);
  MyCreateThread(yieldTo, 0);
  MyCreateThread(yieldTo, 4);
  MyCreateThread(yieldTo, 2);

  Printf("%d: [0] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);
  Printf("    Yielding: %d -> %d\n", MyGetThread(), 3);
  MyYieldThread(3);
  Printf("%d: [1] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);
  Printf("    Scheding: %d -> ?\n", MyGetThread());
  MySchedThread();
  Printf("%d: [2] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);

  MyCreateThread(yieldTo, 4);
  MyCreateThread(yieldTo, 7);
  MyCreateThread(yieldTo, 8);
  MyCreateThread(yieldTo, 6);

  MyExitThread ();
}
