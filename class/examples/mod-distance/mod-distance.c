#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/*
 * mod-distance.c
 * last update: 
 *   12 sep 2020 bjr- creted
 *
 * calculate the difference bwteen a and b when they
 * modular numbers. to be precise: we know how to increment
 * a. the distance to b is the number of increments we need
 * to apply to a to get to b
 *
 */

#define M 19 


int mod_distance_slow_but_sure(int a,int b,int m) {
	int diff = 0 ;

	// are you insane or not?
	assert(m>1 && a>=0 && b>=0 ) ;
	a = a % m ;
	b = b % m ;
	// not insane

	while (1==1) {
		if (a==b) return diff ;
		diff += 1 ;
		a = (a+1)%m ;
	}
	assert(1==0) ; // never gets here	
}

int mod_distance(int a, int b, int m) {
	// are you insane or not?
	assert(m>1 && a>=0 && b>=0 ) ;
	a = a % m ;
	b = b % m ;
	// not insane

	return (b-a+m)%m ;
}


/* 
 * explanation of the formula. 
 *
 *    the mod number systems for mod 5, would be 0 1 2 3 4 then back to 0. 
 *    from the point of walking forward, if you start with 0 through 4, you 
 *    extend the "road" by the next 5 numbers: 0 1 2 3 4 5 6 7 8 9
 *    now the distance from a to b where b<a is clear: move the end point 
 *    b to b+5 and then step from a to b.
 *
 *    Example:
 *        b = 2, a = 4, originally you think:
 *
 *            b   a
 *        0 1 2 3 4 
 *
 *        but you change it to:
 *
 *                a     b
 *        0 1 2 3 4 5 6 7 8 9 
 *
 *        the disance has not changed because in the first case, "a" must walk to 
 *        the "roll over point" from 4 to 0, then continue walking to b; 
 *        Instead of "a" rolling over, we boost b beyond 4 by the number of 
 *        steps it is from the roll over point. This is just adding 5 to b, 
 *        or in general, adding the modulus m to b.
 *
 *    Therefore, when b<a, the distance is (b+m)-a, and when b>=a, the distance 
 *    is b-a. This is the "obvious" case. (Now both cases should be obvious.)
 * 
 *    But you know what? Both these formulas can be combined into (b-a+m)%m.
 *
 */


int main(int argc, char * argv[]) {
	int i, j ;
	int m = M ;
	int count = 0 ;
	for (i=0;i<m;i++) {
		for (j=0;j<m;j++) {
			assert( mod_distance(i,j,m) == mod_distance_slow_but_sure(i,j,m)) ;
			count++ ;
		}
	}
	printf("%d cases checked for mod %d\nNo news is good news!\n",count,m) ;
	return 0;
}

