#
# quiz4 makefile
# csc421 211
# author: Isaac Attuah
# last modified: 9/16/2020
#
#


quiz4: quiz4.c quiz4-sub.o quiz4.h
	cc -o $@ quiz4-sub.o $<

quiz4-sub.o: quiz4-sub.c quiz4.h
	cc -c -o $@ $<

test: quiz4
	./quiz4 now is the time for all good people
	./quiz4
	./quiz4 -v

mytest:	quiz4
	./quiz4 here comes the sun doodoodoodoo
	./quiz4 -v time may change me but I can\'t change time
	./quiz4 time may change me but I can\'t change time

clean:
	-rm quiz4 quiz4-sub.o

