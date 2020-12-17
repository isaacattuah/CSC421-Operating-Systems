
/* 
 * author: bjr
 * last-update
 *    1 oct 2020: created
 *
 */


/*
 * the reader class waits for buffer not empty and dequeues and integer.
 * if the interger 0 is received, the thread exits.
 */

import java.util.Random;

class Reader implements Runnable {
	RingBuffer rb ;
	int ident ;
	Random prg = new Random() ;
	
	Reader(int ident, RingBuffer rb) { 
		this.ident = ident ;
		this.rb = rb ;
	}

	public void run() {
		int msg ;
		System.out.println("R"+ ident + " start") ;
		
		while (true) synchronized(rb) {
		
			if (!rb.is_empty()) {
				msg = rb.rb_dequeue() ;
				System.out.println("R" + ident+ " deq "+ msg) ;
				
				rb.notifyAll() ;
				
				// receive stop messages
				if (msg==0) {
					System.out.println("R" + ident+ " exits") ;
					return ;
				}
			}
			else try {
				rb.wait() ;
			}
			catch (InterruptedException e)  {
				System.out.println("R"+ident+" interrupted") ;
			}		
		}
	}
}
