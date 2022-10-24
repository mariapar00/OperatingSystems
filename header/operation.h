#ifndef _OPER_H_
#define _OPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>

void init(int semid, int semnum, int value);

void up(int semid, int semnum);

void down(int semid, int semnum);

#endif 