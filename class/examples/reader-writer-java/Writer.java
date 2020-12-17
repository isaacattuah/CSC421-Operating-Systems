
/* 
 * author: bjr
 * last-update
 *    1 oct 2020: created
 *
 */

import java.util.Random;


/*
 * the writer class waits on buffer full and when not, adds enqueues and 
 * integer. implements a special 0 message to cause reader threads to exit.
 *
 */
 

class Writer implements Runnable {
	RingBuffer rb ;
	int n_readers ;
	Random prg = new Random() ;
	int n_messages = 100 ; 
	int ident ;
	
	Writer(RingBuffer rb, int ident, int n_readers, int n_messages ) {
		this.n_readers = n_readers ;
		this.ident = ident ;
		this.rb = rb ;
		this.n_messages = n_messages ;
	}
	
	public void run() {
		Random prg = new Random() ;
		int msg = 0 ;
		int i ; 

		System.out.println("W"+ident+" start") ;

		while (n_readers>0) synchronized(rb) {
			if (!rb.is_full()) {
				msg ++ ;
				i = msg ;
				
				// send stop messages 
				if (msg>n_messages){
					i = 0 ;
					n_readers-- ;
				}

				rb.rb_enqueue(i) ;
				System.out.println("W"+ident+" enq "+ i) ;
				rb.notifyAll() ;
			}
			else try {
				rb.wait() ;
			}
			catch (InterruptedException e)  {
				System.out.println("W"+ident+" interrupted") ;
			}
		}		
	}

}
