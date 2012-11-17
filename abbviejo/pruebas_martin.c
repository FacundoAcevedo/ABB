#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
// los datos de los arboles seran numeros, asi no es necesario crear un funcion
// para destruir en general; exceptuando en una de las pruebas en las que se
// verificará que estas precisamente funcionen correctamente

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

/* Funcion auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result){
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

// Funciones de gestion de random
void random_inicializar(){
	unsigned int seed = (unsigned int)time(NULL);
	srand (seed);
}

int nuestro_random(size_t lim){
	return rand()%lim;
}

// Funciones de swapeo
void swap_char(char** x, char** y){
	char* aux=*x;
	*x=*y;
	*y=aux;
}

void swap_int(int** x, int** y){
	int* aux=*x;
	*x=*y;
	*y=aux;
}
     
// funcion que desordena el vector
void vector_desordenar(char* claves[], int* valores[], size_t largo){
	random_inicializar();
	size_t i;
	int rnd;
	// es importante que el par clave-valor se mantenga siempre igual
	for (i=0; i<largo;i++){
		rnd=nuestro_random(largo);

		swap_char(&claves[i], &claves[rnd]);
		swap_int(&valores[i], &valores[rnd]);
	}

}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/
void pruebas_vacio(){
	printf("pruebas con arbol vacio\n");
	abb_t* arbol=abb_crear(strcmp, NULL);

	print_test("El arbol no tiene elemento", abb_cantidad(arbol)==0);
	print_test("La clave 'Hola' no pertenece al arbol", !abb_pertenece(arbol,"Hola"));
	print_test("No puedo borrar el elemento 'Mundo' del arbol vacio", abb_borrar(arbol,"Mundo")==NULL);
	print_test("No puedo obtener el valor del elemento 'Chau' del arbol vacio", abb_obtener(arbol,"Chau")==NULL);
	abb_destruir(arbol);
	print_test("se detruyo el arbol vacio", true);   
}

void pruebas_unico_valor(){
	printf("Pruebas con arbol con un solo elemento\n");
	abb_t* arbol=abb_crear(strcmp, free);
	int* datos1=malloc(sizeof(int)*2);
	int* datos2=malloc(sizeof(int)*2);

	print_test("El arbol no tiene elemento", abb_cantidad(arbol)==0);
	print_test("La clave 'Hola' no pertenece al arbol", !abb_pertenece(arbol,"Hola"));
	print_test("se agrego el elemento 'Hola'", abb_guardar(arbol,"Hola", datos1));
	print_test("No puedo borrar el elemento 'Mundo' del arbol", abb_borrar(arbol,"Mundo")==NULL);
	print_test("El arbol tiene un elemento", abb_cantidad(arbol)==1);
	print_test("El elemento 'Hola' pertenece al arbol", abb_pertenece(arbol,"Hola"));
	print_test("El dato del elemento 'Hola' es correcto", abb_obtener(arbol,"Hola")==datos1);
	print_test("Puedo actualizar el elemento 'Hola'", abb_guardar(arbol,"Hola", datos2));
	print_test("El dato del elemento 'Hola' es correcto", abb_obtener(arbol,"Hola")==datos2);
	print_test("Puedo borrar el elemento 'Hola'", abb_borrar(arbol,"Hola")!=NULL);
	// aca me estaria faltando liberar memoria (lo hago mas abajo)

	print_test("El elemento 'Hola' no pertenece al arbol", !abb_pertenece(arbol, "Hola"));
	print_test("El arbol no tiene elementos", abb_cantidad(arbol)==0);
	print_test("No puedo borrar el elemento 'Hola'", abb_borrar(arbol, "Hola")==NULL);
	abb_destruir(arbol);
	print_test("se detruyo el arbol vacio", true);      
	free(datos2);
}

void pruebas_pocos_valores(){
	printf("Pruebas con un arbol de 3 o 4 elementos\n");
	int valor1=1,valor2=2,valor3=3;
	abb_t* arbol=abb_crear(strcmp, NULL);
	print_test("Arbol esta vacio", abb_cantidad(arbol)==0);
	print_test("Puedo agregar el elemento 'Hola'", abb_guardar(arbol, "Hola", &valor1));
	printf("Cant de arbol: %zu", abb_cantidad(arbol));
	puts("");
	print_test("Puedo agregar el elemento 'Mundo'", abb_guardar(arbol, "Mundo", &valor2));
	printf("Cant de arbol: %zu", abb_cantidad(arbol));
	puts("");
	print_test("El arbol tiene dos elementos", abb_cantidad(arbol)==2);
	print_test("Puedo agregar el elemento 'Chau'", abb_guardar(arbol, "Chau", &valor3));
	print_test("El arbol tiene tres elementos", abb_cantidad(arbol)==3);
	print_test("El elemento 'Hola' pertenece al arbol", abb_pertenece(arbol, "Hola"));
	print_test("El elemento 'Chau' pertenece al arbol", abb_pertenece(arbol, "Chau"));
	print_test("El elemento 'Mundo' pertenece al arbol", abb_pertenece(arbol, "Mundo"));
	print_test("El elemento 'Viaje' no pertenece al arbol", !abb_pertenece(arbol, "Viaje"));
	print_test("El dato del elemento 'Hola' es 1", *(int*)abb_obtener(arbol, "Hola")==valor1);
	print_test("El dato del elemento 'Chau' es mayor al del elemento 'Mundo'", *(int*)abb_obtener(arbol, "Chau")>*(int*)abb_obtener(arbol, "Mundo"));
	print_test("El elemento 'Viaje' no tiene dato porque no esta en el arbol", abb_obtener(arbol, "Viaje")==NULL);
	print_test("Puedo borrar el elemento 'Chau'", abb_borrar(arbol, "Chau")!=NULL);
	print_test("El arbol tiene 2 elementos", abb_cantidad(arbol)==2);
	print_test("No puedo borrar el elemento 'Viaje'", abb_borrar(arbol, "Viaje")==NULL);
	print_test("Puedo guardar el elemento 'Viaje'", abb_guardar(arbol, "Viaje", NULL));
	print_test("El arbol tiene 3 elementos", abb_cantidad(arbol)==3);
	abb_destruir(arbol);
	print_test("El arbol fue destruido", true);
}

void prueba_iterar_arbol_vacio(){
	printf("Pruebas de iterador con arbol vacio\n");
	abb_t* arbol=abb_crear(strcmp,NULL);
	abb_iter_t* iter=abb_iter_in_crear(arbol);
	print_test("El iter no es nulo", iter!=NULL);
	print_test("La cantidad de elementos del arbol es 0", abb_cantidad(arbol)==0);
	print_test("La clave actual es nula", abb_iter_in_ver_actual(iter)==NULL);
	print_test("Intento avanzar y no puedo", !abb_iter_in_avanzar(iter));
	print_test("Estoy al final con el iterador", abb_iter_in_al_final(iter));
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	print_test("Se destruyeron correctamente el arbol y el iterador",true);
}

void prueba_iterar_unico_elemento(){
	printf("Pruebas de iteracion de un arbol con un solo elemento\n");
	abb_t* arbol=abb_crear(strcmp,free);
	char* valor=malloc(sizeof(char)*5);
	valor[0]='m'; valor[1]='i'; valor[2]='a'; valor[3]='u'; valor[4]='\0';

	print_test("Agrego elemento", abb_guardar(arbol, "Gato", valor));     
	abb_iter_t* iter=abb_iter_in_crear(arbol);

	print_test("No estoy al final", !abb_iter_in_al_final(iter));
	print_test("El elemento actual es de la clave 'Gato'", strcmp(abb_iter_in_ver_actual(iter),"Gato")==0);

	print_test("El dato que guarda el actual es 'miau'", strcmp((char*)abb_obtener(arbol,abb_iter_in_ver_actual(iter))  , "miau")==0);

	print_test("intento avanzar y puedo", abb_iter_in_avanzar(iter));
	print_test("Estoy al final", abb_iter_in_al_final(iter));
	print_test("No puedo avanzar", !abb_iter_in_avanzar(iter));
	print_test("El valor actual es NULL", abb_iter_in_ver_actual(iter)==NULL);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	print_test("Se destruyeron correctamente el arbol y el iterador",true);
}
     
void prueba_iterar_arbol_pocos_elementos(){
	printf("Pruebas de iterar un arbol con 3 elementos\n");
	abb_t* arbol=abb_crear(strcmp,NULL);
	abb_iter_t* iter;
	int valor1=1,valor2=2,valor3=3;

	print_test("Agrego 3 elementos", abb_guardar(arbol, "Hola", &valor1) && abb_guardar(arbol, "Mundo", &valor2) && abb_guardar(arbol, "Chau", &valor3));
	iter=abb_iter_in_crear(arbol);
	print_test("No estoy al final", !abb_iter_in_al_final(iter));
	print_test("El elemento actual es 'Hola'", strcmp(abb_iter_in_ver_actual(iter), "Hola"));
	print_test("Puedo avanzar", abb_iter_in_avanzar(iter));

	print_test("No estoy al final", !abb_iter_in_al_final(iter));
	print_test("Puedo avanzar", abb_iter_in_avanzar(iter));
	print_test("No estoy al final", !abb_iter_in_al_final(iter));
	print_test("El elemento actual es 'Chau'", strcmp(abb_iter_in_ver_actual(iter), "Chau"));
	print_test("Puedo avanzar", abb_iter_in_avanzar(iter));
	print_test("Estoy al final", abb_iter_in_al_final(iter));
	print_test("El valor actual es NULL", abb_iter_in_ver_actual(iter)==NULL);
	print_test("No puedo avanzar", !abb_iter_in_avanzar(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	print_test("Se destruyeron correctamente el arbol y el iterador",true);
}

void pruebas_volumen_y_buscar(){
	printf("Prueba de arbol a volumen\n");

	abb_t* arbol=abb_crear(strcmp,NULL);

	size_t largo=10;
	char* claves[largo];
	int* valores[largo];

	size_t i;
	for (i = 0; i < largo; i++) {
		claves[i] = malloc(10*sizeof(char));
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%08d", i);
		*valores[i] = i;
	}

	vector_desordenar(claves,valores,largo);
	bool todoBien=true;
	bool ok= true;
	i=0;
	while (i<largo && todoBien){
		todoBien=abb_guardar(arbol, claves[i], valores[i]);
		ok = abb_pertenece(arbol, claves[i]);
		if (!ok) break;
		if (!todoBien) break;
		i++;
	}
	print_test("Puedo agregar 10000 valores", todoBien);
	print_test("Las claves efectivamente se guardan", ok);
	print_test("La cantidad de elementos del arbol es 10000", abb_cantidad(arbol)==largo);

	i=0;
	todoBien=true;
	while(todoBien && i<largo){
		todoBien = abb_pertenece(arbol, claves[i]);
		todoBien = (abb_obtener(arbol, claves[i]) == valores[i]);
		printf("Clave: %s, Valor: %d", claves[i], valores[i]);
		puts("");
		if (!todoBien) break;
		i++;
	}

	print_test("Los elementos estan bien guardados, y pertenecen",todoBien);
	print_test("Hay 10000 elementos en el arbol", abb_cantidad(arbol)==largo);

	/* Verifica que borre y devuelva los valores correctos */
	i=0; todoBien=true;
	while (i < largo ) {
		printf("Clave: %s", claves[i]);
		printf(" Valor borrado: %d", abb_borrar(arbol, claves[i]));

		puts("");		
		i++;
	}
	print_test("Los elementos al borrarlos dieron todos bien", todoBien);
	print_test("Ahora el arbol esta vacio", abb_cantidad(arbol)==0);

	abb_destruir(arbol);
	arbol = abb_crear(strcmp,free);

	/* Inserta 'largo' parejas en el hash */
	todoBien = true;
	i=0;
	while(i < largo && todoBien) {
		todoBien = abb_guardar(arbol, claves[i], valores[i]);
		i++;
	}

	/* Libera las cadenas */
	for (i = 0; i < largo; i++) {
		free(claves[i]);
	}
	
	/* Destruye el arbol - deberÃ­a liberar los enteros */
	abb_destruir(arbol);

}

void prueba_iterar_volumen(){
	printf("Prueba de iterar un arbol a volumen\n");
	abb_t* arbol=abb_crear(strcmp,free);

	size_t largo=10000;
	char* claves[largo];
	int* valores[largo];
	char* clavesaux[largo];
	int* valoresaux[largo];

	size_t i;
	for (i = 0; i < largo; i++) {
		claves[i] = malloc(10*sizeof(char));
		valores[i] = malloc(sizeof(int));

		sprintf(claves[i], "%08d", i);
        
		*valores[i] = i;
		clavesaux[i] = malloc(10*sizeof(char));
		valoresaux[i] = malloc(sizeof(int));
		sprintf(clavesaux[i], "%08d", i);
		*valoresaux[i] = i;
			
	}
	vector_desordenar(claves,valores,largo);
	bool todoBien=true;
	i=0;
	
	while (i<largo && todoBien){
		todoBien=abb_guardar(arbol, claves[i], valores[i]);
		i++;
	}
	print_test("Puedo agregar 10000 valores", todoBien);
	print_test("La cantidad de elementos del arbol es 10000", abb_cantidad(arbol)==largo);

	abb_iter_t* iter=abb_iter_in_crear(arbol);

	i=0;
	todoBien=true;
	while (i<largo && todoBien && !abb_iter_in_al_final(iter)){
		todoBien=strcmp(abb_iter_in_ver_actual(iter), clavesaux[i])==0;
		abb_iter_in_avanzar(iter);
		i++;
	}
	print_test("Los valores estan bien iterados", todoBien);
	print_test("La cantidad iterada fue 10000", i==largo);

	for (i=0; i<largo;i++){
		free(claves[i]);
		free(clavesaux[i]);
		free(valoresaux[i]);
	}

	abb_iter_in_destruir(iter);
	abb_destruir(arbol);

}

void pruebas_iterador(){
	prueba_iterar_arbol_vacio(); // listo
	prueba_iterar_unico_elemento();// listo
	prueba_iterar_arbol_pocos_elementos();//listo
	prueba_iterar_volumen();
}

void pruebas(){
	//~ pruebas_vacio(); //listo
	//~ puts("");
	//~ pruebas_unico_valor(); //listo
	//~ puts("");
	//~ pruebas_pocos_valores(); //listo
	puts("");
	pruebas_volumen_y_buscar(); //
	puts("");
	//~ pruebas_iterador(); 
}

int main(void){
	pruebas();
	return 0;
}
