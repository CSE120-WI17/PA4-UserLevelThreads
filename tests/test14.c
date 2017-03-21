#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUMYIELDS	5

static int square, cube;			// global variable, shared by threads
static int usingMyCode;

void Main ()
{
	usingMyCode = 1;

	void testThreadReuse();		// tests thread re-use (works with usingMyCode)

	testThreadReuse();			// PASS / FAIL
}

void testThreadReuse() {
	void spawnThreads();

	if (usingMyCode)
		MyInitThreads();
	else
		InitThreads();

	int dummy = 10000;  // just so it has something in its stack
	dummy++;

	Printf("Thread 0 dummy var = %d\n", dummy);

	if (usingMyCode)
		MyCreateThread(spawnThreads, 1);
	else
		CreateThread(spawnThreads, 1);

	if (usingMyCode)
		MyExitThread();
	else
		ExitThread();

}

void spawnThreads() {
	void inconsistantThread();

	int dummy = 20000;
	dummy++;

	Printf("Thread 1 dummy var = %d\n", dummy);

	for (int i = 50; i < 70; i++) {
		if (usingMyCode)
			MyCreateThread(inconsistantThread, i);
		else
			CreateThread(inconsistantThread, i);
	}

}

void inconsistantThread(int param) {

	int dummy = param + 30000;
	dummy++;

	int current;
	if (usingMyCode)
		current = MyGetThread();
	else
		current = GetThread();

	Printf("Thread %d dummy = %d\n", current, dummy);

	if (param % 2 == 0) {
		Printf("Thread %d exiting\n", current);
		if (usingMyCode)
			MyExitThread();
		else
			ExitThread();
	}

	Printf("Thread %d giving up control\n", current);

	if (usingMyCode)
		MySchedThread();
	else
		SchedThread();

	Printf("Thread %d is back! Exiting now...\n", current);

}
