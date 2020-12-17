
/* 
 * author: bjr
 * last-update
 *    1 oct 2020: created
 *
 */

/*
 * reader-writer demonstration program.
 *
 * a ring buffer is instantiated and shared with multiple readers and writers
 * .
 * the readers read when there is something to read, else they wait;
 *
 * the writers write when there is room to write, else they wait.
 *
 * readers and writers both notify-all when they read or write, to signal
 * to their opposite to re-attempt their action.
 *
 * writers send zeros to readers to have them exit. the protocol here is that
 * writers except writer 0 send one zero; and writer 0 sends the necessary 
 * balance to get all readers to exit.
 *
 */

import java.util.Random;

class ReaderWriter {

	static Thread [] make_readers(RingBuffer rb, int n) {
		Thread [] t = new Thread[n] ;
		for (int i = 0; i<n; i++) {
			t[i] = new Thread(new Reader(i,rb)) ;
		}
		return t ;
	}
	
	static Thread [] make_writers(RingBuffer rb, int n_w, int n_r, int n_m) {
		Thread [] t = new Thread[n_w] ;
		int m = n_m/n_w ;
		int w_zero_stops = n_r - n_w + 1 ;
		
		for (int i=0;i<n_w;i++) {
			int stopping = 1 ;
			if (i==0) stopping = w_zero_stops ;
			t[i] = new Thread(new Writer(rb,i,stopping,m)) ;
		}
		return t ;
	}

	static void fire_it_up(RingBuffer rb, int n_readers, int n_writers, int n_messages) {
		Thread [] t_readers ;
		Thread [] t_writers ;

		t_readers = make_readers(rb,n_readers) ;
		t_writers = make_writers(rb, n_writers, n_readers, n_messages ) ;
		
		for (int i = 0; i<t_readers.length; i++) {
			t_readers[i].start() ;
		}

		for (int i = 0; i<t_writers.length; i++) {
			t_writers[i].start() ;
		}

		// wait for the threads to exit
		try {
			for (int i = 0; i<t_readers.length; i++) {
				t_readers[i].join() ;
			}
			for (int i = 0; i<t_writers.length; i++) {
				//while (t_writers[i].isAlive()) ;
				t_writers[i].join() ;
			}
		}
		catch (InterruptedException e)  {
		}
		
		
	}
	
	public static void main(String [] args) {
		RingBuffer rb = new RingBuffer(1) ;
		int n_readers ;
		int n_writers  ;
		int n_messages  ;
	
		n_writers = 1 ;
		n_readers = 4 ;
		n_messages = 50 ; 
		 
		System.out.println("") ;
		System.out.println("Number of writers: "+n_writers) ;
		System.out.println("Number of readers: "+n_readers) ;
		fire_it_up( rb, n_readers, n_writers, n_messages ) ;
		
		n_writers = 2 ;
		n_readers = 4 ;
		n_messages = 50 ; 
		
		System.out.println("") ;
		System.out.println("Number of writers: "+n_writers) ;
		System.out.println("Number of readers: "+n_readers) ;
		fire_it_up( rb, n_readers, n_writers, n_messages ) ;
	
	}

}