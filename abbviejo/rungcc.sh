#!/bin/bash
OPCIONES="-std=c99 -Wall -pedantic -g"


gcc $OPCIONES -c abb.c tdas.c

gcc $OPCIONES abb.o tdas.o pruebas_martin.c -o pruebas_martin.bin
