#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 5

void *thread(void *arg);

struct intarg {
	int a;
	char b;
};

void *thread(void *arg) {
	struct intarg *ia = (struct intarg *)arg;
    int myint = ia->a; 
	char mychar = ia->b;
    printf("Thread: %lx myint: %d, %c\n", pthread_self(), myint, mychar);
    return "abcd";
}

int main(int argc, char *argv[]) {

    struct intarg args[N]; //have N of these structs
    pthread_t tid;
    int i;
	void *ret;
 
    for(i=0; i < N; i++) {
	    args[i].a = i; //copying i into the struct
		args[i].b = 'X';
       pthread_create(&tid, NULL, thread, &args[i]); //passing the pointer of the struct
    }
    //pthread_exit(NULL);
	//IF YOU WANT TO KNOW THE RETUN VALUE OF A THREAD
	pthread_join(tid, &ret); //fillin the return value to ret location
	printf("thread return value: %s\n", (char *)ret);
}