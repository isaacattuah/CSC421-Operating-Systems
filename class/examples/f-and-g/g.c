#include<stdio.h>
#include<stdlib.h>

static int gx = 4 ;
extern int fgx ;

int g(int i) { printf("Goodnight moon %d %d\n", gx, fgx) ; } 
