
/*
 * name: burt
 * date: sep 2016
 *
 * header file for ringbuf.c and ringbuf-sub.h 
 *
 */

/* globals shared between ringbuf.c and ringbuf-sub.c */

extern int g_debug ;
extern int is_verbose ;

/* the ringbuf "API" */

#define RB_Q_SIZE 0
#define RB_IS_EMPTY 1
#define RB_IS_FULL 2
#define RB_Q_COUNT 3

int rb_ioctl(int op) ; 
int rb_enqueue(int ele) ;
int rb_dequeue(void) ;

