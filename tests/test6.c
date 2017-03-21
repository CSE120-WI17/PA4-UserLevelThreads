#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;	// global variables, shared by threads

void Main ()
{
	int i, me;
	void printSquares (), printCubes ();
	int t[11];
	MyInitThreads ();

	me = MyGetThread ();
	t[1] = MyCreateThread (printCubes, me);
	Printf("Created thread: %d\n", t[1]);
	MyYieldThread(t[1]);
	// testing createThread
	for (i = 2; i < 11; i++) { //t[10] = T1
		t[i] = MyCreateThread(printSquares, 10 - i + 1);
		Printf ("Created thread: %d, will yield to %d\n", t[i], 10 - i + 1);
	}

	MyExitThread ();
}

void printSquares (t)
int t;				// thread to yield to
{
	int i;

	for (i = 0; i < NUMYIELDS; i++) {
		square = i * i;
		Printf ("T%d: %d squared = %d\n", MyGetThread (), i, square);
		if (square == 9) {
			Printf("Calling schedThread now\n");
			MySchedThread();
		}
		MyYieldThread (t);
	}
}

void printCubes (t)
int t;				// thread to yield to
{
	int i;

	for (i = 0; i < NUMYIELDS; i++) {
		cube = i * i;
		Printf ("T%d: %d cubed = %d\n", MyGetThread (), i, cube);
		MyExitThread ();
	}
}
