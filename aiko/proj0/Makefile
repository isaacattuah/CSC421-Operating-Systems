#
# makefile for hello world project
# author: bjr
# date: 21 aug 2017
#

helloworld: helloworld.c
	cc -o helloworld helloworld.c

test: helloworld
	./helloworld > helloworld.out
	diff helloworld.out helloworld.ref

clean:
	-rm helloworld helloworld.out

commit:
	svn commit -m "commit for grade"

