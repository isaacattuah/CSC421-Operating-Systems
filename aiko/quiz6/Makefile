#
# Makefile for fuse hello world example
# author: bjr
# created: november 2014
# updated: 
#     nov 8 2015 updated for csc421.161
#     dec 4 2015 updated for stage1 hints
#     dec 17 2015 updated for stage2 hints
#     nov 7 2016
#     oct 19 2020 
#

FUSE_OPTS= $(shell pkg-config fuse --cflags --libs)

all:
	@echo "*** Do not forget |make installfuse|, if there are problems"

hello: hello.c
	gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello

mnt:
	-mkdir mnt

test: hello mnt
	./hello mnt
	ls -la mnt
	cat mnt/hello
	fusermount -u mnt

clean:
	-rm hello
	-rmdir mnt
	@echo sometimes: sudo umount -l ${MT}

installfuse:
	sudo apt-get install libfuse-dev
	sudo apt-get install pkg-config

