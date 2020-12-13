#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 5

void *thread(void *arg);
// ssize_t write(int fd, const void *buf, size_t count);
struct write_args {
	int fd;
	const void *buf;
	size_t count;
};
// A wrapper function  for write() so that it can be executed in a thread
static void *write_wrapper(void *arg) {
	struct write_args *wargs = (struct write_args *)arg; //declare struct pointer and cast arg to that
	long result; 
	
	result = (long)write(wargs->fd, wargs->buf, wargs->count); //return the number of characters written 
	
	return (void *)result; 
}

//entry point of program
int main(int argc, char *argv[]) {

	long		helper_ret;
	pthread_t	helper;
	void		*result;
	struct write_args args;
 
	args.fd = 1; //fd for stdout
	args.buf = "Hello\n";
	args.count = 6;
	
    pthread_create(&helper, NULL, write_wrapper, &args); //passing the pointer of the struct
    
	pthread_join(helper, &result); 
	helper_ret = (long)result;
	
	printf("return value: %ld\n", helper_ret);
	exit(0);
}