#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void testYield();
void yieldTo(int t);

void Main ()
{
	usingMyCode = 1;
	
	void testYield();			// correct yield vals (works with usingMyCode)

	testYield();			// PASS / FAIL
}

void testYield() {
	void yieldTo();

	if(usingMyCode)
		MyInitThreads();
	else
		InitThreads();

	if(usingMyCode)
		MyCreateThread(yieldTo, 1);
	else
		CreateThread(yieldTo, 1);

	yieldTo(1);

	if(usingMyCode)
		MyCreateThread(yieldTo, 3);
	else
		CreateThread(yieldTo, 3);

	if(usingMyCode)
		MyCreateThread(yieldTo, 0);
	else
		CreateThread(yieldTo, 0);

	yieldTo(2);

	if(usingMyCode)
		MyExitThread();
	else
		ExitThread();

	Printf("Manual Exit\n");
	Exit();
}

void yieldTo(int t) {
	int current;	
	if(usingMyCode)
		current = MyGetThread();
	else
		current = GetThread();

	Printf("Thread %d yielding to Thread %d\n", current, t);

	int yielded;
	if(usingMyCode)
		yielded = MyYieldThread(t);
	else
		yielded = YieldThread(t);

	Printf("Thread %d got control from Thread %d\n", current, yielded);

}
