#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "criptfunc.h"


pthread_mutex_t hiloInf = PTHREAD_MUTEX_INITIALIZER;
int nHilos;
int longArchivo;
char const *argv[];

/*Funcion que sustituye el truncado de c por redondeo hacia arriba*/
int divRoundClosest(int n, int d)
{
  return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
}

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
    int limite;
    char *string;             //Apuntador al string modificado
}dataMedia;

void *hilosInferiores(void *arg){
    int cotaInf,cotaSup;
    int i;
    dataInferior *dataArg;
    char *contenidoActual;
    FILE *entrada;

    dataArg = (dataInferior*) arg;

    cotaInf = dataArg->cotaInfString;
    cotaSup = dataArg->cotaSupString;

    contenidoActual = (char *) calloc(cotaSup-cotaInf+2,sizeof(char));

    

    entrada = fopen(argv[3],"r");

    fseek(entrada,cotaInf,SEEK_SET);
    for(i=0;i<cotaSup-cotaInf+1;i++){
        fread(&contenidoActual[i],1,1,entrada);
        //printf("%c\n",contenidoActual[i]);
        if  (strcmp(argv[1],"-c")==0) {
            cesarizar(&contenidoActual[i]);
        }
        else if (strcmp(argv[1],"-d")==0){
            desmurcielagisar(&contenidoActual[i]);
        }
    }
    fseek(entrada, 0, SEEK_SET);

    fclose(entrada);

    (dataArg)->string = contenidoActual;

}


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

    limite = (*((dataMedia*)arg)).limite;

    arregloHilos = (pthread_t *) malloc(nHilos * sizeof(pthread_t));
    comunicadorHijos = (dataInferior**) malloc(nHilos * sizeof(dataInferior*));

    for(i=0;i<nHilos;i++){
        //Creacion de las estructuras de informacion para los nuevos threads

        comunicadorHijos[i] = (dataInferior*) malloc(sizeof(dataInferior));

        //Asignacion de limites de trabajo de los hilos inferiores
        (comunicadorHijos[i])->cotaInfString = limite;
        limite +=  divRoundClosest(longArchivo,(nHilos * nHilos));
        (comunicadorHijos[i])->cotaSupString = limite;
        limite++;

        //Asignacion del tam inicial del string,se reducira luego en cada espaci
        tam = ( 
                (comunicadorHijos[i])->cotaSupString - 
                (comunicadorHijos[i])->cotaInfString 
              );

        (comunicadorHijos[i])->tamString = tam;



        //Almacenamos el valor de salida de la creacion del thread
        estado = pthread_create(
                                &arregloHilos[i], NULL, hilosInferiores,
                                comunicadorHijos[i]
                                );

        if (estado) {
            printf("Error en la creacion del hilo inferior!\n");
            abort();
        }
    }

    
    
    strSalida = (char*) malloc( longArchivo * sizeof(char));
    contactoMain.string = strSalida; //String para enviar al padre

    for (i=0; i< nHilos;i++){
        //Esperar por la finalizacion del primer thread
        estado = pthread_join(arregloHilos[i],NULL);
        if (estado){
            printf("Error, en la ejecucion del hilo!\n");
            abort();
        }

        //Verificamos tam del ultimo String
        tam        = (comunicadorHijos[i])->tamString;      //Aqui me quede a las 3 am
        strLectura = (comunicadorHijos[i])->string;

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



int main(int argc, argv){
    
    pthread_t *arregloHilos;

    int i;
    int limInf;
    int estado;

    FILE *entrada;

    dataMedia **comunicadorHijos;

    comunicadorHijos = (dataMedia**) malloc(nHilos * sizeof(dataMedia*));

    


    FILE *salida;

    //Iniciamos tantos hilos como indique arg2
    nHilos = atoi(argv[2]);

    entrada = fopen(argv[3],"r");
    fseek(entrada, 0, SEEK_END); // seek to end of file
    longArchivo = ftell(entrada); // get current file pointer
    fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
    fclose(entrada);

    limInf = 0;

    for(i=0;i<nHilos;i++){
        //Aqui creo las nuevas estructuras para cada thread
        comunicadorHijos[i] = (comunicadorHijos*) malloc(sizeof(comunicadorHijos));

        (comunicadorHijos[i])->limite = limInf;

        limInf +=  divRoundClosest(longArchivo, nHilos);

        estado = pthread_create(
                        &arregloHilos[i], NULL, hilosInferiores,
                        comunicadorHijos[i]
                               );
        if (estado) {
            printf("Error en la creacion del hilo medio!\n");
            abort();
        }
    }


    salida = fopen(argv[4],"w");

    for (i=0; i< nHilos;i++){
        //Esperar por la finalizacion del primer thread
        estado = pthread_join(arregloHilos[i],NULL);
        if (estado){
            printf("Error, en la ejecucion del hilo!\n");
            abort();
        }

        //Agregar al archivo el contenido de cada hijo finalizado siguiendo
        //el orden dado
        fprintf(salida,"%s", (*comunicadorHijos[i]).string );
        free ((*comunicadorHijos[i]).string);

    }

    fclose(salida);

}