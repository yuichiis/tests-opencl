#CFLAGS=-IC:/OpenCL/include -IC:/OpenCL/CLBlast-1.6.1-windows-x64/include
#CPPFLAGS=-IC:/OpenCL/include -IC:/OpenCL/CLBlast-1.6.1-windows-x64/include
LIBS=-lOpenCL -lclblast

all: cfloat ccomplex cppcomplex

cfloat : cfloat.o
	$(CC) cfloat.o $(LIBS) -o $@

ccomplex : ccomplex.o
	$(CC) ccomplex.o $(LIBS) -o $@

cppcomplex : cppcomplex.o
	$(CC) cppcomplex.o $(LIBS) -o $@

cfloat.o : cfloat.c

ccomplex.o : ccomplex.c

cppcomplex.o : cppcomplex.cpp

test :
	./cfloat
	./ccomplex
	./cppcomplex
    
clean: 
	rm -f *.o
	rm -f cfloat ccomplex cppcomplex
	rm -f *.out
