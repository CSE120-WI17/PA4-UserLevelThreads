#include "mythreads.h"
#include "umix.h"
#include "aux.h"

void helper(int n);
void testOneThread();

void helper(int n) {
	if (n >= 0) {
		Printf("%d Helper Yield to: %d\n", MyGetThread(), n);
		int ret = MyYieldThread(n);
		Printf("%d Ret: %d\n", MyGetThread(), ret);
	}
	else {
		Printf("%d Helper Exit %d\n", MyGetThread(), n);
		MyExitThread();
	}
}


void testOneThread() {
	int t = MyCreateThread(helper, 0);
	int ret = MyYieldThread(t);
}

void Main() {
	MyInitThreads();
	int me = MyGetThread();
	testOneThread();
}
