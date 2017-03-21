#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void Main ()
{
	usingMyCode = 1;
	
	void piazzaTest1();			// using all threads, exit/scheduling, re-use of 0

	piazzaTest1();			// PASS / FAIL
}

void piazzaTest1() {
	int i, t, me;       

  	void exitT(), printCubes2();
  	MyInitThreads ();
	me = MyGetThread ();    
  	t = MyCreateThread(exitT, me); //1   
  	t = MyCreateThread(printCubes2, t); //2  
  	t = MyCreateThread(exitT, t); //3  
  	t = MyCreateThread(exitT, t); //4   
  	t = MyCreateThread(exitT, t); //5   
  	t = MyCreateThread(exitT, t); //6  
  	t = MyCreateThread(exitT, t); //7 
  	t = MyCreateThread(exitT, t); //8                                      
  	t = MyCreateThread(exitT, t); //9  
  	Printf ("T%d: square = %d, cube = %d\n", me, square, cube);                    
  	MyExitThread ();  

}

void exitT (t)                                
  int t;        // thread to yield to                                              
{                                                                                  
  int i;                                                                           
  Printf("Exiting %d\n", MyGetThread());     
  MyExitThread();                                                               
}      

void printCubes2 (t)                                                             
  int t;        // thread to yield to                                           
{                                                                               
  int i;                                                                      
  t = MyCreateThread(exitT, t); //10                                     
  Printf("Created thread %d\n", t);                                             
  Printf("Exiting %d\n", MyGetThread());                                        
  MyExitThread();                                                               
}  
