.PHONY: all
.PHONY: clean

all: linux

.PHONY: linux
linux:
	make -f Makefile.lin

.PHONY: debug
debug:
	make -f Makefile.debug
clean:
	rm -f lbuild/*
	rm -f padraGB
	rmdir lbuild
