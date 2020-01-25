# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= practica_objetos_B4

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuración
#
CFLAGS=    -g -DXWINDOWS -DSHM -I/usr/include -I.
CXXFLAGS=  -g -DXWINDOWS -DSHM -I/usr/include -I.

# banderas pra el enlazador
# Se indica directorios donde encontrar las funciontecas con -L. Usando -l seguido del
# nombre se indica la funcionteca que se desea enlazar.
#
LDFLAGS=  -lGL -lGLU -lglut -lsupc++ -lm -lc -lstdc++ -lpthread -L/usr/lib/nvidia-331

# definicion del compilador
#
CC = g++

# orden que sera ejecutada por defecto
#
default : $(TARGETS) run

# regla de ejecucion
#
$(TARGETS) : file_ply_stl.o objetos_B4.o practica_objetos_B4.o
	$(CC) -o $@ $(CXXFLAGS) file_ply_stl.o objetos_B4.o practica_objetos_B4.o $(LDFLAGS)

#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)

#
#
redo:
	touch *.cc
	make

#
#
superclean:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)

#
#
tgz:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)
	tar -zcvf $(TARGETS).tgz *

#
#
run:
	./practica_objetos_B4 beethoven
