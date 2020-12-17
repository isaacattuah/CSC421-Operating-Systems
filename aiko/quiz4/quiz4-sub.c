#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<assert.h>
#include "quiz4.h"

/*
 * author: Isaac Attuah
 * date: 9/16/2020
 * pledge: this is my own work, unless otherwise noted
 *
 */


//#define USAGE_MESSAGE "usage: quiz4  _word1_ _word2_ ... "

void capitalize(char * s) {
	while (*s) {
		*s = toupper(*s) ;
		s++ ;
	}
	return ;
}

void capitalize_all(char ** sp) {
	while (*sp) {
		capitalize(*sp) ;
		sp++ ;
	}
	return ;
}

void print_all(char ** sp, char * delimit) {

       if (*sp){
               printf("%s",*sp);
               sp++;
        }
       while (*sp){
              printf("%c%s", *delimit,*sp);
              sp++;
        }
        printf("\n");
	return ;
}
