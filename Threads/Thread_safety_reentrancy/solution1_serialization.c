#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_SIZE 32768

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//THREAD UNSAFE FUNCTION
char *str_to_upper(char *string) { //pass string and convert all characters to uppercase

	static char buffer[MAX_STRING_SIZE]; //static means we are declaring the variable to reside in memory region. 
	int index;							 // (pointing to the same buffer). t1 may change the value of bufer before t2 prints!
	
	for(index = 0; string[index]; index++) {
		buffer[index] = toupper(string[index]); //toupper() is an existing C function
	}
	buffer[index] = 0;	

	return buffer;
}

void* t1(void *arg) { //OK
	while(1) {
		pthread_mutex_lock(&lock);
		printf("%s\n", str_to_upper("T1: I am Thread Number 1"));
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void* t2(void *arg) { //BETTER - because we are not holding the lock when printing
	char buffer[MAX_STRING_SIZE];
	while(1) {
		pthread_mutex_lock(&lock);
		char *upper = str_to_upper("This is the second thread");
		memcpy(buffer, upper, strlen(upper)+1);
		pthread_mutex_unlock(&lock);
		
		printf("T2: %s\n", buffer);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t thread_a, thread_b;
	
	pthread_create(&thread_b, NULL, t2, NULL);
	pthread_create(&thread_a, NULL, t1, NULL);
	
	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);
	
	exit(0);
}