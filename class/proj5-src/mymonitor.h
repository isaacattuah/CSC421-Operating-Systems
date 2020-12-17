
/*
 * name: burt
 * date: oct 2016
 *
 * header file for mymonitor.c and mymonitor-sub.h 
 *
 */

/* globals shared between ringbuf.c and ringbuf-sub.c */

extern int is_verbose ;

/* the ringbuf "API" */

#define MONITOR_OP_RESET 0
#define MONITOR_OP_ENTER 1
#define MONITOR_OP_LEAVE 2
#define MONITOR_OP_WAIT 3
#define MONITOR_OP_NOTIFY 4
#define MONITOR_OP_NOTIFY_ALL 5

int my_monitor(int op) ;

