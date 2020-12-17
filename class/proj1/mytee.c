#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>


/*
 * author:
 * date: 
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. file in author and date above and modify
 *
 * template last change: 8 oct 2019
 *
 */

#define USAGE_MESSAGE "usage: mytee [-av] _filename_"

/* globals and externs go here */


int main(int argc, char * argv[]) {
	int ch ;
	int is_verbose = 0 ;
	int is_append = 0 ;
	char * mode_s = NULL ;
	char * filename ;
	FILE * f ;

	while ((ch = getopt(argc, argv, "va")) != -1) {
		switch(ch) {
		/*
		 * modify or add to these case statements
		 */
		case 'v':
			is_verbose = 1 ;
			break ;
		case 'a':
			is_append = 1 ;
			break ;
		default:
				printf("%s\n", USAGE_MESSAGE) ;
				return 0 ;
		}
	}
	argc -= optind; // these are globals defined in the getopt code
	argv += optind;

	if (argc!=1) {
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
	
	filename = argv[0] ; // access to data protected by an assertion that data is in fact there
	mode_s = "w" ;
	if (is_verbose) { // example of using verbose 
		printf("%s:%d: openning file %s in mode %s\n", 
				__FILE__, __LINE__, filename, mode_s) ;
	}
	f = fopen(filename,mode_s) ;
	assert(f) ; // example of using assert's for DEBUGGING PURPOSES, to weed out errors
	
	while (1) { 
		// this is just placeholder code, you will need a loop 
		break ;
	}

	return 0 ;
}


