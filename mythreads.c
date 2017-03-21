/*	User-level thread system
 *
 */

#include <setjmp.h>
#include <string.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h" 

#define STACKSIZE	65536		// maximum size of thread stack

static struct thread {			// thread table
	int valid;			// 1 if entry is valid, else 0
	jmp_buf env;			// current context

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

	MyInitThreadsCalled = 1;
}

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
}

/*	MyGetThread () returns ID of currently running thread.  
 */

int MyGetThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("GetThread: Must call InitThreads first\n");
		Exit ();
	}
	return thread_idx;

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
	if (queue.size == 0) Exit();
	else if (queue.size == 1 || thread[MyGetThread()].valid == 0) 
		MyYieldThread(queue.queue[queue.head]);
	else MyYieldThread(getNextOf(queue.head));

}

/*	MyExitThread () causes the currently running thread to exit.  
 */

void MyExitThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("ExitThread: Must call InitThreads first\n");
		Exit ();
	}

	//thread should exit now
	thread[MyGetThread()].valid = 0;
	get();
	MySchedThread();
}
