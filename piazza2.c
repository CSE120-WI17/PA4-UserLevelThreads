#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS 5

static int square, cube;      // global variable, shared by threads
static int usingMyCode;

void piazzaTest2();
void printCubes (int t);
void printCubes1 (int t);
void printCubes22 (int t);
void printCubes3 (int t);
void exitT (int t) ;
void Main ()
{
  usingMyCode = 1;

  void piazzaTest2();     // reusing thread 0 and FIFO

  piazzaTest2();      // PASS / FAIL
}

void piazzaTest2 ()
{
  int i, t, me;
  void exitT (), printCubes (), printCubes1 (),
       printCubes22 (), printCubes3 ();
  MyInitThreads ();
  me = MyGetThread ();
  t = MyCreateThread (printCubes3, t); //1
  t = MyCreateThread (printCubes3, t); //2
  t = MyCreateThread (printCubes3, t); //3
  t = MyCreateThread (printCubes3, t); //4
  t = MyCreateThread (printCubes3, t); //5
  t = MyCreateThread (printCubes3, t); //6
  t = MyCreateThread (printCubes3, t); //7
  t = MyCreateThread (printCubes3, t); //8
  t = MyCreateThread (printCubes3, t); //9
  Printf ("T%d: square = %d, cube = %d\n", me, square, cube);
  MyExitThread ();
}

void printCubes (t)
int t;        // thread to yield to
{
  int i;
  t = MyCreateThread(exitT, t);
  Printf("In printCubes...Created thread %d\n", t);
  Printf("In printCubes...Exiting %d\n", MyGetThread());
  MyExitThread();
}

void printCubes1 (t)
int t;        // thread to yield to
{
  int i;
  t = MyCreateThread(printCubes, t);
  Printf("In printCubes1...Created thread %d\n", t);
  Printf("In printCubes1...Exiting %d\n", MyGetThread());
  MyExitThread();
}

void printCubes22 (t)
int t;        // thread to yield to
{
  int i;
  t = MyCreateThread(printCubes1, t);
  Printf("In printCubes22...Created thread %d\n", t);
  Printf("In printCubes22...Exiting %d\n", MyGetThread());
  MyExitThread();
}

void printCubes3 (t)
int t;        // thread to yield to
{
  int i;
  t = MyCreateThread(printCubes22, t);
  Printf("In printCubes3...Created thread %d\n", t);
  Printf("In printCubes3...Exiting %d\n", MyGetThread());
  MyExitThread();
}

void exitT (t)                                
  int t;        // thread to yield to                                              
{                                                                                  
  int i;                                                                           
  Printf("Exiting %d\n", MyGetThread());     
  MyExitThread();                                                               
} 
