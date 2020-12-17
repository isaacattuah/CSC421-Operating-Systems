#
# some curious makefile stanzas
# csc421 semester 211
# created: bjr 26 aug 2020
# last-update:
#	bjr 26 aug 2020
#

words: word-2
	@echo right!

word-1: word-3
	@echo -n The\ 

word-2: word-1
	@echo -n  price\ 

word-3: 
	@echo -n is\ 

count:
	@make count-1
	@make count-1
	@make count-1
	rm count.txt

count-1:
	@echo "      " >> count.txt
	@cat count.txt | wc -c 


program: hello
	./hello

hello.c: 
	@echo "#include<stdio.h>" > hello.c
	@echo "int main(){printf(\"hello\");}" >> hello.c

hello: hello.c
	cc -o hello hello.c


clean:
	-rm count.txt hello hello.c


