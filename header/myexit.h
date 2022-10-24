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

void detach_shm(struct row * shared_memory);

void remove_shm(int shmid);

void remove_sem(int semid);