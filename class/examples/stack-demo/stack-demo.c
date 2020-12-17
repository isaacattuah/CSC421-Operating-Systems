#include<stdio.h>
#include<stdlib.h>


/*
 * stack frame demonstration program
 * author: bjr
 * created: 11 Sept 2014
 * lastupdate:
 *
 */

int g(int d) {
	int i = d ;
	int * ip = &i ;
	ip += d ;
	for ( ; i>=0; i-- )  {
		printf("%p  0x%08x\n", ip, *ip) ;
		ip -= 1 ;
	}
	printf("done\n") ;
	return 0 ;
}

int f(int n ,int a, int b, int c) {
	int i = 0x0a ;
	int j = 0x0b ;
	int k = 0x0c ;

	if ( n ) f(n-1, a, b, c );
	else g(100) ;
	return 0 ;
}

int main(int argc, char * argv[]) {
	printf("addresses of subroutines:\n  main= %p, f=%p, g=%p\n\n",
		main, f, g ) ;
	f(4,5,6,7) ;
	return 0 ;
}

