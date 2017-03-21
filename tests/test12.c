#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void Main ()
{
	usingMyCode = 1;
	
	void testWithPrintParam();	// testing param passing (works with usingMyCode)

	testWithPrintParam();			// PASS / FAIL
}


void testWithPrintParam () {
	void printParam();

	if(usingMyCode)
		MyInitThreads();
	else
		InitThreads();
	
	for(int i = 0; i < 15; i++) {
		if(usingMyCode)
			MyCreateThread(printParam, i);
		else
			CreateThread(printParam, i);

		if(usingMyCode)
			MySchedThread();
		else
			SchedThread();
	}

	printParam(-1);

	if(usingMyCode)
		MyExitThread();
	else
		ExitThread();

	Printf("Manual Exit\n");
	Exit();

}

void printParam(int p) {
	//Printf("---In printParam\n");

	int id;
	if(usingMyCode)
		id = MyGetThread();
	else
		id = GetThread();

	Printf("Thread %d with param %d\n", id, p);
}

