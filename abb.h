#ifndef ABB_H
#define ABB_H
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct abb_nodo abb_nodo_t;	
typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//Auxiliar
abb_nodo_t* abb_ver_raiz (abb_t* arbol);
const char* ver_clave (abb_nodo_t* nodo);
void* ver_dato (abb_nodo_t* nodo);

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
abb_nodo_t* abb_nodo_crear (const char* clave, void* dato);

bool abb_guardar_r (abb_nodo_t** nodo, abb_t* arbol, const char* clave, void* dato);
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
void *abb_borrar(abb_t *arbol, const char *clave);

void *abb_obtener(const abb_t *arbol, const char *clave);
bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol);

void abb_destruir(abb_t *arbol);


//***************** ITERADORES ********************
// Iterador externo
void abb_in_order(abb_t *arbol, bool funcion(const char *, void *, void *), void *extra);

// Iterador interno
typedef struct abb_iter abb_iter_t;

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
bool abb_iter_in_avanzar(abb_iter_t *iter);
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
bool abb_iter_in_al_final(const abb_iter_t *iter);
void abb_iter_in_destruir(abb_iter_t* iter);



#endif // ABB_H
