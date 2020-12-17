#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"
#include "mysyscalls.h"


/*
 * author: bjr 
 * last-update: 28 sep 2020 -bjr: added man page 
 */

/*
NAME
    rb_ioctl, rb_enqueue, rb_dequeue -- ring buffer syscalls
    ringbuf -- command line interface to the rb_* system calls
    
SYNOPSIS
    rb_enqueue enqueues characters to a single, in-kernel ring buffer; re_dequeue 
    dequeues an enqueued character. rb_ioctl implements the control functions return
    if the queue is empty or full, depending on the control argument.

DESCRIPTION
    The rb_enqueue syscall takes a single character argument and returns the argument
    if the character can be enqueued, -1 else. For instance, it will return -1 if the
    ring buffer is full. 
    
    The rb_dequeue syscall takes no arguments are returns a single integer value, which
    is the dequeued character, -1 else. For instance, if will return -1 if the ring 
    buffer is empty.
    
    The rb_ioctl takes an integer argument giving the operation, as follows:
    
    Argument value:
        RB_Q_SIZE   - return the size of the ring buffer.
    	RB_IS_EMPTY - return 1 if ring buffer is empty, 0 else.
    	RB_IS_FULL  - return 1 if ring buffer is full, 0 else.
    	RB_Q_COUNT  - return the current number of characters in the ring buffer
	
    On error, or an invalid argument value, return -1.
	
    The ring buffer is of fixed size. The kernel needs to be rebuilt if the ring buffer 
    is to be of a different size. For this project, the size is 16.
    

HISTORY
    Introduced as Project 2 in CSC 421 session 151.

BUGS
    The code might not be thread safe.

*/

extern int is_verbose ;

int rb_ioctl(int op) {
	switch (op) {
	case RB_Q_SIZE:
		return syscall(RB_IOCTL, RB_OP_SIZE ) ;
	case RB_IS_EMPTY:
		return syscall(RB_IOCTL, RB_OP_ISEMPTY ) ;
	case RB_IS_FULL:
		return syscall(RB_IOCTL, RB_OP_ISFULL ) ;
	case RB_Q_COUNT:
		return syscall(RB_IOCTL, RB_OP_COUNT ) ;
	default:
		;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
	return syscall(RB_ENQUEUE, (char) ele ) ;
}

int rb_dequeue(void) {
	return syscall(RB_DEQUEUE) ;
}

