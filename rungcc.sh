#!/bin/bash
OPCIONES="-std=c99 -Wall -pedantic -g"


gcc $OPCIONES -c abb.c

gcc $OPCIONES abb.o pruebas.c -o pruebas.bin
