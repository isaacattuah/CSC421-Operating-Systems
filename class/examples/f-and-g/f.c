#include<stdio.h>
#include<stdlib.h>

static int fx = 3 ;
extern int fgx  ;

int f(int i) { printf("Hello world %d %d\n",fx,fgx) ; }
