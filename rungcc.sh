#!/bin/bash
OPCIONES="-std=c99 -Wall -pedantic -g"


gcc $OPCIONES -c abb.c pila.c

gcc $OPCIONES abb.o pila.o pruebas.c -o pruebas.bin
