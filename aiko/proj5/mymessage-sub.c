#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include <ctype.h>

#include "mysyscalls.h"
#include "mymonitor.h"
#include "ringbuf.h"
#include "mymessage.h"

extern int is_verbose ;

/*
 * author: Isaac Attuah 
 * pledge: this is my own work, unless otherwise noted
 * this is a template. change "author" and continue work
 *
 * update: 27 sep 2020 -bjr created
 */


int message_reader() {
  my_monitor(MONITOR_OP_ENTER);
  int val;
  while(1){
       if(rb_ioctl(RB_IS_EMPTY) == 1){
	   my_monitor(MONITOR_OP_WAIT);
       }
       else{
	 int val = rb_dequeue(); 
	 my_monitor(MONITOR_OP_NOTIFY_ALL);
	 my_monitor(MONITOR_OP_LEAVE);
	 return val ;
     }
  }
  return -1;
}

int message_writer(int m) {
  my_monitor(MONITOR_OP_ENTER);
  if (rb_ioctl(RB_IS_FULL) == 1){
    my_monitor(MONITOR_OP_WAIT);
}
  else{
    rb_enqueue(m);
    my_monitor(MONITOR_OP_NOTIFY_ALL);
    my_monitor(MONITOR_OP_LEAVE);
    return 0;
  }
    return -1;
}
	
int message_reader_mm(char * msg, int len) {
  my_monitor(MONITOR_OP_ENTER);
  int k = 0;
  int val;
  do {
    if(rb_ioctl(RB_IS_EMPTY)){
      my_monitor(MONITOR_OP_WAIT);
    } else {
      val = rb_dequeue();
      msg[k] = val;
      k++;
      my_monitor(MONITOR_OP_NOTIFY_ALL);
    }
  } while(val != 0);
  my_monitor(MONITOR_OP_LEAVE);
  return k ;
}

int message_writer_mm(char * msg) {
  if (rb_ioctl(RB_Q_SIZE) < strlen(msg) + 1){
    return -1;
  }
  my_monitor(MONITOR_OP_ENTER);
  int k = 0;
  int string_length = strlen(msg) + 1;
  while (k < string_length){
    if (rb_ioctl(RB_IS_FULL)){
      my_monitor(MONITOR_OP_WAIT);
    }
    else{
      rb_enqueue(msg[k]);
      k++;
      my_monitor(MONITOR_OP_NOTIFY_ALL);
      }
  }
  // rb_enqueue('\0');
  my_monitor(MONITOR_OP_LEAVE);
}


	
	
//  utility functions and unit tests

void print_nonprintable(char * s, int l) {
	char * t = malloc(((unsigned)(l+1))*sizeof(char)) ;
	int i ;
	assert (t) ;
	memcpy(t,s,l) ;
	for (i=0;i<l;i++) {
		if (!isprint(t[i])) t[i] = '.' ;
	}
	t[l] = '\0' ;
	printf("|%s|\n",t) ;
	free(t) ;
	return ;
}

