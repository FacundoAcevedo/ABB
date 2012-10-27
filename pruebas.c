#include "abb.h"
#include "pila.h"
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

bool printear(const char* clave, void* dato, void* extra){
	printf("Clave: %s\nDato: %s\n\n", clave, (char*) dato);
	return true;
}

abb_comparar_clave_t cmp_ptr = &cmp;


abb_destruir_dato_t destruir_dato (void* dato){
	free (dato);
	return NULL;
}



void prueba_abb(void){
// DATOS ESTATICOS    
char *val1 = "pez espada";
char *val2 = "puerco";
char *val3 = "carpintero";
char *val4 = "ornitorrinco";
char *val5 = "oso hormiguero";
char *val6 = "ballena franca";
char *val7 = "colibri";
char *val8 = "perro callejero";
char *val9 = "marsupial";
char *val10 = "mariposa";
char *val11 = "pavo real";



// Creo un arbol vacio
abb_t* arbol0 = abb_crear(*cmp, NULL);
print_test("Prueba arbol0 vacio: ", arbol0);
print_test("Cantidad de arbol0 es 0: ", abb_cantidad(arbol0) == 0);

// Guardo clave 5, pez espada en la raiz de arbol0
print_test("Guardo clave 52, pez espada en la raiz de arbol0: ", abb_guardar(arbol0, "52", val1));
print_test("Cantidad de arbol0 es 1: ", abb_cantidad(arbol0) == 1);

// Guardo clave 2, puerco en arbol0. Debe guardarse en el hijo izquierdo de 5.
print_test("Guardo clave 2, puerco en arbol0: ", abb_guardar(arbol0, "2", val2));
print_test("Cantidad de arbol0 es 2: ", abb_cantidad(arbol0) == 2);
// Guardo clave 33, carpintero en arbol0. Debe guardarse en el hijo derecho de 2.
print_test("Guardo clave 33, carpintero en arbol0: ", abb_guardar(arbol0, "33", val3));
print_test("Cantidad de arbol0 es 3: ", abb_cantidad(arbol0) == 3);
// Guardo clave 99, ornitorrinco en arbol0. Debe guardarse en el hijo derecho de 5.
print_test("Guardo clave 99, ornitorrinco en arbol0: ", abb_guardar(arbol0, "99", val4));
print_test("Cantidad de arbol0 es 4: ", abb_cantidad(arbol0) == 4);
// Guardo clave 2, oso hormiguero en arbol0. Debe reemplazar a (2, puerco).
print_test("Reemplazo dato de clave 2 por oso hormiguero en arbol0: ", abb_guardar(arbol0, "2", val5));
// La cantidad debe haber permanecido constante.
print_test("Cantidad de arbol0 es 4: ", abb_cantidad(arbol0) == 4);

// Busco clave 2
print_test("Busco clave 2 con abb_obtener, devuelve oso hormiguero: ", val5 == abb_obtener(arbol0, "2"));
// Busco clave 111, no existe
print_test("Busco clave 111 con abb_obtener, devuelve NULL: ", NULL == abb_obtener(arbol0, "111"));
// Busco clave 33
print_test("Busco clave 33 con abb_pertenece, devuelve true: ", abb_pertenece(arbol0, "33"));
// Busco clave 10, no pertenece
print_test("Busco clave 10 con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "10"));


// Borrar
print_test("Cantidad de arbol0 antes de borrar es 4: ", abb_cantidad(arbol0) == 4);
print_test("Borro nodo hoja, clave 33, devuelve el dato carpintero: ", abb_borrar(arbol0, "33") == val3);
print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "33"));
print_test("Cantidad de arbol0 despues de borrar es 3: ", abb_cantidad(arbol0) == 3);
print_test("Cantidad de hojas arbol0 despues de borrar es 2: ", abb_contar_hojas(arbol0) == 2);

print_test("Guardo clave 30, ballena franca en arbol0: ", abb_guardar(arbol0, "30", val6));
print_test("Guardo clave -1, colibri en arbol0: ", abb_guardar(arbol0, "-1", val7));
print_test("Guardo clave -6, perro callejero en arbol0: ", abb_guardar(arbol0, "-6", val8));
print_test("Guardo clave 15, marsupial en arbol0: ", abb_guardar(arbol0, "15", val9));
print_test("Cantidad de hojas arbol0 es 3: ", abb_contar_hojas(arbol0) == 3);


print_test("Borro nodo con un solo hijo, clave -1, devuelve el dato colibri: ", abb_borrar(arbol0, "-1") == val7);
print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "-1"));
print_test("Cantidad de arbol0 despues de borrar es 6: ", abb_cantidad(arbol0) == 6);
print_test("Cantidad de hojas arbol0 despues de borrar es 3: ", abb_contar_hojas(arbol0) == 3);

print_test("Borro nodo con un solo hijo, clave 30, devuelve el dato ballena franca: ", abb_borrar(arbol0, "30") == val6);
print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "30"));
print_test("Cantidad de arbol0 despues de borrar es 5: ", abb_cantidad(arbol0) == 5);
print_test("Cantidad de hojas arbol0 despues de borrar es 3: ", abb_contar_hojas(arbol0) == 3);

print_test("Guardo clave -7, mariposa en arbol0: ", abb_guardar(arbol0, "-7", val10));
print_test("Guardo clave 0, pavo real en arbol0: ", abb_guardar(arbol0, "0", val11));
print_test("Cantidad de hojas arbol0 es 4: ", abb_contar_hojas(arbol0) == 4);

print_test("Borro nodo con un dos hijos, clave 2, devuelve el dato oso hormiguero: ", abb_borrar(arbol0, "2") == val5);
print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "2"));
print_test("Cantidad de arbol0 despues de borrar es 6: ", abb_cantidad(arbol0) == 6);
print_test("Cantidad de hojas arbol0 despues de borrar es 3: ", abb_contar_hojas(arbol0) == 3);

print_test("Borro nodo raiz con un dos hijos, clave 52, devuelve el dato pez espada: ", abb_borrar(arbol0, "52") == val1);
print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "52"));
print_test("Cantidad de arbol0 despues de borrar es 5: ", abb_cantidad(arbol0) == 5);
print_test("Cantidad de hojas arbol0 despues de borrar es 3: ", abb_contar_hojas(arbol0) == 2);

//~ abb_iter_t* iter = abb_iter_in_crear(arbol0);
//~ printf ("Hojas: %zu \n", abb_contar_hojas(arbol0));
abb_in_order(arbol0, printear, NULL);

//~ abb_iter_t* iter = abb_iter_in_crear(arbol0);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));
//~ abb_iter_in_avanzar(iter);
//~ printf("Actual: %s", abb_iter_in_ver_actual(iter));

abb_destruir(arbol0);
// Confirmo que no existe
print_test("Prueba arbol0 no existe: ", !arbol0);


}

int main(int argc, char **argv)
{
	prueba_abb();

	return 0;
}

