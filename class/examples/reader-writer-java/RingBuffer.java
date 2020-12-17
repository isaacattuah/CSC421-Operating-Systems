
/* 
 * author: bjr
 * last-update
 *    1 oct 2020: created
 *
 */

 
/*
 * implements a ring buffer of integers of size determined at instantiation.
 */

class RingBuffer {

	int[] ring_buf ; 
	int head = 0 ;
	int tail = 0 ;
	int rb_size = 0 ;
	boolean full_f = false ;
	
	RingBuffer(int size) {
		rb_size = size ;
	 	ring_buf = new int[rb_size] ;
	}
	
	boolean is_empty() {
		if (head==tail && !full_f) return true ;
		return false ;
	}
	
	boolean is_full() {
		return full_f ;
	}

	int get_rb_size() {
		return rb_size ;
	}

	int rb_enqueue(int c){
		if (full_f) return -1 ;
		ring_buf[head] = c ;
		head = (head+1)%rb_size ;
		if (head==tail) full_f = true ;
		return c ;
	}
	
	int rb_dequeue(){
		if (is_empty()) return -1 ;
		int c = ring_buf[tail] ;
		tail = (tail+1)%rb_size ;
		full_f = false ;
		return c ;
	}
}
