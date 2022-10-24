#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TEXT_SZ 100


struct row {
    int line_num;
	char text[TEXT_SZ];
	int ready_to_exit;
	double mean_time;
};


#endif /* _COMMON_H_ */