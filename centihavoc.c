#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#ifndef NUM_STEPS
#define NUM_STEPS 200000
#endif

void f(int p);
int checksum(int *data, size_t count );

// rand() seems to only return 16 bits on solaris
#define rand() ((rand() << 16) ^ rand())

static int steps = 0;

int checksum(int *data, size_t count ) {
  int sum = 0;
  for (int i = 0; i < count; i++) {
    sum ^= data[i];
  }
  return sum;
}

#ifndef CENTIHAVOC_VERBOSE
#define DPrintf(...)
#else
#define DPrintf printf
#endif

int xs_count = 10000; //15000

void f(int p) {
  // guard the frame of f with a buffer on one end and the checksum on the other
  int xs[xs_count];
  int tid = GetThread();
  int iterations = 4 + rand() % 100;
  int checksum_len = xs_count + 2;
  int i, rv, action, new_tid;
  int sum = checksum(xs, checksum_len);

  if (steps++ >= NUM_STEPS) return;

  DPrintf("%d: step %06d f(%08X): %d\n", tid, steps, p, iterations);
  for (i = 0; i < iterations; i++) {
    p ^= rand();
    action = rand() % 5;
    int target = rand() % MAXTHREADS;
    switch (action) {
    case 0:
      new_tid = CreateThread(f, p);
      DPrintf("%d: + %d\n", tid, new_tid); break;
    case 1:
      DPrintf("%d: -> %d\n", tid, target);
      int yielder = YieldThread(target);
      DPrintf("%d: <- %d\n", tid, yielder);
      break;
    case 2:
      DPrintf("%d: -> ?\n", tid);
      SchedThread();
      break;
    case 3:
      if (!(rand() % 20000)) {
        DPrintf("%d: exiting after %d iterations\n", tid, iterations);
        ExitThread();
      }
      break;
    case 4:
      if (!(rand() % 100)) {

        // check that the buffer has the same checksum as before
        int cur_check = checksum(xs, checksum_len);

        if (cur_check != sum) {
          printf("%d: checksum was %08X; expected %08X\n", tid, cur_check, sum);
          exit(-1);
        }

        // scramble the buffer
        for (int j = 0; i < xs_count; i++) {
          xs[j] ^= rand();
        }

        sum = checksum(xs, checksum_len);
        break;
      }
    }
  }
  DPrintf("%d: finished %d\n", tid, iterations);
}

void Main() {
  srand(1 - 800 - 120 - 0xDEED);
  InitThreads();

  f(0);
  ExitThread();
}
