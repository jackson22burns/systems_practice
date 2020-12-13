#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 5
/* THIS WILL NOT WORK - THE VALUE OF GLOBAL i WILL BE CHANGED BEFORE REACHING THE PRINT STATEMENT WITHIN THE THREAD
void *thread(void *arg) {
   int myint = *((int *)arg);
   printf("Thread: %lx myint: %d\n", pthread_self(), myint);
   return NULL;
}

int main(int argc, char *argv[]) {

   pthread_t tid;
   int i;
 
   for(i=0; i < N; i++) {
      pthread_create(&tid, NULL, thread, &i); 
   }
   pthread_exit(NULL);
} */

//better version
void *thread(void *arg) {
   int myint = (int)((long)arg); //DIFFERENT. long is optional here
   printf("Thread: %lx myint: %d\n", pthread_self(), myint);
   return NULL;
}

int main(int argc, char *argv[]) {

   pthread_t tid;
   int i;
 
   for(i=0; i < N; i++) {
      pthread_create(&tid, NULL, thread, (int *)(long)i); //DIFFERENT, cast the value of i to pointer here instead. long cast just fixes warning
   }
   pthread_exit(NULL);
}