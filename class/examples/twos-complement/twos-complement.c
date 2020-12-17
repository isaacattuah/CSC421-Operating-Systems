#include<stdio.h>
#include<stdlib.h>
// this is an unusual thing to need to include .. 
#include<limits.h>

/*
 * two-s-complement.c
 * a program to check the meaning of two's complement
 * author: bjr
 * last update: created 10 sep 2020 bjr
 *
 */

int main(int argc, char * argv[]) {
	unsigned int i ;
	int j,k ;

	for (i=0;i<UINT_MAX;i++) {
		j = -i ;
		k = ((-1)^i)+1 ;
		if ( j!=k ) {
			printf("FAKE NEWS! %d != %d\n", j,k) ;
			exit(0) ;
		}
	}
	printf("Oh too bad, nothing here to debunk.\n") ;
	return 0 ;
}
