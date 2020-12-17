#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include <ctype.h>

#include "mysyscalls.h"
#include "mymonitor.h"
#include "ringbuf.h"
#include "mymessage.h"

extern int is_verbose ;

/*
 * author: 
 * pledge: this is my own work, unless otherwise noted
 * this is a template. change "author" and continue work
 *
 * update: 27 sep 2020 -bjr created
 */


int message_reader() {
	return 0 ;
}

int message_writer(int m) {
	return 0 ;
}
	
int message_reader_mm(char * msg, int len) {
	return 0 ;
}

int message_writer_mm(char * msg) {
	return 0 ;
}

	
	
//  utility functions and unit tests

void print_nonprintable(char * s, int l) {
	char * t = malloc(((unsigned)(l+1))*sizeof(char)) ;
	int i ;
	assert (t) ;
	memcpy(t,s,l) ;
	for (i=0;i<l;i++) {
		if (!isprint(t[i])) t[i] = '.' ;
	}
	t[l] = '\0' ;
	printf("|%s|\n",t) ;
	free(t) ;
	return ;
}

