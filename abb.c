#include "abb.h"
#include "pila.h"
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
	if (!arbol) return 0;
	return arbol->cant;
}


bool abb_guardar_r (abb_nodo_t** nodo, abb_t* arbol, const char* clave, void* dato){
	// Si el nodo no existe
	if (!(*nodo)){
		abb_nodo_t* nuevo = abb_nodo_crear(clave, dato);
		if (nuevo){
			*nodo = nuevo;
			arbol->cant ++;
			return true;
		}
		return false;
	}
	
	int r = arbol->cmp((*nodo)->clave, clave);
	
	// Si las claves son iguales
	if (r==0){
		// Destruyo el dato de *nodo
		if (arbol->destruir_dato){
			arbol->destruir_dato((*nodo)->dato);
			}
		// Le asigno el nuevo dato
		(*nodo)->dato = dato;
		
		return true;
	}
	
	// Si la clave de *nodo es menor a la clave que me pasaron
	if (r == (-1)){
		// llamo a abb_guardar_r con el izquierdo
		return abb_guardar_r(&(*nodo)->izq, arbol, clave, dato);
	}
	// Si la clave de *nodo es mayor a la clave que me pasaron
	if (r == 1){
		// llamo a abb_guardar_r con el derecho
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

void* abb_borrar_r(abb_nodo_t* nodo, abb_nodo_t* padre, const char* clave, abb_t* arbol){
	if (!nodo){
		return NULL;
	}
	if (arbol->cmp(nodo->clave, clave) == 1)
		return abb_borrar_r(nodo->der, nodo, clave, arbol);

	if (arbol->cmp(nodo->clave, clave) == -1)
		return abb_borrar_r(nodo->izq, nodo, clave, arbol);

	
	if (arbol->cmp(nodo->clave, clave) == 0){
		// Si nodo es hoja
		if (!nodo->izq && !nodo->der){
			// Si su padre es NULL (o sea es raiz ppal del arbol)
			if (padre == NULL){
				// Arbol apunta a NULL
				arbol = NULL;
			}
			// Si nodo es el hijo izquierdo de su padre
			else if (padre->izq == nodo){
				// La rama izquierda del padre apunta a NULL
				padre->izq = NULL;
			}
			// Si nodo es el hijo derecho de su padre
			else if (padre->der == nodo){
				// La rama derecha del padre apunta a NULL
				(padre->der) = NULL;
			}
			
			void* dato = nodo->dato;
			free(nodo);
			arbol->cant --;
			return dato;
		}
		// Si tiene un solo hijo (izq)
		if ((!nodo->der) && (nodo->izq)){
			if (!padre) arbol->raiz = nodo->izq;
			else if (nodo == padre->der)
				padre->der = nodo->izq;
			else if (nodo == padre->izq)
				padre->izq = nodo->izq;
			void* dato = nodo->dato;
			free(nodo);
			arbol->cant --;
			return dato;			
		}
		// Si tiene un soo hijo (der)
		if ((!nodo->izq) && (nodo->der)){
			if (!padre) arbol->raiz = nodo->der;
			else if (nodo == padre->der)
				padre->der = nodo->der;
			else if (nodo == padre->izq)
				padre->izq = nodo->der;
			void* dato = nodo->dato;
			free(nodo);
			arbol->cant --;
			return dato;			
		}
		// Si tiene dos hijos
		abb_nodo_t* reemplazante = nodo->der;
		abb_nodo_t* hijo_izq_reempl = nodo->izq;
		while (true){
			if (reemplazante->izq == NULL) break;
			reemplazante = reemplazante->izq;
		}

		void* dato = nodo->dato;
		if (!padre){ 
			padre = reemplazante;
			arbol->raiz = padre;
		}
		else if (nodo == padre->der)
			padre->der = reemplazante;
		else if (nodo == padre->izq)
			padre->izq = reemplazante;

		reemplazante->izq = hijo_izq_reempl;
		free(nodo);
		arbol->cant --;
		return dato;			
		

	}
	return NULL;
}

void* abb_borrar(abb_t *arbol, const char *clave){
	if (!arbol) return NULL;
	return abb_borrar_r(arbol->raiz, NULL, clave, arbol);
}




//***************** ITERADORES ********************
/* Iterador in order */
void abb_in_order_r(abb_nodo_t* nodo, bool funcion(const char*, void*, void*), void* extra){
	//~ printf("Nodo: %p", nodo);
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
	pila_t* pila;
	abb_nodo_t* actual;
	const abb_t* arbol;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    if (!arbol) return NULL;
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	iter->arbol = arbol;
	pila_t* pila = pila_crear();
	iter->pila = pila;
	abb_nodo_t* nodo = arbol->raiz;
	while (nodo->izq){
		nodo = nodo->izq;
		}
	pila_apilar(pila, nodo);
	iter->actual = pila_ver_tope(pila);
    return iter;
}

bool abb_iter_in_avanzar_r(abb_nodo_t* actual, pila_t* pila){
	puts("entre a iter_in_avanzar_r");
	printf("actual: %s", actual->clave);
	if (!actual) return false;
	if (actual->izq){
		abb_iter_in_avanzar_r(actual->izq, pila);
		pila_apilar(pila, actual->izq);
	}
	printf( "Desapilo: %s", actual->clave);
	pila_desapilar(pila);

	if (actual->der){
		return abb_iter_in_avanzar_r(actual->der, pila);
		pila_apilar(pila, actual->der);
	}
	return true;
	
}
//~ 
bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)) return false;
	abb_iter_in_avanzar_r(iter->actual, iter->pila);
	
	return true;
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	abb_nodo_t* tope = pila_ver_tope(iter->pila);
	if (!tope) return NULL;
	const char* clave = tope->clave;
	return clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	if (!iter->actual) return true;
	return false;
}
//~ void abb_iter_in_destruir(abb_iter_t* iter);	

void abb_destruir_r(abb_nodo_t* nodo, abb_destruir_dato_t funcion){
	puts("entre a bb_destruir_r");
	if (nodo->izq)
		abb_destruir_r(nodo->izq, funcion);
	if (funcion)
		funcion(nodo->dato);
	puts ("Libero nodo");
	free(nodo);
	if (nodo->der)
		abb_destruir_r(nodo->der, funcion);
	return;
}

/* Destructor */
void abb_destruir(abb_t* arbol){
	puts("Entre a abb_destruir");
	abb_destruir_r(arbol->raiz, arbol->destruir_dato);
	puts ("Libero arbol");
	free (arbol);
	}
