/*	NOTIFICATION OF STATE CHANGES AMONG THREDS USING CONDITIONAL VARIABLES                                           
	A conditional variable must always be associated with a mutex 
*/
#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int shared;

void *thread_1(void *arg) {
	
	struct timespec to_sleep;
	
	to_sleep.tv_sec = 0;
	to_sleep.tv_nsec = 5000000;
	
	while(1) {			//set the shared variable and then sleep for a set amunt of nano seconds
		pthread_mutex_lock(&mutex);
		shared = 42;
		pthread_mutex_unlock(&mutex);
		
		nanosleep(&to_sleep, NULL);
	}
	return NULL;
}

void *thread_2(void *arg) {
	
	struct timespec to_sleep;
	
	to_sleep.tv_sec = 0;
	to_sleep.tv_nsec = 7000000;
	
	while(1) {			
		pthread_mutex_lock(&mutex);
		shared = 8;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cv);
		
		nanosleep(&to_sleep, NULL);
	}
	return NULL;
}


void *thread_3(void *arg) {
	
	while(1) {
		//need to do something important when shared == 8. while(shared != 8); => Polling. Bad
		
		pthread_mutex_lock(&mutex);
		/*Suspend calling process & give up the mutex. If some thread signals the condition (cv),
		  it will wake up the waiting process and reaquire the mutex. */
		while(shared != 8) {
			pthread_cond_wait(&cv, &mutex);
		}
		
		printf("Shared: %d; do something important\n", shared); //will always be 8
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
}

int main(void) {
	pthread_t t1, t2, t3;
	
	pthread_create(&t1, NULL, thread_1, NULL);
	pthread_create(&t2, NULL, thread_2, NULL);
	pthread_create(&t3, NULL, thread_3, NULL);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	exit(0);
}