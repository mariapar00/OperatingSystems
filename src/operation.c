#include "operation.h"


void init(int semid, int semnum, int value){
    if (semctl(semid, semnum, SETVAL, value) < 0){
		perror("semctl");
		exit(1);
	}
}


void up(int semid, int semnum){
    struct sembuf oper[1] = {{semnum, 1, 0}};
    if (semop(semid, &oper[0], 1) < 0){
        perror("semop"); 
        exit(1);
    }
}


void down(int semid, int semnum){
    struct sembuf oper[1] = {{semnum, -1, 0}};
    if (semop(semid, &oper[0], 1) < 0){
        perror("semop"); 
        exit(1);
    }
}