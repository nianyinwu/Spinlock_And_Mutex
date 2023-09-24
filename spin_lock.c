#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
int num_thr;
int critical_section_size, noncritical_section_size, num_iter;

atomic_int start=0;
pthread_spinlock_t spinlock;
struct timespec t;

void func(void *givenName) {
	int givenID = (intptr_t)givenName;
	cpu_set_t set; CPU_ZERO(&set); CPU_SET(givenID, &set);
	while (start == 0);
	for (int i=0 ; i < num_iter; i++) {
		pthread_spin_lock(&spinlock);
		//printf("%d in cs\n",givenID);
		struct timespec t1 = {0,critical_section_size};
		nanosleep(&t1,NULL);
		pthread_spin_unlock(&spinlock);
		struct timespec t2 = {0,random()%noncritical_section_size};
		nanosleep(&t2,NULL);
 	}
}

int main(int argc, char** argv) {
	sscanf(argv[1], "%d", &num_thr);
	sscanf(argv[2], "%d", &critical_section_size);
	sscanf(argv[3], "%d", &noncritical_section_size);
	sscanf(argv[4], "%d", &num_iter);
	pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * num_thr);
	pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
	for(long i = 0; i<num_thr;i++){
		pthread_create(&tid[i],NULL,(void *) func,(void*)i);
	}
	atomic_store(&start,1);
	for(int i = 0; i<num_thr;i++){
		pthread_join(tid[i],NULL);
	}
	return (0);
}
