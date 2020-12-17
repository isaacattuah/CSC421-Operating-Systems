#
# author: bjr
# last revision: 27 sep 2020 -bjr created 
#		
#


all:
	make mymessage
	
mymessage: mymessage.c mymessage-sub.o ringbuf-sub.o mymonitor-sub.o mymessage.h
	cc -o $@ $< mymessage-sub.o ringbuf-sub.o mymonitor-sub.o

mymessage-sub.o: mymessage-sub.c mymessage.h mysyscalls.h ringbuf.h mymonitor.h
	cc -c $<

mymonitor-sub.o: mymonitor-sub.c mymonitor.h mysyscalls.h
	cc -c $<
	
ringbuf-sub.o: ringbuf-sub.c ringbuf.h mysyscalls.h
	cc -c $<
	
run-reader: mymessage
	./mymessage -vr

run-writer: mymessage
	./mymessage -vs1 abc12012

run-writer-3: mymessage
	(./mymessage -v -s1 abcdefghiklmopqrstuvw &);(./mymessage -v -s2 ABCDEFGHIKLM &);./mymessage -v -s3 1234567890;echo "done!"

run-reader-mm: mymessage
	./mymessage -vrm -n40

run-writer-mm: mymessage
	./mymessage -vm hello.world.a.0

run-writer-mm-3: mymessage
	(./mymessage -vvm -s1 rouge.blue.vert.blanc.noir.jaune.gris.marron.rose &);(./mymessage -vmv -s2 rojo.azul.verde.blanco.negroo&);./mymessage -mvv -s3 11.10.9.8.7.6.5.4.3.2.1.0;echo "done!"


simple-test: mymessage
	./mymessage -v
	./mymessage -vr this.should.not.be.here
	
clean:
	-rm  mymessage mymessage-sub.o mymessage-sub mymonitor-sub.o ringbuf-sub.o



