
/*
 * name: burt
 * last update: sep 2020
 *
 * header file for mymessage.c and mymessage-sub.c
 *
 */

/* globals shared */
extern int is_verbose ;

/* the message "API" */

int message_reader(void) ;
int message_writer(int m) ;
int message_reader_mm(char * msg, int len) ;
int message_writer_mm(char * msg) ;

void print_nonprintable(char * s, int l) ;
