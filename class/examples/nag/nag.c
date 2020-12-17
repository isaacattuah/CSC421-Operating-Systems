#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>

/*
 * author: burt rosenberg
 * created: 13 feb 2015
 * last update:
 *
 * code hints to do a retry-ACK timer. demonstrates
 * 1- separate reader and writer threads
 * 2- a shared argument struct
 * 3- locking of argument struct
 * 4- sleep of writer thread by timeout or reader notification
 * 5- notification by reader to writer thread of a new read
 * 6- how writer can determine wake up type (notify or timeout)
 * 7- request to exit from reader thread to writer thread
 *
 */


struct T_arg {
 	int req_exit ;
 	int old_count ;
 	int cur_count ;
 	pthread_mutex_t * mutex ;
 	pthread_cond_t * cond  ;
 	char * buf ;
 	} ;

#define SLEEP_TIME 4
#define LINE_MAX 1024
#define QUIT_STRING "quit"
#define PROMPT_NO "Well ... ?" 
#define PROMPT_YES "Is \"%s\" all you have to say for yourself?\n" 
#define INSTRUCTIONS "Speak to me! (\"%s\" to quit)\n" 



void * nag(void * the_args) {
	struct T_arg * t_arg = (struct T_arg *) the_args ;
	struct timeval tv;
    struct timespec ts;
    
	while (1) {
	
		pthread_mutex_lock(t_arg->mutex) ;
		{
			/* holding lock */
			
			gettimeofday(&tv, NULL) ;
			ts.tv_sec = tv.tv_sec + SLEEP_TIME ;
			ts.tv_nsec = 0 ;
			pthread_cond_timedwait (t_arg->cond, t_arg->mutex, &ts) ;
		
			if ( t_arg->cur_count == t_arg->old_count ) {
				printf("%s\n", PROMPT_NO ) ;
			} else {
				printf(PROMPT_YES, t_arg->buf ) ;
				t_arg->old_count = t_arg->cur_count ;
			}
		
		}	
		pthread_mutex_unlock(t_arg->mutex) ;

		if ( t_arg->req_exit) break ;  /* exit has been requested */
	}
	
	pthread_exit(NULL) ;
	assert(0==1) ; /* never gets here */
}


int main(int argc, char * argv[]) {

	pthread_t thread_id ;
	char buf[LINE_MAX], buf_safe[LINE_MAX] ;
	struct T_arg t_arg ;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	
	t_arg.cur_count = t_arg.old_count = 0 ;
	t_arg.req_exit = 0 ;
	t_arg.buf = buf_safe ; 
	t_arg.mutex = &mutex ;
	t_arg.cond = &cond ;
	
	if (pthread_create( &thread_id, NULL, nag, (void *) &t_arg )) {
		perror("pthread_create") ;
		exit(-1) ;
	}
	
	printf(INSTRUCTIONS, QUIT_STRING) ;

	while ( fgets(buf,LINE_MAX,stdin) != NULL ) {
	
		buf[strlen(buf)-1] = '\0' ; 
		if (!strcmp(buf,QUIT_STRING) ) {
			break ;
		}

		pthread_mutex_lock(&mutex) ;
		{
			/* holding lock */
			t_arg.cur_count++ ;
			strcpy(t_arg.buf, buf) ;
			pthread_cond_signal(&cond) ;
		}
		pthread_mutex_unlock(&mutex) ;
	}
	
	pthread_mutex_lock(&mutex) ;
		t_arg.req_exit = 1 ; /* tell thread to exit */
		pthread_cond_signal(&cond) ;
	pthread_mutex_unlock(&mutex) ;
	
	pthread_exit(NULL) ; /* wait for the nag thread to exit */
	assert(0==1) ; /* never gets here */
}