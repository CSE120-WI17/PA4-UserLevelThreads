#include "aux.h"
#include "umix.h"
#include "mythreads.h"
 
#define NUMYIELDS   20
 
static int square, cube;    // global variables, shared by threads
 
void Main ()
{
    int i, t, me;
    void printSquares (), printCubes (), printSquare_again();
    
    MyInitThreads();
    
    me = MyGetThread();
    t = MyCreateThread(printSquares, me);
    Printf("creating %d\n",t);
    t = MyCreateThread(printCubes, t);
    Printf("creating %d\n",t);
    
    for (i = 0; i < NUMYIELDS; i++) {
        //0 yields to 2
        Printf("Yield Return %d\n",MyYieldThread(0));
        Printf ("T%d: square = %d, cube = %d\n", me, square, cube);
    }
    
    MyExitThread ();
}
 
 
 
void printSquares_again (t)
int t;              // thread to yield to
{
    int i;
    //1 yields to 0
    for (i = 0; i < NUMYIELDS; i++) {
        square = i * i;
        Printf ("T%d: %d squared_again = %d\n", MyGetThread (), i, square);
        Printf("Yield Return %d\n",MyYieldThread(t));
    }
}
 
 
void printSquares (t)
int t;              // thread to yield to
{
    int i;
    //1 yields to 0
    for (i = 0; i < NUMYIELDS; i++) {
        square = i * i;
        Printf ("T%d: %d squared = %d\n", MyGetThread (), i, square);
        Printf("Yield Return %d\n",MyYieldThread(t));
        int create = MyCreateThread(printSquares_again, i+1);
        Printf("creating %d\n",create);
    }
}
 
void printCubes (t)
int t;              // thread to yield to
{
    int i;
    
    //2 yields to 1
    for (i = 0; i < NUMYIELDS; i++) {
        cube = i * i * i;
        Printf ("T%d: %d cubed = %d\n", MyGetThread (), i, cube);
        Printf("Print Cubes: thread 2 yields to %d \n", t);
        Printf("Yield Return %d\n",MyYieldThread(t));
        int create = MyCreateThread(printSquares, i+1);
        Printf("creating %d\n",create);
    }   
}
