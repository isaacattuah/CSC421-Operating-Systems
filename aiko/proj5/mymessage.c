#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<assert.h>

#include "mymessage.h"

/*
 * title: mymessage
 * author: bjr
 * last-update: 29 sep 2016 -bjr: created
 * 
 * driver for the mymessage subroutines
 *
 */


/* 
NAME
    mymessage - test the interprocess message system
    
SYNOPSIS
    mymessage [-vm] [-r -n _read_limit_] [-s _write_sleep_ _messsages_]

DESCRIPTION
    mymessage-sub implements an interprocess message system, sending
    single single byte messages, or strings (message mode).
    
    -v verbose. can be optioned multiple times.
    -m message mode. a message is a string. else messages are characters.
    -r be the reader. default is be the writer.
    -n _read_limit_ as reader, exit after _read_limit_ messages received. 
       0 for no read limit.
    -s _write_sleep_ seconds to write between writes.
    
    _messages_ when writer, messages is either the characters to send, or
               if in message mode, a "." separated sequence of messages.
               
    not in message mode, the digits 0-9 in message are converted to binary
    0-9. The value 0 received by the reader will exit the reader.
    
    when in message mode, the message consisting of the length one string "0"
    is sent as the empty string. The empty string when received by the reader
    will exit the reader.
    

HISTORY
    Created for csc421-211

BUGS
    Beta for csc421-211. Bugs will become known.

*/


/* globals go here */
int is_verbose = 0 ;

/* defines go here, in SHOUTY_CASE */
#define USAGE_MESSAGE "usage: mymessage [-vm] [-r -n_read_limit_] [-s _write_sleep_ _messsages_]" 
#define BUFFER_LEN  1024

int main(int argc, char * argv[]) {

	int is_reader = 0 ;
	int likes_octal = 0 ;
	int is_message_mode = 0 ;
	int sleep_time = 0 ;
	int read_limit = 0 ;

	int ch ;
	char * ops ;

	while ((ch = getopt(argc, argv, "vrms:n:")) != -1) {
		switch(ch) {
			case 'v':
				is_verbose += 1 ;
				break ;
			case 'r':
				is_reader = 1 ;
				break ;
			case 'x':
				likes_octal = 1 ;
				break ;
			case 's':
				sleep_time = atoi(optarg) ;
				break ;
			case 'n':
				read_limit = atoi(optarg) ;
				break ;
			case 'm':
				is_message_mode = 1 ;
				break ;
			default:
				printf("%s\n", USAGE_MESSAGE) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if (is_reader && argc) {
		printf("reader must have no argument supplied\n") ;
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
	
	if (!is_reader && !argc) {
		printf("writer must have an argument supplied\n") ;
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
	assert(!is_reader || !argc) ;
	assert(sleep_time>=0) ;
	assert(read_limit>=0) ;
	
	ops = (argv[0])? strdup(argv[0]):argv[0] ;

	if (!is_message_mode) {
		int m ;
		char * mp ;

		// message reader
		if (is_reader) {
			do {
				m = message_reader() ;
				assert(m>=0) ;
				// test for binary transfers. [0-9] are mapped
				printf("%c", (char) ((m<10)?m+'0':m)) ;
				fflush(NULL) ;
			} while (m) ;
			printf("\n") ;
			return 0 ;
		}
	
		// message writer
		for (mp=ops; *mp!='\0'; mp++) {
			if (isdigit(*mp)) {
				// test for binary transfers. [0-9] are mapped
				*mp = *mp - '0' ;
			}
			message_writer(*mp) ;
			sleep(sleep_time) ;
		}

	}
	else {
		//message mode
		char mbuf[BUFFER_LEN] ;
		char * msg ;
		int len ;
		int i ;
		if (is_verbose) printf("mymessage: sending in message mode\n") ;
		
		
		// message reader
		if (is_reader) {
			do {
				len = message_reader_mm(mbuf, BUFFER_LEN) ;
				assert(len>=0) ;
				if (mbuf[len-1]!='\0') {
					if (is_verbose) printf("mymessage: (%d) not a string. Fixing.\n",__LINE__) ;
					mbuf[len-1] = '\0' ;
				}
				printf("%s\n", mbuf) ;
				fflush(NULL) ;
				read_limit = (read_limit<0)?read_limit:(read_limit-1) ;
			} while (strlen(mbuf) && (read_limit!=0)) ;
			return 0 ;
		}
		// message writer
		for (msg = strtok(ops, "."); msg; msg = strtok(NULL,".")) {
			if ((strlen(msg)==1)&&(msg[0]=='0')) msg = "" ;
			if (is_verbose) printf("mymessage: message_writer_mm |%s|\n", msg) ;
			message_writer_mm(msg) ;
			sleep(sleep_time) ;
		}
	}
    return 0 ;
}

