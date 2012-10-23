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


bool abb_guardar_r (abb_nodo_t** nodo, abb_t* arbol, const char* clave, void* dato){
	// Si el nodo no existe
	if (!(*nodo)){
		//~ puts("Entre a nodo no existe");
		abb_nodo_t* nuevo = abb_nodo_crear(clave, dato);
		if (nuevo){
			//~ puts("Asigno lo nuevo");
			*nodo = nuevo;
			arbol->cant ++;
			return true;
		}
		return false;
	}
	
	int r = arbol->cmp((*nodo)->clave, clave);
	
	// Si las claves son iguales
	if (r==0){
		//~ puts ("Entre a claves iguales");
		// Destruyo el dato de *nodo
		if (arbol->destruir_dato){
			arbol->destruir_dato((*nodo)->dato);
			}
		// Le asigno el nuevo dato
		//~ puts("Reemplazo el dato");
		(*nodo)->dato = dato;
		
		return true;
	}
	
	// Si la clave de *nodo es menor a la clave que me pasaron
	if (r == (-1)){
		//~ puts("Entre a clave nueva menor");
		// llamo a abb_guardar_r con el izquierdo
		//~ puts("Voy al nodo izquierdo");
		return abb_guardar_r(&(*nodo)->izq, arbol, clave, dato);
	}
	// Si la clave de *nodo es mayor a la clave que me pasaron
	if (r == 1){
		// llamo a abb_guardar_r con el derecho
		//~ puts("Entre a clave nueva mayor");
		//~ puts("Voy al nodo derecho");
		return abb_guardar_r(&(*nodo)->der, arbol, clave, dato);
		}
	return true;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	if (!arbol) return false;
	return abb_guardar_r(&arbol->raiz, arbol, clave, dato);
}


// EJERCICIO BORRAR
size_t abb_contar_hojas_r(abb_nodo_t* nodo){
	if (!nodo) return 0;

	if (!nodo->izq && !nodo->der) return 1;
	return abb_contar_hojas_r(nodo->izq) + abb_contar_hojas_r(nodo->der);
}

size_t abb_contar_hojas(abb_t* arbol){
	if (!arbol) return 0;
	return abb_contar_hojas_r(arbol->raiz);
}


/* Funcion interna abb_buscar */
// Recibe un arbol y una clave. 
// Devuelve el nodo que contiene tal clave, o NULL si no se encuentra.

abb_nodo_t* abb_buscar_r (abb_nodo_t* nodo, abb_comparar_clave_t cmp, const char *clave){
	// Si el nodo es NULL, llegue al final de la busqeuda y no encontre la clave
	if (!nodo) 
		return NULL;
		
	// Si la clave del nodo actual coincide, devuelvo el dato de ese nodo
	if (cmp(nodo->clave, clave) == 0)
		return nodo;
		
	// Si la clave es mayor a la del nodo actual, llamo recursivamente con
	// el nodo izquierdo
	if (cmp(nodo->clave, clave) == -1)
		return abb_buscar_r(nodo->izq, cmp, clave);

	// Si la clave es menor a la del nodo actual, llamo recursivamente con
	// el nodo derecho
	if (cmp(nodo->clave, clave) == 1)
		return abb_buscar_r(nodo->der, cmp, clave);
	
	return NULL;
	
}

abb_nodo_t* abb_buscar(const abb_t *arbol, const char *clave){
	if (!arbol) return NULL;
	return abb_buscar_r (arbol->raiz, arbol->cmp, clave);
}



void* abb_obtener (const abb_t* arbol, const char* clave){
	abb_nodo_t* nodo_buscado = abb_buscar(arbol, clave);
	if (!nodo_buscado)
		return NULL;
	return nodo_buscado->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (abb_buscar (arbol, clave)) return true;
	return false;
}

//~ void* abb_borrar_r(abb_nodo_t** nodo, const char* clave, abb_t* arbol){
	//~ if (!*nodo) return NULL;
	//~ if ((*nodo)->clave == clave){
		//~ // Si nodo es hoja
		//~ if (!(*nodo)->izq && !(*nodo)->der){
			//~ // Si su padre es NULL (o sea es raiz ppal del arbol)
			//~ if (&(*nodo) == NULL){
				//~ arbol = NULL;
				//~ void* dato = (*nodo)->dato;
				//~ free(*nodo);
				//~ return dato;
			//~ }
			//~ if (&(*nodo)->izq == (*nodo)){
				//~ &(*nodo)->izq = NULL;
				//~ void* dato = (*nodo)->dato;
				//~ free(*nodo);
				//~ return dato;
			//~ }
			//~ if (&(*nodo)->der == (*nodo)){
				//~ (&(*nodo)->der) = NULL;
				//~ void* dato = (*nodo)->dato;
				//~ free(*nodo);
				//~ return dato;
			//~ }
//~ 
//~ 
		//~ }
		//~ 
		//~ 
	//~ }
//~ 
//~ }
//~ 
//~ void* abb_borrar(abb_t *arbol, const char *clave){
	//~ if (!arbol) return NULL;
	//~ return abb_borrar_r(&arbol->raiz, clave, arbol);
//~ }





//***************** ITERADORES ********************
/* Iterador in order */
void abb_in_order_r(abb_nodo_t* nodo, bool funcion(const char*, void*, void*), void* extra){
	if (nodo->izq)
		abb_in_order_r(nodo->izq, funcion, extra);
	funcion(nodo->clave, nodo->dato, extra);
	if (nodo->der)
		abb_in_order_r(nodo->der, funcion, extra);
	return;
}

void abb_in_order(abb_t *arbol, bool funcion(const char *, void *, void *), void *extra){
	abb_in_order_r(arbol->raiz, funcion, extra);
	return;
}

/* Iterador externo */
struct abb_iter{
	abb_nodo_t* actual;
	abb_nodo_t* padre;
};
