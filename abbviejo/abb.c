#include "abb.h"
#include "tdas.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
// Libera nodo->clave y nodo, devuelve nodo->dato
void* abb_nodo_destruir(abb_nodo_t* nodo){
	if (!nodo) return NULL;
	void* dato = nodo->dato;
	free((char*) nodo->clave);
	free(nodo);
	return dato;
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
		// Hago una copia de la clave
		char* nueva_clave = malloc(sizeof(char) * (1+ strlen(clave)));
		if (!nueva_clave){
			free(nueva_clave);
			return false;
		}
		strcpy(nueva_clave, clave);
		//~ printf("Nueva_clave: %s", nueva_clave);
		abb_nodo_t* nuevo = abb_nodo_crear(nueva_clave, dato);
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
		// Le asigno el nuevo dato
		(*nodo)->dato = dato;
		
		return true;
	}
	
	// Si la clave de *nodo es menor a la clave que me pasaron
	if (r<0){
		// llamo a abb_guardar_r con el izquierdo
		return abb_guardar_r(&(*nodo)->izq, arbol, clave, dato);
	}
	// Si la clave de *nodo es mayor a la clave que me pasaron
	if (r>0){
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
	if (cmp(nodo->clave, clave) < 0)
		return abb_buscar_r(nodo->izq, cmp, clave);

	// Si la clave es menor a la del nodo actual, llamo recursivamente con
	// el nodo derecho
	if (cmp(nodo->clave, clave) > 0)
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
	if (arbol->cmp(nodo->clave, clave) > 0)
		return abb_borrar_r(nodo->der, nodo, clave, arbol);

	if (arbol->cmp(nodo->clave, clave) < 0)
		return abb_borrar_r(nodo->izq, nodo, clave, arbol);

	
	if (arbol->cmp(nodo->clave, clave) == 0){
		// Si nodo es hoja
		if (!nodo->izq && !nodo->der){
			// Si su padre es NULL (o sea es raiz ppal del arbol)
			if (padre == NULL){
				// Arbol apunta a NULL
				arbol->raiz = NULL;
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
			
			arbol->cant --;
			return abb_nodo_destruir(nodo);
		}
		// Si tiene un solo hijo (izq)
		if ((!nodo->der) && (nodo->izq)){
			if (!padre) arbol->raiz = nodo->izq;
			else if (nodo == padre->der)
				padre->der = nodo->izq;
			else if (nodo == padre->izq)
				padre->izq = nodo->izq;
			arbol->cant --;
			return abb_nodo_destruir(nodo);
		}
		// Si tiene un soo hijo (der)
		if ((!nodo->izq) && (nodo->der)){
			if (!padre) arbol->raiz = nodo->der;
			else if (nodo == padre->der)
				padre->der = nodo->der;
			else if (nodo == padre->izq)
				padre->izq = nodo->der;
			arbol->cant --;
			return abb_nodo_destruir(nodo);
		}
		// Si tiene dos hijos
		abb_nodo_t* reemplazante = nodo->der;
		abb_nodo_t* hijo_izq_reempl = nodo->izq;
		while (true){
			if (reemplazante->izq == NULL) break;
			reemplazante = reemplazante->izq;
		}

		if (!padre){ 
			padre = reemplazante;
			arbol->raiz = padre;
		}
		else if (nodo == padre->der)
			padre->der = reemplazante;
		else if (nodo == padre->izq)
			padre->izq = reemplazante;

		reemplazante->izq = hijo_izq_reempl;
		arbol->cant --;
		return abb_nodo_destruir(nodo);

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
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    // Si el arbol no existe
    if (!arbol) return NULL;
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	iter->pila = pila_crear();
	abb_nodo_t* nodo = arbol->raiz;
	if (arbol->raiz){
		while (nodo->izq){
			pila_apilar(iter->pila, nodo);
			nodo = nodo->izq;
		}
	}
	iter->actual = nodo;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t* iter){
	if (abb_iter_in_al_final(iter)) return false;
	if (!iter->actual->der)
		iter->actual = pila_desapilar(iter->pila);
	else{
		iter->actual = iter->actual->der;
		while (iter->actual->izq){
			pila_apilar(iter->pila, iter->actual);
			iter->actual = iter->actual->izq;
		}
	}
	return true;
}



const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if (!iter->actual) return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	if (!iter->actual) return true;
	return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila, NULL);
	free(iter);
	return;
}	

void abb_destruir_r(abb_nodo_t* nodo, abb_destruir_dato_t funcion){
	if (nodo->izq)
		abb_destruir_r(nodo->izq, funcion);

	if (nodo->der)
		abb_destruir_r(nodo->der, funcion);
	
	void* dato = abb_nodo_destruir(nodo);
	if (funcion)
		funcion(dato);
	return;
}

/* Destructor */
void abb_destruir(abb_t* arbol){
	if (!arbol) return;
	if (arbol->raiz)
		abb_destruir_r(arbol->raiz, arbol->destruir_dato);
	free (arbol);
}
