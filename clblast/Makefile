CFLAGS=-IC:/OpenCL/include -IC:/OpenCL/CLBlast-1.6.1-windows-x64/include
CPPFLAGS=-IC:/OpenCL/include -IC:/OpenCL/CLBlast-1.6.1-windows-x64/include
LIBS=C:\OpenCL\lib\OpenCL.lib C:\OpenCL\CLBlast-1.6.1-windows-x64\lib\clblast.lib

all: cfloat.exe ccomplex.exe cppcomplex.exe

cfloat.exe : cfloat.obj
	cl cfloat.obj $(LIBS)

ccomplex.exe : ccomplex.obj
	cl ccomplex.obj $(LIBS)

cppcomplex.exe : cppcomplex.obj
	cl cppcomplex.obj $(LIBS)

cfloat.obj : cfloat.c

ccomplex.obj : ccomplex.c

cppcomplex.obj : cppcomplex.cpp

test:
	cfloat.exe
	ccomplex.exe
	cppcomplex.exe
