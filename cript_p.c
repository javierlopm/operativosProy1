#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "criptfunc.h" // Contiene los algoritmos: cesarizar,murcielagisar,descesarizar, desmurcielagisar

// Implementacion con procesos para el encriptado/desencriptado de archivos 

int main(int argc, char const *argv[])
    /*  Funcion principal
        Parametros:
            argc: entero que indica numero de comandos en la linea de argumentos
            argv: arreglo que contiene los argumentos
        
        Devuelve nada
    */
{
    clock_t tic = clock(); // contador inicial del tiempo de ejecucion
    pid_t ramas,hojas;     // id de los procesos para las ramas y hojas
    int i,j,nHijos,cotaInfRamas,cotaSupRamas,cotaInfHojas; /* enteros   i: contador 
                                                                        j: contador
                                                                        nHijos: numero de hijos del proceso
                                                                        cotaInfRamas: cota inferior para dividir los archivos de las ramas
                                                                        cotaSupRamas: cota superior para dividir los archivos de las ramas
                                                                        cotaInfHojas: cota infrior para dividir los archivos de las hojas
                                                            */
    int cotaSupHojas,largoArchivo,nArchHojas,nArchRamas;    /*  enteros:    cotaSupHojas: cota superior para dividir los archivos de las hojas
                                                                            largoArchivo: largo del archivo actual
                                                                            nArchHojas: numero de archivos de las hojas
                                                                            nArchRamas: numero de archivos de las ramas
                                                            */
    char *nombreArchivo, *aux,*contenidoActual,*token;      /* caracteres:  nombreArchivo: apuntador al nombre del archivo actual
                                                                            aux: apuntador auxiliar
                                                                            contenidoActual: arreglo que contiene el contenido del archivo actual
                                                                            token: contiene los segmentos de contenido actual
                                                            */
    FILE *escritor,*entrada;                                /* archivos:    escritor: archivo a escribir
                                                                            entrada: archivo a leer
                                                            */

    nHijos = atoi(argv[2]); // conversion a entero

        entrada = fopen(argv[3],"r");
        
        fseek(entrada, 0, SEEK_END);   // busca el final del archivo
        largoArchivo = ftell(entrada); // obtiene el puntero actual en el archivo
        fseek(entrada, 0, SEEK_SET);   // regresa al inicio del archivo
        
        fclose(entrada);
    
        ramas = 0;
        cotaInfRamas = 0;
        cotaSupRamas = largoArchivo/nHijos;
        nArchRamas   = 0;
    
        nArchHojas     = 0;
        
        // Se crean tantos hijos como indique el argumento de entrada
        for(i=0; i<nHijos ;i++){
            if ((ramas = fork()) < 0) {
                perror("fork:");
                exit(1);
            }
    
            if (ramas == 0) {
                break;
            }
            else{
                // Se determina el numero de archivos correspondiente para cada rama
                cotaInfRamas  = cotaSupRamas + 1;
                cotaSupRamas += largoArchivo/nHijos;
                nArchRamas   ++;
            }
    
        }
        
        // Si el proceso es una rama, se crean las hojas
        if(ramas==0) { 
            cotaInfHojas = cotaInfRamas;
            cotaSupHojas = cotaInfRamas + (largoArchivo/(nHijos * nHijos));
            for(i=0; i<nHijos ;i++){
                if ((hojas = fork()) < 0) {
                    perror("fork:");
                    exit(1);
                }
                if (hojas == 0) {
                    break;
                }
                else{
                    // Se determina el numero de archivos correspondiente para cada hoja
                    cotaInfHojas  = cotaSupHojas + 1;
                    cotaSupHojas += divRoundClosest(largoArchivo,(nHijos * nHijos));
                    if (cotaSupHojas > largoArchivo) cotaSupHojas = largoArchivo;
                    nArchHojas   ++;
                }
            }

            if (hojas == 0){


                contenidoActual = (char *) calloc(cotaSupHojas-cotaInfHojas+2,sizeof(char));

                entrada = fopen(argv[3],"r");

                fseek(entrada,cotaInfHojas,SEEK_SET);
                for(i=0;i<cotaSupHojas-cotaInfHojas+1;i++){
                    fread(&contenidoActual[i],1,1,entrada);

                    // Las hojas se encargan de cesarizar en el encriptado de texto
                    if  (strcmp(argv[1],"-c")==0) {
                        cesarizar(&contenidoActual[i]);
                    }
                    // Las hojas se encargan de desmurcielagisar en el desencriptado de texto
                    else if (strcmp(argv[1],"-d")==0){
                        desmurcielagisar(&contenidoActual[i]);
                    }
                }
                fseek(entrada, 0, SEEK_SET);

                // Creacion del nombre del archivo para cada proceso
                nombreArchivo    = (char *) calloc(4,sizeof(char));
                nombreArchivo[0] = (char) (nArchRamas + 0x30);
                nombreArchivo[1] = (char) (nArchHojas + 0x30);
                nombreArchivo[2] = 'h';

                escritor = fopen( nombreArchivo ,"w");

                fprintf(escritor, "%s",contenidoActual);
                fclose(escritor);

                free(contenidoActual);
                free(nombreArchivo);

                exit(0);
            }

            // Si el proceso es una rama, espera por sus hojas (hijos) y une los archivos creados por ellas
            else{   

                for(i=0; i < nHijos;i++) wait();
    
                nombreArchivo = (char*) malloc(sizeof(char));
                *nombreArchivo = (char) (nArchRamas + 0x30);
                escritor = fopen( nombreArchivo,"w");
                
                free(nombreArchivo);
    
                // Crea los nombres de los archivos que seran creados por los procesos
                for(i=0;i<nHijos;i++){
                    nombreArchivo = (char *) calloc(3,sizeof(char));
                    nombreArchivo[0] = (char) (nArchRamas + 0x30);
                    nombreArchivo[1] = (char) (i + 0x30);
                    nombreArchivo[2] = 'h';
                    entrada = fopen( nombreArchivo,"r");
    
                    largoArchivo = 0;
                    fseek(entrada, 0, SEEK_END);   // busca el final del archivo
                    largoArchivo = ftell(entrada); // obtiene el puntero actual en el archivo
                    fseek(entrada, 0, SEEK_SET);   // regresa al inicio del archivo
                    
                    contenidoActual = (char *) calloc(largoArchivo+1,sizeof(char));

                    fscanf(entrada,"%s",contenidoActual);
    
                    for (j = 0; j < largoArchivo; j++){
                        // Las ramas se encargan de murcielagisar en el encriptado de texto
                        if (strcmp(argv[1],"-c")==0) {
                            murcielagisar(&contenidoActual[j]);
                        }
                        // Las ramas se encargan de descesarizar en el desencriptado de texto
                        else if(strcmp(argv[1],"-d")==0){
                            descesarizar(&contenidoActual[j]);
                        }
                    }

                    fprintf(escritor,"%s",contenidoActual);

                    remove(nombreArchivo);
                    fclose(entrada);
                    
                    free(nombreArchivo);
                    free(contenidoActual);
                    
                }
            }
    
        }
        else{
            //Esperamos por hijos y unimos
            for(i=0; i < nHijos;i++) wait();
    
            escritor = fopen(argv[4],"w");
    
            // Leer los archivos de <nArchivoRamas>.txt
            for(i=0;i<nHijos;i++){

                nombreArchivo = (char*) malloc(sizeof(char));
                *nombreArchivo = (char) (i + 0x30);

                entrada = fopen( nombreArchivo,"r");
                
                largoArchivo = 0;
                fseek(entrada, 0, SEEK_END);   // sbusca el final del archivo
                largoArchivo = ftell(entrada); // obtiene el puntero actual del archivo
                fseek(entrada, 0, SEEK_SET);   // regresa al inicio del archivo
                
                contenidoActual = (char *) calloc(largoArchivo+1,sizeof(char));

                fscanf(entrada,"%s",contenidoActual);

                token = strtok(contenidoActual,"\"");

                while(token){
                    fprintf(escritor,"%s",token);
                    token = strtok(NULL, "\"");
                }
                remove(nombreArchivo);
    
                fclose(entrada);

                free(nombreArchivo);
            
            }
            
            fclose(escritor);
        
        clock_t toc = clock(); // contador final del tiempo de ejecucion
        printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
        }
    
    return 0;
}