#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include "common.h"
#include "operation.h"



int main(int argc, char *argv[]){
    int N = atoi(argv[1]);
    int line_count = atoi(argv[2]);
    int requests = 0;
    struct row *shared_memory = (void *)0;
    int semid;
    
    int shmid = shmget(1, sizeof(struct row), 0); // h shmget mou dinei to id ths shared memory
    if (shmid == -1) {
        perror ("Creation");
        exit(7);
    } else {
    }

    shared_memory = (struct row *) shmat(shmid, NULL,0);
	if (shared_memory == (void *)-1){
		perror("Attachment.");
        exit(8);
	}
    shared_memory->ready_to_exit = 0;
    shared_memory->mean_time = -1;

	if ((semid = semget(10, 4, 0)) < 0){ // returns the semaphore set identifier
		perror("semget");
		exit(9);
	}
    
    clock_t start, end;
    double time_spent;
    double time_sum = 0;
    srand(getpid());   // seeding the random number generator used by rand()
    while(requests!=N){
        // down sem2
        down(semid, 1);

        // give line number
        if (shared_memory->line_num == 0){
            shared_memory->line_num = rand() % line_count + 1;
            start = clock();
            requests++;
        }

        // up sem1
        up(semid, 0);

        // down sem3
        down(semid, 2);     // waiting for the parent to write the line

        // receive line
        if (strlen(shared_memory->text) != 0){
            end = clock();
            time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            time_sum += time_spent;
            printf("%d received line:  %s", getpid(), shared_memory->text);
            memset(shared_memory->text, 0, 100);
        }

        if (requests==N){
            shared_memory->ready_to_exit = 1;
            shared_memory->mean_time = time_sum / N;
            // up sem4
            up(semid, 3);
            exit(0);
        }
        // up sem4
        up(semid, 3);   

        // up sem2
        up(semid, 1);
    }
}