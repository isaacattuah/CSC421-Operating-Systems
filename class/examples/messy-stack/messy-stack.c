#include<stdio.h>

/*
 * example of the unitialized automatic memory of
 * block local variables
 *
 * create: 28 aug 2020 -bjr
 * last-update:
 * 	28 aug 2020 -bjr
 *
 */

/*
 * there are three lifetimes for a variable: 
 * 	auto, static, and dynamic.
 *
 * auto means the storage is allocated automatically when
 * a function is entered and freed automatically when the
 * function exits. These variables are not but default initialized.
 * The lifetime is only during the execution of the function.
 *
 * static means the storage is allocated at the beginning of the
 * program and stays to the end of the program. This storage is
 * zeroed for the programmer during allocation. The lifetime is
 * coincident with the program's execution.
 *
 * dynamic means the storage is allocated by the programmer calling
 * malloc (or other storage allocation function) and released 
 * when the programmer calls free. The lifetime is under direct
 * control of the programmer.
 *
 */


#define N 7
#define T 5


int print_stack() {
	int s[N] ;
	int i ;
	printf("printing the stack.\n") ;
	for (i=0; i<N; i++) {
		printf("\t%d: %d\n", i, s[i]) ;
	}
	return 0 ;
}

int constant_stack(int value) {
	int s[N] ;
	int i ;
	printf("writing the constant value %d on the stack.\n", value) ;
	for (i=0; i<N; i++) {
		s[i] = value ;
	}
	return ++value ;
}

int fibonacci_stack() {
	int s[N] ;
	int i ;
	printf("writing the fibonacci numbers on the stack.\n") ;
	s[0] = s[1] = 1 ;
	for (i=2; i<N; i++) {
		s[i] = s[i-1] + s[i-2] ;
	}
	return 0 ;
}

int quadratic_stack() {
	int s[N] ;
	int i ;
	printf("writing the squares on the stack.\n") ;
	for (i=0; i<N; i++) {
		s[i] = i*i ;
	}
	return 0 ;
}

int main(int argc, char * argv[]) {

	printf("\nWARNING: please do not thing this is a programming technique,\n"
		"this is only to demonstrate that the automatic variables are\n"
		"not initialized, and will have whatever values that last were\n"
		"written into the space used and will be very unpredicable.\n\n") ;

	print_stack() ;
	quadratic_stack() ;
	print_stack() ;
	constant_stack(47) ;
	print_stack() ;
	fibonacci_stack() ;
	print_stack() ;

	return 0 ;
}

