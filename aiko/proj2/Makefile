#
# Name: bjr
# Date: 5 sep 2016
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m
M= Makefile

COPTS=

build:
	make ringbuf

ringbuf: ringbuf.c ringbuf-sub.o ringbuf.h
	cc ${COPTS} -o $@ ringbuf-sub.o $<

ringbuf-sub.o: ringbuf-sub.c ringbuf.h
	cc ${COPTS} -c -o $@ $<
	
test: ringbuf
	-rm ringbuf.out
	./ringbuf +abcdefghijklmno+pq-rs--tuv+ > ringbuf.out
	diff ringbuf.out ringbuf.ref
	@echo "*** ${GREEN}PASSED the test ${NC}***"

mytest: ringbuf
	-rm ringbufmytest.out
	./ringbuf myteacherisburt+-yes+-----he--is > ringbufmytest.out
	diff ringbufmytest.out mytest.txt
	@echo "*** ${GREEN}ISAAC ATTUAH's test worked ${NC}***"

clean:
	-rm ringbuf ringbuf-sub.o ringbuf.out ringbufmytest.out

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


