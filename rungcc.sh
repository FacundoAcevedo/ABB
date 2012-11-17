#!/bin/bash
OPCIONES="-std=c99 -Wall -pedantic -g"


gcc $OPCIONES -c abb.c tdas.c

gcc $OPCIONES abb.o tdas.o prueba3.c -o prueba3.bin
