#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"

/*
 * author: 
 * date: 
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 *
 * update: 
 */


/* static variables (zeroed) */

int g_debug = 0 ;
int is_verbose = 0 ;
struct RingBuf rb ;
int diff;

int rb_ioctl(int op) {
	switch (op) {
	case RB_Q_SIZE:
		return RINGBUF_SIZE ;
	case RB_IS_EMPTY:
	        if(rb.is_full == 0 && rb.head == rb.tail)
		  return 1 ;
		else 
		  return 0;
	case RB_IS_FULL:
	  return rb.is_full;
     
	case RB_Q_COUNT:
	  diff = rb.head - rb.tail;
	  if (diff > 0){
	    return diff;
	  }
	  if (diff < 0){
	    return RINGBUF_SIZE + diff;
	  }
	  if(rb.is_full)
                 return RINGBUF_SIZE ;
	default:
		;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
  if (!rb.is_full) {
    rb.ringbuf[rb.head] = ele ;
    rb.head = (rb.head + 1) % RINGBUF_SIZE ;
    if (rb.head == rb.tail) {
      rb.is_full = 1 ;
    }
    return ele;
  }
  return -1 ;
}

int rb_dequeue(void) {
  if (!rb_ioctl(RB_IS_EMPTY)) {
    char ele = rb.ringbuf[rb.tail] ;
    rb.tail = (rb.tail + 1) % RINGBUF_SIZE ;
    rb.is_full = 0 ;
    return ele ;
  }
  return -1 ;
}

