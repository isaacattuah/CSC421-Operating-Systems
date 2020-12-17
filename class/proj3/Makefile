#
# makefile for proj3
# csc421-211
# last update: 
#   10 sep 2020 bjr- created
#

MF= -f Makefile

RED= \033[0;31m
NC=  \033[0m

all:
	make ${MF} uname-out

uname-out:
	uname -r > uname.out
	@echo "${RED}***\n\tplease now add and commit the file uname.out\n***${NC}"

clean:
	-rm uname.out
