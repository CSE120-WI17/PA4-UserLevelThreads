#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void originalPa4b();
void printSquares (t);

void Main ()
{
	usingMyCode = 1;
	
	void originalPa4b();
	originalPa4b();			// PASS / FAIL
}

void originalPa4b() {
	int i, t;
	void printSquares ();

	MyInitThreads ();		// initialize, must be called first

	MyCreateThread (printSquares, 0);

	for (i = 0; i < NUMYIELDS; i++) {
		MyYieldThread (1);
		Printf ("T0: square = %d\n", square);
	}

	MyExitThread ();
}

void printSquares (t)
	int t;				// thread to yield to
{
	int i;

	for (i = 0; i < NUMYIELDS; i++) {
		square = i * i;
		Printf ("T1: %d squared = %d\n", i, square);
		MyYieldThread (t);
	}
}
