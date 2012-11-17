#include "abb.h"
#include "tdas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


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


bool printear_dato_est(const char* clave, void* dato, void* extra){
	printf("Clave: %s\nDato: %s\n\n", clave, (char*) dato);
	return true;
}

bool printear(const char* clave, void* lista, void* extra){
	printf("Clave: %s\n Datos en lista: ", clave);
	lista_iter_t* iter_lista = lista_iter_crear(lista);
	int i =0;
	while (i <lista_largo(lista)){
		printf("%s ", (char*)lista_iter_ver_actual(iter_lista));
		i++;
	}
	puts("\n");
	lista_iter_destruir(iter_lista);
	return true;
}

void destruir_lista (void* lista){
		lista_destruir(lista, NULL);
	return;
}

abb_destruir_dato_t destruir_dato_lista = &destruir_lista;

void destruir_cola (void* cola){
	cola_destruir(cola, NULL);
		return;
	}

abb_destruir_dato_t destruir_dato_cola = &destruir_cola;

size_t abb_contar_hojas(abb_t* arbol);  




/* PRUEBA_ABB */
void prueba_abb(void){
	
	//~ 
//~ // DATOS ESTATICOS    
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

// Datos dinámicos tipo lista
lista_t* val12 = lista_crear();
lista_t* val13 = lista_crear();
lista_t* val14 = lista_crear();
lista_t* val15 = lista_crear();
lista_t* val16 = lista_crear();
lista_t* val17 = lista_crear();
lista_t* val18 = lista_crear();
lista_t* val19 = lista_crear();
lista_t* val20 = lista_crear();
lista_t* val21 = lista_crear();
lista_t* val22 = lista_crear();


lista_insertar_ultimo(val12, "a");
lista_insertar_ultimo(val12, "b");
lista_insertar_ultimo(val12, "c");
lista_insertar_ultimo(val13, "d");
lista_insertar_ultimo(val13, "e");
lista_insertar_ultimo(val14, "f");
lista_insertar_ultimo(val15, "g");
lista_insertar_ultimo(val16, "h");
lista_insertar_ultimo(val17, "i");
lista_insertar_ultimo(val17, "j");
lista_insertar_ultimo(val18, "k");
lista_insertar_ultimo(val19, "l");
lista_insertar_ultimo(val20, "m");
lista_insertar_ultimo(val22, "n");
lista_insertar_ultimo(val22, "o");

	
	// Creo un arbol vacio
	abb_t* arbol0 = abb_crear(*cmp, NULL);
	print_test("Prueba arbol0 vacio: ", arbol0);
	print_test("Cantidad de arbol0 es 0: ", abb_cantidad(arbol0) == 0);

	// Guardo clave 5, pez espada en la raiz de arbol0
	print_test("Guardo clave 52, pez espada en la raiz de arbol0: ", abb_guardar(arbol0, "52", val1));
	print_test("El 52 pertenece", abb_pertenece(arbol0, "52"));
	print_test("Cantidad de arbol0 es 1: ", abb_cantidad(arbol0) == 1);

	// Guardo clave 2, puerco en arbol0. Debe guardarse en el hijo izquierdo de 5.
	print_test("Guardo clave 2, puerco en arbol0: ", abb_guardar(arbol0, "2", val2));
	print_test("El 2 pertenece", abb_pertenece(arbol0, "2"));
	print_test("Cantidad de arbol0 es 2: ", abb_cantidad(arbol0) == 2);
	// Guardo clave 33, carpintero en arbol0. Debe guardarse en el hijo derecho de 2.
	print_test("Guardo clave 33, carpintero en arbol0: ", abb_guardar(arbol0, "33", val3));
	print_test("El 33 pertenece", abb_pertenece(arbol0, "33"));
	print_test("Cantidad de arbol0 es 3: ", abb_cantidad(arbol0) == 3);
	// Guardo clave 99, ornitorrinco en arbol0. Debe guardarse en el hijo derecho de 5.
	print_test("Guardo clave 99, ornitorrinco en arbol0: ", abb_guardar(arbol0, "99", val4));
	print_test("El 99 pertenece", abb_pertenece(arbol0, "99"));
	print_test("Cantidad de arbol0 es 4: ", abb_cantidad(arbol0) == 4);
	// Guardo clave 2, oso hormiguero en arbol0. Debe reemplazar a (2, puerco).
	print_test("Reemplazo dato de clave 2 por oso hormiguero en arbol0: ", abb_guardar(arbol0, "2", val5));
	// La cantidad debe haber permanecido constante.
	print_test("Cantidad de arbol0 es 4: ", abb_cantidad(arbol0) == 4);
//~ 
	//~ // Busco clave 2
	print_test("Busco clave 2 con abb_obtener, devuelve oso hormiguero: ", val5 == abb_obtener(arbol0, "2"));
	//~ // Busco clave 111, no existe
	print_test("Busco clave 111 con abb_obtener, devuelve NULL: ", NULL == abb_obtener(arbol0, "111"));
	//~ // Busco clave 33
	print_test("Busco clave 33 con abb_pertenece, devuelve true: ", abb_pertenece(arbol0, "33"));
	puts("");
	//~ // Busco clave 10, no pertenece
	print_test("Busco clave 10 con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "10"));
//~ 
//~ 
	// Borrar
	print_test("Cantidad de arbol0 antes de borrar es 4: ", abb_cantidad(arbol0) == 4);
	print_test("Borro nodo hoja, clave 33, devuelve el dato carpintero: ", abb_borrar(arbol0, "33") == val3);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol0, "33"));
	print_test("Cantidad de arbol0 despues de borrar es 3: ", abb_cantidad(arbol0) == 3);
	print_test("Cantidad de hojas arbol0 despues de borrar es 2: ", abb_contar_hojas(arbol0) == 2);
//~ 
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
	//~ 
	abb_t* arbol3 = abb_crear(cmp, NULL);
	print_test("Guardo clave -7, mariposa en arbol3: ", abb_guardar(arbol3, "-7", val10));
	print_test("Busco clave -7 con abb_obtener, devuelve mariposa: ", val10 == abb_obtener(arbol3, "-7"));
	print_test("Borro nodo raiz sin hijos, clave -7, devuelve el dato mariposa: ", abb_borrar(arbol3, "-7") == val10);
	
	abb_destruir(arbol3);

	// Iterador externo
	printf("Iterador inorder con función de impresión: \n");
	abb_in_order(arbol0, printear_dato_est, NULL);

	// Iterador interno
	// Con arbol no vacio
	abb_iter_t* iter = abb_iter_in_crear(arbol0);
	print_test("Iterador interno (al final es false): ", !abb_iter_in_al_final(iter));
	const char* clave = abb_iter_in_ver_actual(iter);
	int clave_int = atoi(clave);
	print_test("Iterador interno (-7): ", clave_int == -7);
	abb_iter_in_avanzar(iter);
	clave = abb_iter_in_ver_actual(iter);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (-6)): ", clave_int == -6);
	abb_iter_in_avanzar(iter);
	clave = abb_iter_in_ver_actual(iter);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (0)): ",  clave_int == 0);
	print_test("Iterador interno (al final es false): ", !abb_iter_in_al_final(iter));
	abb_iter_in_avanzar(iter);
	clave = abb_iter_in_ver_actual(iter);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (15)): ",  clave_int == 15);
	abb_iter_in_avanzar(iter);
	clave = abb_iter_in_ver_actual(iter);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (99)): ",  clave_int == 99);
	abb_iter_in_avanzar(iter);
	print_test("Iterador interno (avanzar, al final): ", !abb_iter_in_ver_actual(iter));
	print_test("Iterador interno (al final es true): ", abb_iter_in_al_final(iter));
	abb_iter_in_destruir(iter);
	// Con arbol vacio
	abb_t* arbol1 = abb_crear(cmp, NULL);
	abb_iter_t* iter1 = abb_iter_in_crear(arbol1);
	print_test("Iterador interno, arbol vacio (ver actual es NULL): ", !abb_iter_in_ver_actual(iter1));
	print_test("Iterador interno, arbol vacio (al final es true): ", abb_iter_in_al_final(iter1));
	print_test("Iterador interno, arbol vacio (avanzar es false): ", !abb_iter_in_avanzar(iter1));
	abb_iter_in_destruir(iter1);


	// Destruir arbol
	abb_destruir(arbol0);
	abb_destruir(arbol1);


	/*Arbol con datos dinamicos tipo listas*/
	// Creo un arbol vacio
	abb_t* arbol2 = abb_crear(*cmp, *destruir_dato_lista);
	print_test("Prueba arbol2 vacio: ", arbol2);
	print_test("Cantidad de arbol2 es 0: ", abb_cantidad(arbol2) == 0);

	// Guardo clave 5, val12 en la raiz de arbol2
	print_test("Guardo clave 52, val12 en la raiz de arbol2: ", abb_guardar(arbol2, "52", val12));
	print_test("Cantidad de arbol2 es 1: ", abb_cantidad(arbol2) == 1);

	// Guardo clave 2, val13 en arbol2. Debe guardarse en el hijo izquierdo de 5.
	print_test("Guardo clave 2, val13 en arbol2: ", abb_guardar(arbol2, "2", val13));
	print_test("Cantidad de arbol2 es 2: ", abb_cantidad(arbol2) == 2);
	// Guardo clave 33, val14 en arbol2. Debe guardarse en el hijo derecho de 2.
	print_test("Guardo clave 33, val14 en arbol2: ", abb_guardar(arbol2, "33", val14));
	print_test("Cantidad de arbol2 es 3: ", abb_cantidad(arbol2) == 3);
	// Guardo clave 99, val15 en arbol2. Debe guardarse en el hijo derecho de 5.
	print_test("Guardo clave 99, val15 en arbol2: ", abb_guardar(arbol2, "99", val15));
	print_test("Cantidad de arbol2 es 4: ", abb_cantidad(arbol2) == 4);
	// Guardo clave 2, val16 en arbol2. Debe reemplazar a (2, val13). 
	print_test("Reemplazo dato de clave 2 por val16 en arbol2: ", abb_guardar(arbol2, "2", val16));
	// La cantidad debe haber permanecido constante.
	print_test("Cantidad de arbol2 es 4: ", abb_cantidad(arbol2) == 4);

	// Busco clave 2
	print_test("Busco clave 2 con abb_obtener, devuelve val16: ", val16 == abb_obtener(arbol2, "2"));
	// Busco clave 111, no existe
	print_test("Busco clave 111 con abb_obtener, devuelve NULL: ", NULL == abb_obtener(arbol2, "111"));
	// Busco clave 33
	print_test("Busco clave 33 con abb_pertenece, devuelve true: ", abb_pertenece(arbol2, "33"));
	// Busco clave 10, no pertenece
	print_test("Busco clave 10 con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "10"));


	// Borrar
	print_test("Cantidad de arbol2 antes de borrar es 4: ", abb_cantidad(arbol2) == 4);
	print_test("Borro nodo hoja, clave 33, devuelve el dato val14: ", abb_borrar(arbol2, "33") == val14);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "33"));
	print_test("Cantidad de arbol2 despues de borrar es 3: ", abb_cantidad(arbol2) == 3);
	print_test("Cantidad de hojas arbol2 despues de borrar es 2: ", abb_contar_hojas(arbol2) == 2);

	print_test("Guardo clave 30, val17 en arbol2: ", abb_guardar(arbol2, "30", val17));
	print_test("Guardo clave -1, val18 en arbol2: ", abb_guardar(arbol2, "-1", val18));
	print_test("Guardo clave -6, val19 en arbol2: ", abb_guardar(arbol2, "-6", val19));
	print_test("Guardo clave 15, val20 en arbol2: ", abb_guardar(arbol2, "15", val20));
	print_test("Cantidad de hojas arbol2 es 3: ", abb_contar_hojas(arbol2) == 3);


	print_test("Borro nodo con un solo hijo, clave -1, devuelve el dato val18: ", abb_borrar(arbol2, "-1") == val18);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "-1"));
	print_test("Cantidad de arbol2 despues de borrar es 6: ", abb_cantidad(arbol2) == 6);
	print_test("Cantidad de hojas arbol2 despues de borrar es 3: ", abb_contar_hojas(arbol2) == 3);

	print_test("Borro nodo con un solo hijo, clave 30, devuelve el dato val17: ", abb_borrar(arbol2, "30") == val17);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "30"));
	print_test("Cantidad de arbol2 despues de borrar es 5: ", abb_cantidad(arbol2) == 5);
	print_test("Cantidad de hojas arbol2 despues de borrar es 3: ", abb_contar_hojas(arbol2) == 3);

	print_test("Guardo clave -7, val21 en arbol2: ", abb_guardar(arbol2, "-7", val21));
	print_test("Guardo clave 0, val22 en arbol2: ", abb_guardar(arbol2, "0", val22));
	print_test("Cantidad de hojas arbol2 es 4: ", abb_contar_hojas(arbol2) == 4);

	print_test("Borro nodo con un dos hijos, clave 2, devuelve el dato val16: ", abb_borrar(arbol2, "2") == val16);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "2"));
	print_test("Cantidad de arbol2 despues de borrar es 6: ", abb_cantidad(arbol2) == 6);
	print_test("Cantidad de hojas arbol2 despues de borrar es 3: ", abb_contar_hojas(arbol2) == 3);

	print_test("Borro nodo raiz con un dos hijos, clave 52, devuelve el dato val12: ", abb_borrar(arbol2, "52") == val12);
	print_test("Busco el nodo borrado con abb_pertenece, devuelve false: ", !abb_pertenece(arbol2, "52"));
	print_test("Cantidad de arbol2 despues de borrar es 5: ", abb_cantidad(arbol2) == 5);
	print_test("Cantidad de hojas arbol2 despues de borrar es 3: ", abb_contar_hojas(arbol2) == 2);

	// Iterador externo
	printf("Iterador inorder con funcion de impresion:\n");
	abb_in_order(arbol2, printear, NULL);


	// Iterador interno
	abb_iter_t* iter_Auto = abb_iter_in_crear(arbol2);
	int contador = 0;
	while (!abb_iter_in_al_final(iter_Auto)){
		abb_iter_in_avanzar(iter_Auto);
		contador++;
	}
	print_test("Iterador interno recorre todos los elementos: ", abb_cantidad(arbol2) == contador);
	abb_iter_in_destruir(iter_Auto);
		
	abb_iter_t* iter2 = abb_iter_in_crear(arbol2);
	print_test("Iterador interno (al final es false): ", !abb_iter_in_al_final(iter2));
	clave = abb_iter_in_ver_actual(iter2);
	clave_int = atoi(clave);
	print_test("Iterador interno (-7): ", clave_int == -7);
	abb_iter_in_avanzar(iter2);
	clave = abb_iter_in_ver_actual(iter2);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (-6)): ", clave_int == -6);
	abb_iter_in_avanzar(iter2);
	clave = abb_iter_in_ver_actual(iter2);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (0)): ",  clave_int == 0);
	print_test("Iterador interno (al final es false): ", !abb_iter_in_al_final(iter2));
	abb_iter_in_avanzar(iter2);
	clave = abb_iter_in_ver_actual(iter2);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (15)): ",  clave_int == 15);
	abb_iter_in_avanzar(iter2);
	clave = abb_iter_in_ver_actual(iter2);
	clave_int = atoi(clave);
	print_test("Iterador interno (avanzar (99)): ",  clave_int == 99);
	abb_iter_in_avanzar(iter2);
	print_test("Iterador interno (avanzar, al final): ", !abb_iter_in_ver_actual(iter2));
	print_test("Iterador interno (al final es true): ", abb_iter_in_al_final(iter2));
	abb_iter_in_destruir(iter2);

	// Destruir arbol
	abb_destruir(arbol2); // Destruyo val20, val21, val15, val19, val22. Ya destrui val13
	
	// Destruyo las listas que quedaron
	//~ lista_destruir(val12, NULL);
	//~ //lista_destruir(val13, NULL);
	//~ lista_destruir(val14, NULL);
	//~ lista_destruir(val16, NULL);
	//~ lista_destruir(val17, NULL);
	//~ lista_destruir(val18, NULL);
	//~ 
	// Nuevas pruebas
	
	cola_t* val23 = cola_crear();
	cola_t* val24 = cola_crear();
	char* clave2 = "2";
	char* clave3 = "3";


	cola_encolar(val23, "a");
	cola_encolar(val24, "d");

	abb_t* arbol4 = abb_crear(*cmp, destruir_cola);

	print_test("Cantidad de arbol4 es 0: ", abb_cantidad(arbol4) == 0);

	print_test("Guardo clave 2  en la raiz de arbol4: ", abb_guardar(arbol4, clave2, val23));
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 2 es val23: ", abb_obtener(arbol4, clave2) == val23);

	print_test("Borro clave 2: ", abb_borrar(arbol4, clave2) == val23);
	print_test("Cantidad de arbol4 es 0: ", abb_cantidad(arbol4) == 0);
	print_test("Prueba abb_obtener clave 2 es falso: ", !abb_obtener(arbol4, clave2));
	
	print_test("Guardo clave 2  en la raiz de arbol4: ", abb_guardar(arbol4, clave2, val24));
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 2 es val24: ", abb_obtener(arbol4, clave2) == val24);

	print_test("Reemplazo el valor de clave 2,  en arbol4: ", abb_guardar(arbol4, clave2, val23));
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 2 es val23: ", abb_obtener(arbol4, clave2) == val23);
	
	print_test("Borro clave 2: ", abb_borrar(arbol4, clave2) == val23);
	print_test("Cantidad de arbol4 es 0: ", abb_cantidad(arbol4) == 0);	
	print_test("Prueba abb_obtener clave 2 es falso: ", !abb_obtener(arbol4, clave2));
	
	print_test("Guardo clave 3,  en arbol4: ", abb_guardar(arbol4, clave3, val23));
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 3 es val23: ", abb_obtener(arbol4, clave3) == val23);

	print_test("Reemplazo clave 3,  en arbol4: ", abb_guardar(arbol4, clave3, val24));
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 3 es val24: ", abb_obtener(arbol4, clave3) == val24);

	print_test("Guardo clave 2  en la raiz de arbol4: ", abb_guardar(arbol4, clave2, val24));
	print_test("Cantidad de arbol4 es 2: ", abb_cantidad(arbol4) == 2);
	print_test("Prueba abb_obtener clave 2 es val24: ", abb_obtener(arbol4, clave2) == val24);
	
	print_test("Borro clave 3: ", abb_borrar(arbol4, clave3) == val24);
	print_test("Cantidad de arbol4 es 1: ", abb_cantidad(arbol4) == 1);
	print_test("Prueba abb_obtener clave 3 es falso: ", !abb_obtener(arbol4, clave3));

	//Modifico desde afuera clave2 (valor original "2"):
	clave2 = "33";
	// Sin embargo en el arbol sigue estando "2"
	print_test("Prueba abb_obtener clave 2 es val24: ", abb_obtener(arbol4, "2") == val24);

	print_test("Borro clave 2: ", abb_borrar(arbol4, "2") == val24);
	print_test("Cantidad de arbol4 es 0: ", abb_cantidad(arbol4) == 0);	
	print_test("Prueba abb_obtener clave 2 es falso: ", !abb_obtener(arbol4, "2"));

	
	abb_iter_t* iter4 = abb_iter_in_crear(arbol4);
	abb_iter_in_destruir(iter4);
	abb_iter_t* iter5 = abb_iter_in_crear(arbol4);

	print_test("Iterador interno (actual NULL): ",  !abb_iter_in_ver_actual(iter5));
	print_test("Iterador interno (avanzar falso): ",  !abb_iter_in_avanzar(iter5));
	
	abb_iter_in_destruir(iter5);

	//~ cola_destruir(val23, NULL);
	//~ cola_destruir(val24, NULL);
	//~ 
	abb_destruir(arbol4);


}


int main(int argc, char **argv)
{
	prueba_abb();

	return 0;
}

