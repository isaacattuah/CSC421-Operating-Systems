#include<linux/kernel.h>
#include<linux/syscalls.h>

/*
 * kernel/mysyscalls.c
 *
 * this code does in the kernel
 *
 * author: bjr
 * date: 28 sep 2020
 *
 */

#include "mysyscalls.h"

#define RINGBUF_SIZE 16

struct RingBuf {
	int head ;
	int tail ;
	int is_full ;
	char ringbuf[RINGBUF_SIZE] ;
} ;

struct RingBuf rb ;


asmlinkage long sys_my_syslog(const char * msg) {
	printk(KERN_ALERT "my_syslog: %s\n", msg) ;
	return 0 ;
}

asmlinkage long sys_rb_ioctl(unsigned int op) {
	switch (op) {
	case RB_OP_SIZE:
		return RINGBUF_SIZE ;
	case RB_OP_ISEMPTY:
		return (rb.head==rb.tail && !rb.is_full) ;
	case RB_OP_ISFULL:
		return rb.is_full ;
	case RB_OP_COUNT:
		if ( rb.is_full ) return RINGBUF_SIZE ;
		return ( rb.head-rb.tail+RINGBUF_SIZE) % RINGBUF_SIZE ;
	default:
		;
	}
	return 0 ;
}

asmlinkage long sys_rb_enqueue(char c) {
	if ( rb.is_full ) return -1 ;
	rb.ringbuf[rb.head] = (char) c ;
	rb.head = (rb.head+1) % RINGBUF_SIZE ;
	if (rb.head==rb.tail) rb.is_full = 1 ;
	return c ;
}

asmlinkage long sys_rb_dequeue(void) {
	int res ;
	if ( rb.head==rb.tail && !rb.is_full ) {
		return -1 ;
	}
	res = rb.ringbuf[rb.tail] ;
	rb.tail = (rb.tail+1) % RINGBUF_SIZE ;
	rb.is_full = 0 ;
	return res ;
}


static DEFINE_SEMAPHORE(lock_sema) ;
static struct semaphore wait_sema = __SEMAPHORE_INITIALIZER(wait_sema,0) ;
static int monitor_wait_count = 0 ;

asmlinkage long sys_my_monitor(unsigned int op) {
	int result = 0 ;
	int r ; 
	printk( KERN_INFO "sys_my_montor: entered\n") ;
	switch(op) {

	case MYMON_OP_ENTER:
		/* assume NOT holding lock_sema */
		r = down_interruptible(&lock_sema) ;
		break ; 

	case MYMON_OP_LEAVE:
		/* assume holding lock_sema */
		up(&lock_sema) ;
		break ; 

	case MYMON_OP_WAIT:
		/* assume holding lock_sema */
		monitor_wait_count ++ ;
		up(&lock_sema) ;
		r = down_interruptible(&wait_sema) ;
		r = down_interruptible(&lock_sema) ;
		break ; 

	case MYMON_OP_NOTIFY:
		/* assume holding lock_sema */
		if (monitor_wait_count>0) {
			monitor_wait_count-- ;
			up(&wait_sema) ;
		}
		break ; 

	case MYMON_OP_NOTIFY_ALL:
		/* assume holding lock_sema */
		while (monitor_wait_count>0) {
			monitor_wait_count-- ;
			up(&wait_sema) ;
		}
		break ;

	default:
		result = -1 ;
	}
	return result ;
}

