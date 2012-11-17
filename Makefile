OPCIONES = -std=c99 -Wall -pedantic -g

all: pruebas

pruebas: pruebas.c abb.c
	gcc $(OPCIONES) -c abb.c tdas.c
	gcc $(OPCIONES) tdas.o abb.o pruebas.c -o pruebas
