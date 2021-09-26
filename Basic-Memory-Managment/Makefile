include Makefile.inc

all:
	$(GCC) $(GCCFLAGS) -c freeMgr.c -o freeMgr.o
	$(GCC) $(GCCFLAGS) -c main.c -o main.o
	$(GCC) $(GCCFLAGS) freeMgr.o main.o -o main.out


clean:
	@rm *.o *.out