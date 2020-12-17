#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include <sys/wait.h>

/*
 * caos-sh.c: a simple shell
 * last update:
 *    15 sept 2020 -bjr; scrambled to make quiz5
 *    13 sept 2020 -bjr: created
 *
 */
 

/* globals go here */
extern int g_debug ;
extern int is_verbose ;

/* defines go here, in SHOUTY_CASE */
#define USAGE_MESSAGE "usage: caos-sh [-v]"
#define EXPECTED_N_ARGS 0
#define PROMPT "?: "
#define BUFFER_LEN 1024
#define SHELL_USAGE_MESSAGE "possible commands are - l: ls, p: ps, d: date"

char * commands_g[3] = {
	"/bin/ls",
	"/bin/ps",
	"/bin/date"
} ;

int main(int argc, char * argv[]) {

	int is_verbose = 0 ;
	int ch ;
	char * ops ;
	int res ;
	char buffer[BUFFER_LEN] ;
	char * selected_command ;
	pid_t pid ;
	int exit_status ;
	char * arg_vector[2] ;

	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			// might need more; depends on assignment
			case 'v':
			// handle multiple levels of verbose
				is_verbose++ ;
				break ;
			default:
			// do drop through to a usage message
				printf("%s\n", USAGE_MESSAGE) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc!=EXPECTED_N_ARGS) {
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}

	printf("%s", PROMPT) ;
	fgets(buffer,BUFFER_LEN,stdin) ;
	while (0==0) {

		selected_command = NULL ;
		switch( buffer[0] ) {
		
		case 'l':selected_command = commands_g[0] ;
		        break;
		case 'd':
			selected_command = commands_g[2] ;
			break;
		case 'p':
			selected_command = commands_g[1] ;
			break ;
				
		case 'q':
			exit(0) ;
			break ;
		
		default:
			printf("%s\n", SHELL_USAGE_MESSAGE) ;
		}
		
			
	
		if (selected_command) {
			
			arg_vector[0] = selected_command ;
			arg_vector[1] = NULL ;
			

			if (!(pid = fork())) {
				// child
				if (is_verbose>0) {
					printf("child runs |%s|\n", selected_command) ;
				}
				execv(selected_command,arg_vector ) ;
				exit(0) ;
			}
			
			
			if (is_verbose>0) {
				printf("parent waits for death of child %d ...\n", pid) ;
					                                
			}
			pid = wait(&exit_status) ;
			
				
		}
		printf("%s", PROMPT) ;
		fgets(buffer,BUFFER_LEN,stdin) ;
		
		

	}
	return 0;
}

