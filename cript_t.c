#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "criptfunc.h"  // Contiene los algoritmos: cesarizar,murcielagisar,descesarizar, desmurcielagisar

// Implementacion con hilos para el encriptado/desencriptado de archivos 
char opcionCript[30];   // contiene argumento -c o -d
char strEntrada[30];    // archivo de entrada
char strSalida[30];     // archivo de salida


typedef struct dataInferior{
    int cotaInfString,cotaSupString; // Posiciones entre las que leera el arch
    int tamString;                   // Tam real sin espacios del string modif.
    int longArchivo;                 // longitud del archivo
    int nHilos;                      // numero de hilos
    char *string;                    // Apuntador al string
}dataInferior;

typedef struct dataMedia{
    int limite;               // limites de trabajo de los hilos
    int longArchivo;          // longitud del archivo actual
    int nHilos;               // numero de hilos
    char *string;             // Apuntador al string modificado
}dataMedia;

void *hilosInferiores(void *arg){
    /*  Funcion a ser ejecutada por los hilos inferiores
        Parametros:
            arg: datos necesarios para que los hilos inferiores realicen su trabajo

        Devuelve nada
    */
    int cotaInf,cotaSup;            // cotas inferior y superior para el trabajo de los hilos
    int i;                          // contador
    dataInferior *dataArg;          // estructura para los datos de los hilos inferiores
    char *contenidoActual;          // apuntador al contenido del archivo actual
    FILE *entrada;                  // archivo de entrada

    dataArg = (dataInferior*) arg;  

    cotaInf = dataArg->cotaInfString;
    cotaSup = dataArg->cotaSupString;

    contenidoActual = (char *) malloc((cotaSup-cotaInf+2) * sizeof(char));

    entrada = fopen(strEntrada,"r");

    fseek(entrada,cotaInf,SEEK_SET);

    // Los hilos inferiores se encargan de cesarizar en el encriptado del archivo
    if  (strcmp(opcionCript,"-c")==0){
        
        for(i=0;i<cotaSup-cotaInf;i++){
            fread(&contenidoActual[i],1,1,entrada);
            cesarizar(&contenidoActual[i]);
        }
    }
    // Los hilos inferiores se encargan de desmurcielagisar en el desencriptado de archivo
    else if (strcmp(opcionCript,"-d")==0){
        for(i=0;i<cotaSup-cotaInf+1;i++){
            fread(&contenidoActual[i],1,1,entrada);
            desmurcielagisar(&contenidoActual[i]);
        }
    }

    fseek(entrada, 0, SEEK_SET);

    fclose(entrada);

    (dataArg)->string = contenidoActual;

}

void *hilosMedios(void *arg){
    /*  Funcion a ser ejecutada por lo hilos medios, inicializa la informacion necesaria para que cada thread inferior pueda trabajar
        Parametros:
            arg: datos necesarios para que los hilos medios realicen su trabajo

        Devuelve nada
     */
    
    pthread_t *arregloHilos; // arreglo de hilos

    int estado; // Estado resultante de creacion de cada thread
    int limite; // limite de trabajo de los hilos medios
    int tam,i,j,longArchivo,nHilos;     /*  enteros tam: tamaño del string sobre el cual se esta trabajando
                                                    i,j: contador
                                                    longArchivo: longitud del archivo actual
                                                    nHilos: numero de hilos indicado en los argumentos
                                        */

    char *strLectura,*strSalida;        // string del archivo de lectura y salida respectivamente

    FILE *entrada;                      // archivo de entrada

    dataMedia contactoMain;             // estructura para los datos de los hilos medios

    dataInferior **comunicadorHijos;    // arreglo de apuntadores para comunicarse con los thread inferiores

    contactoMain = *((dataMedia*) (arg));

    limite = (*((dataMedia*)arg)).limite;
    longArchivo = contactoMain.longArchivo;
    nHilos = contactoMain.nHilos;

    arregloHilos = (pthread_t *) malloc(nHilos * sizeof(pthread_t));
    comunicadorHijos = (dataInferior**) malloc(nHilos * sizeof(dataInferior*));

    //Creacion de las estructuras de informacion para los nuevos threads
    for(i=0;i<nHilos;i++){
        
        comunicadorHijos[i] = (dataInferior*) malloc(sizeof(dataInferior));

        //Asignacion de limites de trabajo de los hilos inferiores
        (comunicadorHijos[i])->cotaInfString = limite;
        //printf("%d\n",limite);
        //printf("longitud archivo %d numero de hilos %d\n",longArchivo,nHilos*nHilos);
        limite +=  divRoundClosest(longArchivo,(nHilos * nHilos));
        (comunicadorHijos[i])->cotaSupString = limite;
        //printf("%d\n",limite);
        limite++;

        //printf("Mi hijo trabajara entres %d y %d \n",(comunicadorHijos[i])->cotaInfString,(comunicadorHijos[i])->cotaSupString);

        //Asignacion del tam inicial del string,se reducira luego en cada espacio
        tam = ( 
                (comunicadorHijos[i])->cotaSupString - 
                (comunicadorHijos[i])->cotaInfString 
              );

        (comunicadorHijos[i])->tamString = tam;

        (comunicadorHijos[i])->nHilos      = nHilos;
        (comunicadorHijos[i])->longArchivo = longArchivo;


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
        tam        = (comunicadorHijos[i])->tamString;     
        strLectura = (comunicadorHijos[i])->string;

        //Realizamos el trabajo de criptografia sobre el texto modificado
        for(j=0;j<tam;j++){
            if (strcmp(opcionCript,"-c")==0) murcielagisar(&strLectura[j]);
            else if (strcmp(opcionCript,"-d")==0) descesarizar(&strLectura[j]);
        }

        //printf("%s\n",strLectura);----------------------------------------------------------------Con este debemos empezar a verificar que todo sale bien

        if (i==0)strcpy(strSalida,strLectura);
        else strcat(strSalida,strLectura);

        free(strLectura);
    }

    ((dataMedia*) arg)->string = strSalida;

}


int main(int argc, char const *argv[]){
    /*  Funcion principal
        Parametros:
            argc: entero que indica numero de comandos en la linea de argumentos
            argv: arreglo que contiene los argumentos
        
        Devuelve nada
    */    

    int tic = Tomar_Tiempo(); // contador inicial del tiempo de ejecucion
    pthread_t *arregloHilos; // arreglo de hilos

    int i; // contador
    int limInf; // inicia la seccion en la que cada hilo medio trabaja
    int estado; // obtiene el estatus resultante de cada creacion y espera por un hilo
    int nHilos; // numero de hilos que se crea en cada nivel inferior
    int longArchivo;

    FILE *entrada,*salida; // archivos de entrada y salida de texto

    dataMedia **comunicadorHijos; // arreglo de apuntadores que permite el envio de datos entre los hilos

    char *token; // contiene los segmentos de contenido actual

    comunicadorHijos = (dataMedia**) malloc(nHilos * sizeof(dataMedia*));
    

    strcpy(opcionCript,argv[1]);
    nHilos =     atoi(argv[2]);
    strcpy(strEntrada ,argv[3]);
    strcpy(strSalida  ,argv[4]);

    entrada = fopen(argv[3],"r");

    fseek(entrada, 0, SEEK_END);  // busca el final del archivo
    longArchivo = ftell(entrada); // obtiene el apuntador actual del archivo
    fseek(entrada, 0, SEEK_SET);  // regresa al inicio del archivo
    fclose(entrada);


    limInf = 0;

    arregloHilos = (pthread_t*) malloc(nHilos * sizeof(pthread_t));

    for(i=0;i<nHilos;i++){

        // Se crean las nuevas estructuras para cada hilo
        comunicadorHijos[i] = (dataMedia*) malloc(sizeof(comunicadorHijos));
        (comunicadorHijos[i])->limite = limInf;
        limInf +=  divRoundClosest(longArchivo, nHilos);
        (comunicadorHijos[i])->nHilos      = nHilos;
        (comunicadorHijos[i])->longArchivo = longArchivo;

        estado = pthread_create(
                        &arregloHilos[i], NULL, hilosMedios,
                        (void*)comunicadorHijos[i]
                               );

        if (estado) {
            printf("Error en la creacion del hilo medio!\n");
            abort();
        }
    }
    
    salida = fopen(strSalida,"w");

    for (i=0; i< nHilos;i++){

        //Esperar por la finalizacion del primer hilo
        estado = pthread_join(arregloHilos[i],NULL);

        if (estado){
            printf("Error, en la ejecucion del hilo!\n");
            abort();
        }

        //Agregar al archivo el contenido de cada hijo finalizado siguiendo
        //el orden dado

        //printf("Voya imprimir... %s \n",comunicadorHijos[i]->string);

        token = strtok((*comunicadorHijos[i]).string,"\"");

        while(token){
            fprintf(salida,"%s", token);
            token = strtok(NULL, "\"");
        }

        //printf("Impreso\n");
        free ((*comunicadorHijos[i]).string);
        //printf("Liberado\n");

    }
    fclose(salida);

    int toc = Tomar_Tiempo(); // contador final del tiempo de ejecucion
    printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
        
}