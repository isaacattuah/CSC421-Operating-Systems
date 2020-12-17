#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "mymonitor.h"
#include "mysyscalls.h"


/*
 * author: bjr
 * update: 29 sep 2020 -bjr pasted in man page
 */
 
/*

NAME
    my_monitor -- monitor syscall
    
SYNOPSIS
    my_monitor takes a single argument, the operation, and implements monitor semantics
    of enter, leave, wait, notify and notify-all. The monitor implements Mesa semantics
    with a single wait queue.

DESCRIPTION
    The single argument values are:
    
    Argument values:
        MONITOR_OP_ENTER  - enter the monitor
        MONITOR_OP_LEAVE  - leave the monitor
        MONITOR_OP_WAIT   - go into a wait while leaving the monitor
        MONITOR_OP_NOTIFY - notify a single thread waiting on in the monitor,
                            the notify is lost if there is no waiting thread. 
        MONITOR_OP_NOTIFY_ALL - notify all thread waiting on the monitor
	
    The monitor uses Mesa semantics. The notify and notify-all release one or all
    waiting threads, respectively, and the thread or threads contend for re-entry
    into the monitor.

    The caller of notify and notify-all remains in the monitor after the call to 
    notify or notify-all.
	
    The programmer must guarantee that leave, wait, notify and notify-all are only 
    called by a thread that is in the monitor. The monitor is not assured to be
    re-entrant: the thread in the monitor must not call enter.

HISTORY
    The kernel monitor project first appeared an project 3 in csc521.111 (2011-2012).

BUGS
    Reset does not work.
*/


extern int is_verbose ;

int my_monitor(int op) {
	switch (op) {
	case MONITOR_OP_RESET:
		return syscall(MY_MONITOR, MYMON_OP_RESET) ;
	case MONITOR_OP_ENTER:
		return syscall(MY_MONITOR, MYMON_OP_ENTER) ;
	case MONITOR_OP_LEAVE:
		return syscall(MY_MONITOR, MYMON_OP_LEAVE) ;
	case MONITOR_OP_WAIT:
		return syscall(MY_MONITOR, MYMON_OP_WAIT) ;
	case MONITOR_OP_NOTIFY:
		return syscall(MY_MONITOR, MYMON_OP_NOTIFY) ;
	case MONITOR_OP_NOTIFY_ALL:
		return syscall(MY_MONITOR, MYMON_OP_NOTIFY_ALL) ;
	default:
		return -1 ;
	}
	assert(0==1) ;
	return 0 ;
}

