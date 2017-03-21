#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void Main ()
{
	usingMyCode = 1;

	void piazzaTest3();			// simple test for MySched

	piazzaTest3();			// PASS / FAIL
}

void piazzaTest3 ()
{
	int i, t, me;
	void printSquaresFor3 (), realPrintCubes ();

	MyInitThreads ();
	me = MyGetThread ();
	t = MyCreateThread (printSquaresFor3, me);
	t = MyCreateThread (realPrintCubes, t);
	for (i = 0; i < NUMYIELDS; i++) {
		MyYieldThread (t);
		Printf ("T%d: square = %d, cube = %d\n", me, square, cube);
	}

	MyExitThread ();
}


void printSquaresFor3 (t)
int t;				// thread to yield to
{
	int i;
	for (i = 0; i < NUMYIELDS; i++) {
		square = i * i;
		Printf ("T%d: %d squared = %d\n", MyGetThread (), i, square);

		MySchedThread();
		MyYieldThread (t);
	}
}


void realPrintCubes (t)
int t;				// thread to yield to
{
	int i;

	for (i = 0; i < NUMYIELDS; i++) {

		cube = i * i * i;
		Printf ("T%d: %d cubed = %d\n", MyGetThread (), i, cube);
		MySchedThread();
		MyYieldThread (t);
	}
}
