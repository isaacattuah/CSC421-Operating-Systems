#
# makefile for leveldb project
# author: bjr
# create: 9 nov 2019
# last update: 19 oct 2020
#

MNT= store
STOR= kv-tables

MF= -f Makefile
L0= l0.dat

RED= \033[0;31m
GREEN= \033[0;32m
NC= \033[0m
DOT=  | head -2 | tail -n +2
NODOT= | tail -n +2


all:
	@echo "make leveldbd	:builds all"
	@echo "make run-d		:starts deamon"
	@echo "make clean	:cleans all"
	@echo "make test	:runs basic test"
	

leveldbd: leveldbd.c leveldb-sub.o leveldb.h
	cc $< `pkg-config fuse --cflags --libs` -o $@ leveldb-sub.o

leveldb-sub.o: leveldb-sub.c leveldb.h 
	cc `pkg-config fuse --cflags --libs` -o $@ -c $<
	

${L0}:
	-rm -rf ${STOR}
	mkdir ${STOR}
	dd if=/dev/zero of=${STOR}/${L0} bs=1 count=16384

run-d: leveldbd ${L0}
	-fusermount -u ${MNT}
	-mkdir ${MNT} ${STOR}
	./leveldbd -v ${MNT} ${STOR}

test:
	@echo "test if leveldb is running"
	ls ${MNT}
	@echo "***\n*** ${RED}key-value pair (hello:world) ${NC}\n***"
	echo -n "hello" > ${MNT}/world
	ls -l ${MNT}/world
	cat ${MNT}/world > basic-test.out
	@echo >> basic-test.out
	rm ${MNT}/world
	ls -l ${MNT}/world
	@echo "***\n*** ${RED}four key-value pairs ${NC}\n***"
	echo -n "test " > ${MNT}/d
	echo -n "is " > ${MNT}/b
	echo -n "this " > ${MNT}/a
	echo -n "a " > ${MNT}/c
	ls -l ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a
	cat ${MNT}/a ${MNT}/b ${MNT}/c ${MNT}/d >> basic-test.out
	@echo >> basic-test.out
	echo -n "test " > ${MNT}/a
	echo -n "was " > ${MNT}/c
	echo -n "this " > ${MNT}/b
	echo -n "a " > ${MNT}/d
	ls -l ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a
	cat ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a >> basic-test.out
	@echo >> basic-test.out
	rm ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a
	ls -l ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a
	cat ${MNT}/b ${MNT}/c ${MNT}/d ${MNT}/a >> basic-test.out
	@echo >> basic-test.out
	hexdump -C ${STOR}/${L0} >> basic-test.out
	diff basic-test.out basic-test.ref
	@echo "***\n*** ${GREEN}passed ${NC}\n***"


installfuse:
	sudo apt-get install libfuse-dev
	sudo apt-get install pkg-config

clean:
	-fusermount -u ${MNT}
	-rm leveldbd leveldb-sub.o basic-test.out
	-rmdir ${MNT}
	-rm -rf ${STOR}

