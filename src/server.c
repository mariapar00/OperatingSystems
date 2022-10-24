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
#include "common.h"
#include "operation.h"
#include "myexit.h"


int main(int argc, char *argv[]){
    if(argc != 4) return 0;
    char* X = argv[1];
    int K = atoi(argv[2]);
    int shmid, semid;
    struct row *shared_memory;
    int status;
    int err = 0;
    
    FILE * fp;
    fp = fopen(X, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    /* Make shared memory segment */
    shmid = shmget(1, sizeof(struct row), 0666 | IPC_CREAT); // h shmget mou dinei to id ths shared memory
    if (shmid == -1) {
        perror ("Creation");
        exit(1);
    } else {
    printf("Allocated Shared Memory with ID: %d\n",(int)shmid);
    }

    /* Attach the segment */
    shared_memory = (struct row *) shmat(shmid, NULL, 0); // h shmat mou epistrefei th diefthynsh tou attached shm segment
    if (shared_memory == (void *)-1) {
        perror("Attachment.");
        remove_shm(shmid);
        exit(2);
    } else {
    printf("Shared memory segment with id %d attached at %p\n", shmid, shared_memory);
    }
    shared_memory->line_num = 0;
    
	/* creates 4 semaphores */
	if ((semid = semget(10, 4, IPC_CREAT | 0666)) <0) { // returns the semaphore set identifier
		perror("semget");
		detach_shm(shared_memory);
        remove_shm(shmid);
        exit(3);
	}
	printf("Creating a semaphore set with ID: %d \n", semid);

    /* initialize semaphores */
    init(semid, 0, 0);   // sem1 = 0
    init(semid, 1, 1);   // sem2 = 1
    init(semid, 2, 0);   // sem3 = 0
    init(semid, 3, 0);   // sem4 = 0

    /* reading the file one time to calculate the number of lines */
    char line[TEXT_SZ];
    int line_count = 0;
    while (fgets(line, TEXT_SZ, fp)) {
        line_count++;
    } 
    char str[100];
    sprintf(str, "%d", line_count);

    int exited = 0;
    /* starting K child processes */
    for(int i=0 ; i<K ; i++){
        if(fork() == 0){
            err = execl("client", "client", argv[3], str, NULL);
            if(err == -1){
                // down sem2
                down(semid, 1);
                perror("Execl.");
                // up sem1
                up(semid, 0);     // unblocking parent
                shared_memory->ready_to_exit = 1;
                // up sem4
                up(semid, 3);
                exit(0);
            }
        }
    }

    while(exited != K){
        // down sem1
        down(semid, 0);
       // printf("down sem1\n");

        // receive line number and give line
        if (shared_memory->line_num != 0){
            fseek(fp, 0, SEEK_SET);    // starting to count from the start of the file
            printf("\n");
            printf("Received line request with number %d\n", shared_memory->line_num);
            int i=0;
            while (fgets(line, TEXT_SZ, fp)){
                i++;
                if(i==shared_memory->line_num){
                    strcpy(shared_memory->text, line);
                    shared_memory->line_num = 0;
                    break;
                }
            } 
        }
        // up sem3
        up(semid, 2);  

        // down sem4
        down(semid, 3); // waiting for the child to change or not the values of ready_to_exit and mean_time

        if (shared_memory->ready_to_exit == 1){
            int i = wait(&status);
            printf("Wait returned %d\n", i);
            printf("Average time %f\n", shared_memory->mean_time);
            printf("\n");
            shared_memory->ready_to_exit = 0;
            exited++;
            // up sem2
            up(semid, 1);   // giving the turn to another child-process after the current one finished
        }
    }
    detach_shm(shared_memory);
    remove_shm(shmid);
    remove_sem(semid);

    return 0;
}