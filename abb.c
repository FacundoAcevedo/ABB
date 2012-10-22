#include "abb.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct abb_nodo{
	const char* clave;
	void* dato;
	abb_nodo_t* izq;
	abb_nodo_t* der;
	};

struct abb{
	abb_nodo_t* raiz;
	size_t cant;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	};


abb_nodo_t* abb_nodo_crear (const char* clave, void* dato){
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->clave = clave;
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

// Crea un arbol
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if (arbol == NULL) return NULL;
	arbol->raiz = NULL;
	arbol->cant = 0;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	return arbol;
}

size_t abb_cantidad(abb_t* arbol){
	return arbol->cant;
}

abb_nodo_t* abb_ver_raiz (abb_t* arbol){
	if (!arbol->raiz) return NULL;
	return arbol->raiz;
}

const char* ver_clave (abb_nodo_t* nodo){
	if (!nodo) return NULL;
	if (!nodo->clave) return NULL;
	return nodo->clave;
}
void* ver_dato (abb_nodo_t* nodo){
	if (!nodo) return NULL;
	if (!nodo->dato) return NULL;
	return nodo->dato;
}


bool abb_guardar_r (abb_nodo_t** nodo, abb_t* arbol, const char* clave, void* dato){
	// Si el nodo no existe
	if (!(*nodo)){
		puts("Entre a nodo no existe");
		abb_nodo_t* nuevo = abb_nodo_crear(clave, dato);
		if (nuevo){
			puts("Asigno lo nuevo");
			*nodo = nuevo;
			arbol->cant ++;
			return true;
		}
		return false;
	}
	
	int r = arbol->cmp((*nodo)->clave, clave);
	
	// Si las claves son iguales
	if (r==0){
		puts ("Entre a claves iguales");
		// Destruyo el dato de *nodo
		if (arbol->destruir_dato){
			arbol->destruir_dato((*nodo)->dato);
			}
		// Le asigno el nuevo dato
		puts("Reemplazo el dato");
		(*nodo)->dato = dato;
		
		return true;
	}
	
	// Si la clave de *nodo es menor a la clave que me pasaron
	if (r == (-1)){
		puts("Entre a clave nueva menor");
		// llamo a abb_guardar_r con el izquierdo
		puts("Voy al nodo izquierdo");
		return abb_guardar_r(&(*nodo)->izq, arbol, clave, dato);
	}
	// Si la clave de *nodo es mayor a la clave que me pasaron
	if (r == 1){
		// llamo a abb_guardar_r con el derecho
		puts("Entre a clave nueva mayor");
		puts("Voy al nodo derecho");
		return abb_guardar_r(&(*nodo)->der, arbol, clave, dato);
		}
	return true;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	if (!arbol) return false;
	return abb_guardar_r(&arbol->raiz, arbol, clave, dato);
}
