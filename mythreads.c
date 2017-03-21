/*	User-level thread system
 *
 */

#include <setjmp.h>
<<<<<<< HEAD
#include <string.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h" 

#define STACKSIZE	65536		// maximum size of thread stack
=======

#include "aux.h"
#include "umix.h"
#include "mythreads.h"

static int MyInitThreadsCalled = 0;	// 1 if MyInitThreads called, else 0
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c

static struct thread {			// thread table
	int valid;			// 1 if entry is valid, else 0
	jmp_buf env;			// current context
<<<<<<< HEAD

	jmp_buf orig_env;
	int func_param;
	void (*stored_func)();

} thread[MAXTHREADS];

static struct {
	int head;
	int tail;
	int size;
	int queue[MAXTHREADS];
} queue;


static int MyInitThreadsCalled = 0;	// 1 if MyInitThreads called, else 0
static int thread_idx =0;
static int last_insert_idx = 0;
static int retval;

void splitStack(int numstacks);
void put(int val);
int get();
int getNextOf(int idx);
void positionElemHead(int val);
/*void printqueue();

void printqueue(){
	for (int i = queue.head; i < queue.head + queue.size; i++){
		DPrintf("%d ", queue.queue[i%MAXTHREADS]);
	}
	DPrintf("\tHEAD: %d, TAIL: %d, SIZE:%d\n", queue.head,queue.tail,
		queue.size);
}*/
void put(int val){
	if (queue.size < MAXTHREADS){
		queue.tail = (++queue.tail) % MAXTHREADS;
		queue.queue[queue.tail] = val;
		queue.size++;
	}
}

int get(){
	if (queue.size > 0){
		int resval = queue.queue[queue.head];
		queue.head = (++queue.head) % MAXTHREADS;
		queue.size--;
		if (queue.size == 0){
			queue.head = 0;
			queue.tail = -1;
		}
		return resval;
	}
	return -1;
}

int getNextOf(int idx){
	if (queue.size > 1){
		int next = (idx + 1) % MAXTHREADS;
		return queue.queue[next];
	}
	return -1;
}

void positionElemHead(int val){
	for (int i = queue.head; i < (queue.head + queue.size); i++){
		int idx = i % MAXTHREADS;
		if (queue.queue[idx] == val){
			for (int j = i; j > queue.head; j--){
				int temp = queue.queue[(j-1) % MAXTHREADS];
				queue.queue[(j-1) % MAXTHREADS] = queue.queue[j % MAXTHREADS];
				queue.queue[j % MAXTHREADS] = temp;
			}
		}
	}
}


=======
} thread[MAXTHREADS];

#define STACKSIZE	65536		// maximum size of thread stack
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c

/*	MyInitThreads () initializes the thread package. Must be the first
 *	function called by any user program that uses the thread package.  
 */

void MyInitThreads ()
{
	int i;

	if (MyInitThreadsCalled) {		// run only once
		Printf ("InitThreads: should be called only once\n");
		Exit ();
	}

	for (i = 0; i < MAXTHREADS; i++) {	// initialize thread table
		thread[i].valid = 0;
	}

<<<<<<< HEAD
	queue.head = 0;
	queue.tail = -1;
	queue.size = 0;


	if (setjmp(thread[0].env) !=0){
		void (*func)() = thread[0].stored_func;
		int param = thread[0].func_param;
		(*func)(param);
		MyExitThread();
	}else{
		jmp_buf * orig_env = &(thread[0].orig_env);
		jmp_buf * env = &(thread[0].env);
		memcpy(orig_env,env,sizeof(jmp_buf));
		splitStack(MAXTHREADS);
		thread[0].valid = 1;			// initialize thread 0
		put(0);
	}
=======
	thread[0].valid = 1;			// initialize thread 0
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c

	MyInitThreadsCalled = 1;
}

<<<<<<< HEAD
void splitStack(int numstacks){
	if (numstacks < 2) return;
	else{
		char dummy_stack[STACKSIZE];

		//resolves errors resulting from compiler optimization 
		if (((int) &dummy_stack[STACKSIZE-1]) - ((int) &dummy_stack[0]) + 1 
			!= STACKSIZE) {
			Printf ("Stack space reservation failed\n");
			Exit ();
		}

		int thread_id = 1 + MAXTHREADS - numstacks;
		
		if (setjmp(thread[thread_id].env) != 0){
			
			void (*func)() = thread[MyGetThread()].stored_func;
			int param =  thread[MyGetThread()].func_param;

			(*func)(param);
			//we are done with the function, time for thread to exit
			MyExitThread();

		}else{
			jmp_buf * orig_env = &(thread[thread_id].orig_env);
			jmp_buf * env = &(thread[thread_id].env);
			
			memcpy(orig_env, env, sizeof(jmp_buf));
			splitStack(--numstacks);
		}
		
	}
}

=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
/*	MyCreateThread (func, param) creates a new thread to execute
 *	func (param), where func is a function with no return value and
 *	param is an integer parameter.  The new thread does not begin
 *	executing until another thread yields to it. 
 */

int MyCreateThread (func, param)
	void (*func)();			// function to be executed
	int param;			// integer parameter
{
	if (! MyInitThreadsCalled) {
		Printf ("CreateThread: Must call InitThreads first\n");
		Exit ();
	}
<<<<<<< HEAD
	
	int iter_idx = last_insert_idx;
	int d;
	for (d = 0; d < MAXTHREADS; d++){
		iter_idx = (++iter_idx) % MAXTHREADS;
		//I found a spot, break now
		if (!thread[iter_idx].valid) break;
		
	}

	//no room to create more threads; failed operation 
	if (d == MAXTHREADS) return (-1);

	last_insert_idx = iter_idx; 
	
	thread[last_insert_idx].func_param = param;
	thread[last_insert_idx].stored_func = func;
	thread[last_insert_idx].valid = 1;

	
	jmp_buf * orig_env = &(thread[last_insert_idx].orig_env);
	jmp_buf * env = &(thread[last_insert_idx].env);
	
	memcpy(env, orig_env, sizeof(jmp_buf));

	//add to the scheduling queue
	put(last_insert_idx);

	return last_insert_idx;

=======

	if (setjmp (thread[0].env) == 0) {	// save context of thread 0

		/* The new thread will need stack space.  Here we use the
		 * following trick: the new thread simply uses the current
		 * stack, and so there is no need to allocate space.  However,
		 * to ensure that thread 0's stack may grow and (hopefully)
		 * not bump into thread 1's stack, the top of the stack is
		 * effectively extended automatically by declaring a local
		 * variable (a large "dummy" array). This array is never
		 * actually used; to prevent an optimizing compiler from
		 * removing it, it should be referenced.  
		 */

		char s[STACKSIZE];	// reserve space for thread 0's stack
		void (*f)() = func;	// f saves func on top of stack
		int p = param;		// p saves param on top of stack

		if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
			Printf ("Stack space reservation failed\n");
			Exit ();
		}

		if (setjmp (thread[1].env) == 0) {	// save context of 1
			longjmp (thread[0].env, 1);	// back to thread 0
		}

		/* here when thread 1 is scheduled for the first time */

		(*f) (p);			// execute func (param)

		MyExitThread ();		// thread 1 is done - exit
	}

	thread[1].valid = 1;	// mark the entry for the new thread valid

	return (1);		// done, return new thread ID
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
}

/*	MyYieldThread (t) causes the running thread, call it T, to yield to
 *	thread t.  Returns the ID of the thread that yielded to the calling
 *	thread T, or -1 if t is an invalid ID. Example: given two threads
 *	with IDs 1 and 2, if thread 1 calls MyYieldThread (2), then thread 2
 *	will resume, and if thread 2 then calls MyYieldThread (1), thread 1
 *	will resume by returning from its call to MyYieldThread (2), which
 *	will return the value 2.
 */

int MyYieldThread (t)
	int t;				// thread being yielded to
{
<<<<<<< HEAD
	
=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
	if (! MyInitThreadsCalled) {
		Printf ("YieldThread: Must call InitThreads first\n");
		Exit ();
	}

	if (t < 0 || t >= MAXTHREADS) {
		Printf ("YieldThread: %d is not a valid thread ID\n", t);
		return (-1);
	}
	if (! thread[t].valid) {
		Printf ("YieldThread: Thread %d does not exist\n", t);
		return (-1);
	}
<<<<<<< HEAD
	retval = MyGetThread();
	
	//if there is only 1 thread, yield to the same thread
	if (retval == t) return t;

	//insert at the tail
	put(get());
	
	//move t to the head
	positionElemHead(t);
	
    if (setjmp (thread[MyGetThread()].env) == 0) {
    	//give control to another thread
    	thread_idx = t;
    	longjmp (thread[t].env, 1);
    }
    
    return retval;
=======

        if (setjmp (thread[1-t].env) == 0) {
                longjmp (thread[t].env, 1);
        }
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
}

/*	MyGetThread () returns ID of currently running thread.  
 */

int MyGetThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("GetThread: Must call InitThreads first\n");
		Exit ();
	}
<<<<<<< HEAD
	return thread_idx;
=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c

}

/*	MySchedThread () causes the running thread to simply give up the
 *	CPU and allow another thread to be scheduled.  Selecting which
 *	thread to run is determined here. Note that the same thread may
 * 	be chosen (as will be the case if there are no other threads).  
 */

void MySchedThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("SchedThread: Must call InitThreads first\n");
		Exit ();
	}
<<<<<<< HEAD
	if (queue.size == 0) Exit();
	else if (queue.size == 1 || thread[MyGetThread()].valid == 0) 
		MyYieldThread(queue.queue[queue.head]);
	else MyYieldThread(getNextOf(queue.head));

=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
}

/*	MyExitThread () causes the currently running thread to exit.  
 */

void MyExitThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("ExitThread: Must call InitThreads first\n");
		Exit ();
	}
<<<<<<< HEAD

	//thread should exit now
	thread[MyGetThread()].valid = 0;
	get();
	MySchedThread();
=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
}
