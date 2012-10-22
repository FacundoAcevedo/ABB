#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/* ******************************************************************
 *                        PRUEBAS UNITARIAS 
 * *****************************************************************/

/* Funcion auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int cmp (const char* clave_vieja, const char* clave_nueva){
	// Si las claves son iguales, devuelve 0
	int vieja = atoi(clave_vieja);
	int nueva = atoi(clave_nueva);
	if (vieja == nueva) return 0;
	// Si clave nueva es mayor a clave vieja, return 1
	else if (nueva > vieja) return 1;
	// Si clave nueva es menor a clave vieja, return -1
	else return (-1);
}

abb_comparar_clave_t cmp_ptr = &cmp;


abb_destruir_dato_t destruir_dato (void* dato){
	free (dato);
	return NULL;
}
size_t abb_contar_hojas_r(abb_nodo_t** nodo, abb_t* arbol){
	if (!nodo) return 0;
	abb_nodo_t* izq = (*nodo)->izq;
	abb_nodo_t* der = (*nodo)->der;
	if (!izq && !der) return 1;
	return abb_contar_hojas_r(izq) + abb_contar_hojas_r(der);
}

size_t abb_contar_hojas(abb_t* arbol){
	if (!arbol) return 0;
	return abb_contar_hojas_r(arbol->raiz, arbol);
}


void prueba_abb(void){
// DATOS ESTATICOS    
char *val1 = "pez espada";
char *val2 = "puerco";
char *val3 = "carpintero";
char *val4 = "ornitorrinco";
//~ char *val5 = "zorro";



// Creo un arbol vacio
abb_t* arbol0 = abb_crear(*cmp, NULL);
print_test("Prueba arbol0 vacio: ", arbol0);
print_test("Cantidad de arbol0 es 0: ", abb_cantidad(arbol0) == 0);
print_test("La raiz de arbol0 es NULL: ", !abb_ver_raiz(arbol0));
print_test("Clave de raiz de arbol0 es NULL: ", ver_clave(abb_ver_raiz(arbol0)) == NULL);

// Guardo clave 5, pez espada en la raiz de arbol0
print_test("Guardo clave 52, pez espada en la raiz de arbol0: ", abb_guardar(arbol0, "52", val1));
print_test("Cantidad de arbol0 es 1: ", abb_cantidad(arbol0) == 1);
print_test("Clave de raiz de arbol0 es 52: ", ver_clave(abb_ver_raiz(arbol0)) == (const char*) "52");
print_test("Dato de raiz de arbol0 es pez espada: ", ver_dato(abb_ver_raiz(arbol0)) == val1);

print_test("Guardo clave 2, puerco en arbol0: ", abb_guardar(arbol0, "2", val2));
print_test("Cantidad de arbol0 es 2: ", abb_cantidad(arbol0) == 2);
print_test("Guardo clave 33, carpintero en arbol0: ", abb_guardar(arbol0, "33", val3));
print_test("Cantidad de arbol0 es 3: ", abb_cantidad(arbol0) == 3);
print_test("Guardo clave 99, ornitorrinco en arbol0: ", abb_guardar(arbol0, "99", val4));
print_test("Cantidad de arbol0 es 4: ", abb_cantidad(arbol0) == 4);

printf ("Hojas: %zu ", abb_contar_hojas(arbol0));
}

int main(int argc, char **argv)
{
	prueba_abb();

	return 0;
}

