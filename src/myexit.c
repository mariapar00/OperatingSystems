#include "myexit.h"


void detach_shm(struct row *shared_memory){
    int err = 0;
    /* Detach segment */
    err = shmdt(shared_memory);
    if (err == -1) {
        perror ("Detachment.");
        exit(4);
    } else {
        printf(">> Detachment of Shared Segment %d\n", err);
    }
}


void remove_shm( int shmid){
    int err = 0;
    /* Remove segment */
    err = shmctl(shmid, IPC_RMID, 0);
    if (err == -1) {
        perror ("Removal.");
        exit(5);
    } else {
        printf("Just Removed Shared Segment. %d\n", (int)(err));
    }
}


void remove_sem(int semid){
    int err = 0;
    err = semctl(semid, 0, IPC_RMID);
    if (err == -1) {
        perror ("Removal.");
        exit(6);
    } else {
        printf("Just Removed Set of Semaphores. %d\n", (int)(err));
    }
}