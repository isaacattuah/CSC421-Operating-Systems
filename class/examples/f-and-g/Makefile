all: fg gf
	./fg
	./gf

f.o: f.c
	cc -fPIC --shared -c $<

g.o: g.c
	cc -fPIC -c $<

fg: fg.c f.o g.o
	cc -o $@ $< f.o g.o

gf: fg.c g.o f.o 
	cc -o $@ $< g.o f.o

nm:
	nm f.o g.o fg gf

objdump:
	objdump --disassemble-all f.o > objdump.f.out
	objdump --disassemble-all g.o > objdump.g.out
	objdump --disassemble-all fg > objdump.fg.out

clean:
	-rm fg gf *.o *.out
