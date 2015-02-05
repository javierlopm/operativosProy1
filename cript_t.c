#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "criptfunc.h"

pthread_mutex_t hiloInf = PTHREAD_MUTEX_INITIALIZER;


typedef struct dataInferior{
    int idHilo;                      //Identificador del hilo Inferior
    int cotaInfString,cotaSupString; //Posiciones entre las que leera el arch
    int tamString;                   //Tam real sin espacios del string modif.
    char *cotaInfString;             //Apuntador al string

    pthread_mutex_t *candado;   //Ap a Semaforo para envio de informacion entre h
    pthread_cond_t  *condicion; //Ap a Condicion seteada en default


}dataInferior;

typedef struct dataMedia{
    int idHilo;                      //Identificador del hilo Medio
    char *cotaInfString;             //Apuntador al string modificado
}dataInferior;


/* 
 *   Funcion a ser ejecutada por lo hilos medios, inicializa la informacion 
 *   necesaria para que cada thread inferior pueda trabajar
 */
void *hilosMedios(void *arg){
    //Encriptamos o desencriptamos de acuerdo a 
    pthread_mutex_t hiloInf = PTHREAD_MUTEX_INITIALIZER; //Sem hilos med e inf
    pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

    for(i=0;i<nHilos;i++){
        //Creacion de las estructuras de informacion para los nuevos threads

        if (pthread_create( &mythread, NULL, thread_function, NULL/*Aqui va el contenido que le pasaremos*/)) {
            printf("Error en la creacion ");
            abort();
        }
    }


}

void *hilosInferiores(void *arg){

}


int main(int argc, char const *argv[]){
    int nHilos,i;
    //Iniciamos tantos hilos como indique arg2
    nHilos = atoi(argv[2]);

    for(i=0;i<nHilos;i++){
        //Aqui creo las nuevas estructuras para cada thread

        if (pthread_create( &mythread, NULL, thread_function, NULL/*Aqui va el contenido que le pasaremos*/)) {
            printf("Error en la creacion ");
            abort();
        }
    }

}