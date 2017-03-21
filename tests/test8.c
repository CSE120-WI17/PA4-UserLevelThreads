#include "mythreads.h"
#include "umix.h"
#include "aux.h"

void helper(int n);
void testMultThread();

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

void testMultThread() {
	int t = MyCreateThread(helper, -1);
	int s = MyCreateThread(helper, t);
	int f = MyCreateThread(helper, s);
	MyYieldThread(f);
}

void Main() {
	MyInitThreads();
	int me = MyGetThread();
	testMultThread();
}
