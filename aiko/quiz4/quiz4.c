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


#define USAGE_MESSAGE "usage: quiz4  _word1_ _word2_ ... "


int main(int argc, char * argv[]) {
	int ch ;
	char ** word_array ;
	char ** pp ;
	int i ; 

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch(ch) {
		/*
		 * modify or add to these case statements
		 */
			break ;
		default:
				printf("%s\n", USAGE_MESSAGE) ;
				return 0 ;
		}
	}
	argc -= optind; // these are globals defined in the getopt code
	argv += optind;

	word_array = (char **) malloc((sizeof(void *)*(argc+1))) ;
	for (i=0;i<argc;i++) {
		word_array[i] = argv[i] ;
	}
	word_array[argc] = NULL ;
	
	pp = word_array ;
	capitalize_all(pp) ;
	print_all(word_array," ") ;

	return 0 ;
}


