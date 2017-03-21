#include "aux.h"
#include "umix.h"
#include "mythreads.h"
 
#define NUMYIELDS   5
 
static int square, cube;    // global variables, shared by threads
 
void Main ()
{
    int i, t, me;
    void print1 (), print2 (), print3(), print4 (), print5 (), print6(), print7 (), print8 (), print9();
    
    MyInitThreads();
    
    me = MyGetThread();
    Printf("Created Thread %d\n",MyCreateThread(print1, me));
    Printf("Created Thread %d\n",MyCreateThread(print2, me));
    Printf("Created Thread %d\n",MyCreateThread(print3, me));
    Printf("Created Thread %d\n",MyCreateThread(print4, me));
    Printf("Created Thread %d\n",MyCreateThread(print5, me));
    Printf("Created Thread %d\n",MyCreateThread(print6, me));
    Printf("Created Thread %d\n",MyCreateThread(print7, me));
    Printf("Created Thread %d\n",MyCreateThread(print8, me));
    Printf("Created Thread %d\n",MyCreateThread(print9, me));
    MyYieldThread (9);
    MyExitThread ();
}
 
 
 
void print1 (t)
int t;  // thread to yield to
{
    void print10 ();
        Printf ("Current Thread %d\n", MyGetThread ());
        Printf("Created Thread %d\n",MyCreateThread(print10, 0));
}
 
void print2 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
 
void print3 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
 
void print4 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
 
void print5 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
void print6 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
void print7 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
void print8 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
 
void print9 (t)
int t;              // thread to yield to
{
 
    Printf ("Current Thread %d\n", MyGetThread ());
 
}
 
void print10 (t)
int t;              // thread to yield to
{
    Printf ("Current Thread %d\n", MyGetThread ());
}
