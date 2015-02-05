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
    char *string;             //Apuntador al string

    //pthread_mutex_t *candado;   //Ap a Semaforo para envio de informacion entre h
    //pthread_cond_t  *condicion; //Ap a Condicion seteada en default


}dataInferior;

typedef struct dataMedia{
    int idHilo;               //Identificador del hilo Medio
    char *string;             //Apuntador al string modificado
}dataMedia;


/* 
 *   Funcion a ser ejecutada por lo hilos medios, inicializa la informacion 
 *   necesaria para que cada thread inferior pueda trabajar
 */
void *hilosMedios(void *arg){
    //Arreglo de hilos
    pthread_t *arregloHilos;

    //Candados de thread
    //pthread_mutex_t hiloInf = PTHREAD_MUTEX_INITIALIZER; //Sem hilos med e inf
    //pthread_cond_t  cond    = PTHREAD_COND_INITIALIZER;  

    int estado; // Estado resultante de creacion de cada thread
    int limite;
    int tam,i,j,longArchivo;

    char *strLectura,*strSalida;

    FILE *entrada;

    dataMedia contactoMain;



    //Estructuras para comunicarse con los thread inferiores
    dataInferior **comunicadorHijos;

    contactoMain = *((dataMedia*) (arg));

    limite = 0; //No puede ser  0 SE DEBE PASAR POR ARGUMENTOOOOOOOOOOOOOOOOOOOOOOO
    


    arregloHilos = (pthread_t *) malloc(nHilos * sizeof(pthread_t));
    dataInferior = (dataInferior**) malloc(nHilos * sizeof(dataInferior*));

    for(i=0;i<nHilos;i++){
        //Creacion de las estructuras de informacion para los nuevos threads

        *comunicadorHijos[i] = (dataInferior*) malloc(sizeof(dataInferior));

        //Asignacion de limites de trabajo de los hilos inferiores
        (*comunicadorHijos[i])->cotaInfString = limite;
        limite +=  divRoundClosest(largoArchivo,(nHilos * nHilos));
        (*comunicadorHijos[i])->cotaSupString = limite;
        limite++;

        //Asignacion del tam inicial del string,se reducira luego en cada espaci
        tam = ( 
                (*comunicadorHijos[i])->cotaSupString - 
                (*comunicadorHijos[i])->cotaInfString 
              );

        (*comunicadorHijos[i])->tamString = tam;



        //Almacenamos el valor de salida de la creacion del thread
        estado = pthread_create(
                                &arregloHilos[i], NULL, hilosInferiores,
                                comunicadorHijos[i]
                                );

        if (estado)) {
            printf("Error en la creacion del hilo inferior!\n");
            abort();
        }
    }

    
    
    strSalida = (char*) malloc( longArchivo * sizeof(char));
    contactoMain.string = strSalida; //String para enviar al padre

    for (i=0; i< nHilos;i++){
        //Esperar por la finalizacion del primer thread
        estado = pthread_join(*arregloHilos[i],NULL);
        if (estado){
            printf("Error, en la ejecucion del hilo!\n");
            abort();
        }

        //Verificamos tam del ultimo String
        tam = (*dataInferior[i])->tamString;
        strLectura = (*dataInferior[i])->string;

        //Realizamos el trabajo de criptografia sobre el texto modificado
        for(j=0;j<tam;j++){
            if (strcmp(argv[1],"-c")==0) murcielagisar(&strLectura[i]);
            else if (strcmp(argv[1],"-d")==0) descesarizar(&strLectura[i]);
        }

        if (i==0)strcpy(strSalida,strLectura);
        else strcat(strSalida,strLectura);

        free(strLectura);
    }


}

void *hilosInferiores(void *arg){

}

int nHilos;
int longArchivo;

int main(int argc, char const *argv[]){
    
    pthread_t *arregloHilos;

    int i;

    dataMedia **comunicadorHijos;
    dataMedia = (dataMedia**) malloc(nHilos * sizeof(dataMedia*));


    FILE *salida;

    //Iniciamos tantos hilos como indique arg2
    nHilos = atoi(argv[2]);

    entrada = fopen(argv[3],"r");
    fseek(entrada, 0, SEEK_END); // seek to end of file
    longArchivo = ftell(entrada); // get current file pointer
    fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
    fclose(entrada);

    for(i=0;i<nHilos;i++){
        //Aqui creo las nuevas estructuras para cada thread

        estado = pthread_create(
                        &arregloHilos[i], NULL, hilosInferiores,
                        comunicadorHijos[i]
                               );
        if (estado)) {
            printf("Error en la creacion del hilo medio!\n");
            abort();
        }
    }


    salida = fopen(argv[4],"w");

    for (i=0; i< nHilos;i++){
        //Esperar por la finalizacion del primer thread
        estado = pthread_join(*arregloHilos[i],NULL);
        if (estado){
            printf("Error, en la ejecucion del hilo!\n");
            abort();
        }

        //Agregar al archivo el contenido de cada hijo finalizado siguiendo
        //el orden dado
        fprintf(salida,"%s", (*dataMedia[i])->string );
        free ((*dataMedia[i])->string);

    }

    fclose(salida);

}