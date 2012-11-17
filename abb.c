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

// Crea una copia de la clave pasada por parametro (dato tipo const char*)
// Devuelve NULL si no funciono malloc(), devuelve la copia en caso contrario
// (dato tipo const char*)
char* crear_clave(const char* clave){
	char *copia = malloc (sizeof(char) * (strlen(clave) + 1));
	if (!copia)
		return NULL;
	strcpy(copia, clave);
	return copia;
}

// Crea un nodo (dato tipo abb_nodo_t*). Recibe una clave (dato tipo const
// char*) y un dato (void*).
// Devuelve el nodo creado y NULL si fallo malloc()
abb_nodo_t* abb_nodo_crear (const char* clave, void* dato){
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->clave = crear_clave(clave);
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}
// Libera nodo->clave y nodo, devuelve nodo->dato
void* abb_nodo_destruir(abb_nodo_t* nodo){
	if (!nodo) return NULL;
	void* dato = nodo->dato;
	// Si no pongo char* da warning! So stop complaining!
	free((char*)nodo->clave);
	free(nodo);
	return dato;
}
// Crea un arbol (tipo abb_t*)
// Recibe una funcion del tipo abb_comparar_clave_t y una del tipo
// abb_destruir_dato_t
// Devuelve NULL si fallo malloc(), el arbol en caso contrario
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if (arbol == NULL) return NULL;
	arbol->raiz = NULL;
	arbol->cant = 0;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	return arbol;
}

// Recibe dato tipo abb_t* y devuelve dato tipo size_t con la cantidad
// de elementos guardados en el arbol.
// si el arbol pasado por parametro no existe, devuelve 0.
size_t abb_cantidad(abb_t* arbol){
	if (!arbol) return 0;
	return arbol->cant;
}


bool abb_guardar_r (abb_nodo_t* nodo, abb_nodo_t* padre, int lado, abb_t* arbol, const char* clave, void* dato){
	// lado = 1 "derecha"
	// lado = -1 "izquierda
	// Si el nodo no existe
	if (!nodo){
		nodo = abb_nodo_crear(clave, dato);
		arbol->cant ++;

		if (lado == 1)
			padre->der = nodo;
		else if (lado == -1)
			padre->izq = nodo;
		
		return true;
	}

	
	int r = arbol->cmp(nodo->clave, clave);

	if (r==0){
		if (arbol->destruir_dato)
			arbol->destruir_dato(nodo->dato);
		nodo->dato = dato;
		
		return true;
	}
	
	// Si la clave de *nodo es menor a la clave que me pasaron
	if (r < 0){
		// llamo a abb_guardar_r con el izquierdo
		return abb_guardar_r(nodo->der, nodo, 1, arbol, clave, dato);
	}
	// Si la clave de *nodo es mayor a la clave que me pasaron
	if (r > 0){
		// llamo a abb_guardar_r con el derecho
		return abb_guardar_r(nodo->izq, nodo, -1, arbol, clave, dato);
		}
	return true;
}

// Recibe un arbol, un const char* y un void* para crear un nuevo nodo y guardarlo
// en el arbol. Si ya existe dato con la clave pasada, se reemplaza el dato
// en el nodo con la clave repetida.
bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	if (!arbol) return false;
	// Si el arbol esta vacio, guardo en la raiz
	if (arbol->cant == 0){
		abb_nodo_t* nuevo = abb_nodo_crear(clave, dato);
		arbol->raiz = nuevo;
		arbol->cant ++;
		return true;
	}
	// llamada a la recursiva
	else
		return abb_guardar_r(arbol->raiz, NULL, 0, arbol, clave, dato);

}


// EJERCICIO extra. Cuenta la cantidad de hojas de un arbol
size_t abb_contar_hojas_r(abb_nodo_t* nodo){
	if (!nodo) return 0;

	if (!nodo->izq && !nodo->der) return 1;
	return abb_contar_hojas_r(nodo->izq) + abb_contar_hojas_r(nodo->der);
}

size_t abb_contar_hojas(abb_t* arbol){
	if (!arbol) return 0;
	return abb_contar_hojas_r(arbol->raiz);
}



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
	// el nodo derecho
	if (cmp(nodo->clave, clave) < 0)
		return abb_buscar_r(nodo->der, cmp, clave);

	// Si la clave es menor a la del nodo actual, llamo recursivamente con
	// el nodo izquierdo
	if (cmp(nodo->clave, clave) > 0)
		return abb_buscar_r(nodo->izq, cmp, clave);
	
	return NULL;
	
}

// Recibe un arbol y una clave, devuelve NULL si no encuentra la clave en
// el arbol o dato de tipo abb_nodo_t* si encuentra el nodo que contiene
// la clave
abb_nodo_t* abb_buscar(const abb_t *arbol, const char *clave){
	if (!arbol)
		return NULL;
	return abb_buscar_r(arbol->raiz, arbol->cmp, clave);
}


// Devuelve el dato del nodo que contiene la clave pasada por parametro.
void* abb_obtener (const abb_t* arbol, const char* clave){
	abb_nodo_t* nodo_buscado = abb_buscar(arbol, clave);
	if (!nodo_buscado)
		return NULL;
	return nodo_buscado->dato;
}


// devuelve true si encuentra la clave en el arbol, false en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave){
	return abb_buscar(arbol, clave) != NULL;
}

// Borra un nodo que es hoja
void* abb_borrar_hoja(abb_nodo_t* nodo, abb_nodo_t* padre, int lado, abb_t* arbol){
	// Si su padre es NULL (o sea es raiz ppal del arbol)
	if (padre == NULL){
		// Arbol apunta a NULL
		arbol->raiz = NULL;
	}
	// Si nodo es el hijo izquierdo de su padre
	else if (lado == 1){
		// La rama izquierda del padre apunta a NULL
		padre->izq = NULL;
	}
	// Si nodo es el hijo derecho de su padre
	else if (lado == -1){
		// La rama derecha del padre apunta a NULL
		(padre->der) = NULL;
	}
	
	arbol->cant --;
	return abb_nodo_destruir(nodo);
}


// Borra un nodo que tiene un solo hijo
void* abb_borrar_con_hijo_unico(abb_nodo_t* nodo, abb_nodo_t* padre, int lado, abb_t* arbol){

	// Si tiene hijo izquierdo
	if ((!nodo->der) && (nodo->izq)){
		if (!padre) 
			arbol->raiz = nodo->izq;
			
		else if (lado == -1)
			padre->der = nodo->izq;

		else if (lado == 1)
			padre->izq = nodo->izq;

		arbol->cant --;
		return abb_nodo_destruir(nodo);
	}

	// Si tiene hijo derecho
	else if ((nodo->der) && (!nodo->izq)){
		if (!padre) 
			arbol->raiz = nodo->der;
		else if (lado == -1)
			padre->der = nodo->der;
		else if (lado == 1)
			padre->izq = nodo->der;
		arbol->cant --;
		return abb_nodo_destruir(nodo);
	}
	return NULL;
}


// Funcion que dado un nodo busca sus descendientes izquierdos. Devuelve
// el padre del mas izquierdo
abb_nodo_t* buscar_padre_mas_izq(abb_nodo_t* reemplazante){
	abb_nodo_t* padre_del_reempl = NULL; 
	while (reemplazante->izq != NULL){
		padre_del_reempl = reemplazante;
		// reemplazante se transforma en su izquierdo
		reemplazante = reemplazante->izq;
	}
	return padre_del_reempl;
}

// Asigna los hijos que va a tener un nodo reemplazante de izquierda
void asignar_hijos_a_reempl_de_izq(abb_nodo_t* padre_del_reempl, abb_nodo_t* nodo){
	abb_nodo_t* reemplazante = padre_del_reempl->izq;
	
	padre_del_reempl->izq = reemplazante->der;
	// De todas formas
	reemplazante->izq = nodo->izq;
	reemplazante->der = nodo->der;
	
	return;
}


// Borra un nodo con dos hijos
void* abb_borrar_con_2hijos(abb_nodo_t* nodo, abb_nodo_t* padre, int lado, abb_t* arbol){
	
	abb_nodo_t* padre_del_reemplazante = buscar_padre_mas_izq(nodo->der);
	abb_nodo_t* reemplazante;
	if (!padre_del_reemplazante){
		reemplazante = nodo->der;
		reemplazante->izq = nodo->izq;
	}
	else{
		reemplazante = padre_del_reemplazante->izq;
		asignar_hijos_a_reempl_de_izq(padre_del_reemplazante, nodo);
	}
	
	if (!padre){
		arbol->raiz = reemplazante;
	}
	
	else if (lado == -1){
		padre->der = reemplazante;
	}
	else if (lado == 1){
		padre->izq = reemplazante;
	}
	
	arbol->cant --;
	return abb_nodo_destruir(nodo);
}


// Funcion recursiva de borrar
void* abb_borrar_r(abb_nodo_t* nodo, abb_nodo_t* padre, int lado, const char* clave, abb_t* arbol){
	// lado -1 "izq"
	// lado 1 "der"
	int r = arbol->cmp(nodo->clave, clave);
	if (r > 0)
		return abb_borrar_r(nodo->izq, nodo, 1, clave, arbol);
	
	if (r < 0)
		return abb_borrar_r(nodo->der, nodo, -1, clave, arbol);
	
	if (r == 0){
		// Si nodo es hoja
		if (!nodo->izq && !nodo->der)
			return abb_borrar_hoja(nodo, padre, lado, arbol);
		
		// Si tiene un solo hijo
		else if ((!nodo->der && nodo->izq) || (!nodo->izq && nodo->der))
			return abb_borrar_con_hijo_unico(nodo, padre, lado, arbol);
		
		// Si tiene dos hijos
		else if (nodo->der && nodo->izq)
			return abb_borrar_con_2hijos(nodo, padre, lado, arbol);
	}
	return NULL;
}

// Dado un arbol y una clave, se encarga de borrar el nodo que contiene
// dicha clave. Devuelve el dato del nodo borrado.
// si el nodo buscado no existe devuelve NULL.
void* abb_borrar(abb_t *arbol, const char *clave){
	if (!arbol) return NULL;
	if (!abb_pertenece(arbol, clave))
		return NULL;
	else
		return abb_borrar_r(arbol->raiz, NULL, 0, clave, arbol);
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
