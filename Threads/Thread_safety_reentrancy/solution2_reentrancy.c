#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_STRING_SIZE 32768

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
/*----------------------------examples of reentant functions using plus_five()-------------------------------------*/
/*	EXPLICITLY REENTRANT FUNCTION - can be called by multiple threads concurrently.
	~Parameters are copied by value, no pointers, no non-local ars in the body~
	Runs on the stack (all the values are on the stack) & each thread has a COPY of those values for is own execution.
*/
int plus_five(int start) {
	return start + 5;
}

/*	IMPLICITLY REENTRANT FUNCTION - it is up to the caller to pass a LOCAL buffer (so it resides on the stack), 
	- has the potential to share memory if it is passed a global variable.
*/
int plus_five2(int start, int *buffer) {
	*buffer = start + 5;
	return start + 5;
}
/*----------------------------------making str_to_upper() reentrant--------------------------------------------------*/
char *str_to_upper(char *string, char *buffer) { 

//	static char buffer[MAX_STRING_SIZE]; ---------> BAD
	int index;							 
	
	for(index = 0; string[index]; index++) {
		buffer[index] = toupper(string[index]); 
	}
	buffer[index] = 0;	

	return buffer;
}

void* t1(void *arg) {
	char buffer[MAX_STRING_SIZE]; //local buffer
	
	while(1) {
		pthread_mutex_lock(&lock);
		printf("%s\n", str_to_upper("T1: I am Thread Number 1", buffer));
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void* t2(void *arg) {
	char buffer[MAX_STRING_SIZE]; //local buffer
	
	while(1) {
		pthread_mutex_lock(&lock);
		printf("%s\n", str_to_upper("T2: This is the second thread", buffer));
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t thread_a, thread_b;
	
	pthread_create(&thread_a, NULL, t1, NULL);
	pthread_create(&thread_b, NULL, t2, NULL);
	
	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);
	
	exit(0);
}