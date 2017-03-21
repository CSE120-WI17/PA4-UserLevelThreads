#include "mythreads.h"
#include "umix.h"
#include "aux.h"

void helper(int n);
void test2();

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


void test2() {
	int a = MyCreateThread(helper, -1);
	int b = MyCreateThread(helper, a);
	int c = MyCreateThread(helper, b);
	int d = MyCreateThread(helper, c);
	int e = MyCreateThread(helper, d);
	int f = MyCreateThread(helper, e);
	int g = MyCreateThread(helper, f);
	int h = MyCreateThread(helper, g);
	int j = MyCreateThread(helper, h);
	Printf("j: %d\n", j);
	MyYieldThread(j);
	int l = MyCreateThread(helper, -1);
	Printf("l: %d\n", l);
	MyYieldThread(l);
}

void Main() {
	MyInitThreads();
	int me = MyGetThread();
	test2();
}

