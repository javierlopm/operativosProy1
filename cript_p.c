#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criptfunc.h"

/*Funcion que sustituye el truncado de c por redondeo hacia arriba*/
int divRoundClosest(int n, int d)
{
  return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
}

int main(int argc, char const *argv[])
{
    pid_t ramas,hojas;
    int i,j,nHijos,cotaInfRamas,cotaSupRamas,cotaInfHojas;
    int cotaSupHojas,largoArchivo,nArchHojas,nArchRamas;
    char *nombreArchivo, *aux,*contenidoActual;
    FILE *escritor,*entrada;

    nHijos = atoi(argv[2]);

    

        //printf("EH........\n");

        entrada = fopen(argv[3],"r");
        
        fseek(entrada, 0, SEEK_END); // seek to end of file
        largoArchivo = ftell(entrada); // get current file pointer
        fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
        // proceed with allocating memory and reading the file
        //printf("%d \n",largoArchivo);
        
        fclose(entrada);
    
        ramas = 0;
        cotaInfRamas = 0;
        cotaSupRamas = largoArchivo/nHijos;
        nArchRamas   = 0;
    
        //cotaInfHojas = 0;
        //cotaSupHojas   = largoArchivo/(nHijos * nHijos);
        nArchHojas     = 0;
    
        //largoArchivo = 0;
        
        for(i=0; i<nHijos ;i++){
            if ((ramas = fork()) < 0) {
                perror("fork:");
                exit(1);
            }
            // Codigo que ejecutaran los hijos para salir
            if (ramas == 0) {
                //printf("Soy el hijo con pid %d y mi padre es: %d\n", getpid(),getppid());
                //printf("Me cree como rama %d \n",getpid());
                //sleep(5);
                break;
            }
            else{
                //modificamos las cotas entre las que trabajara la rama
                cotaInfRamas  = cotaSupRamas + 1;
                cotaSupRamas += largoArchivo/nHijos;
                //nArchHojas   += nHijos;
                nArchRamas   ++;
            }
    
            //sleep y wait por los hijos
            //Cesarizar desde i + longitud/nhijos
        }
    
        if(ramas==0) { //Si soy una rama creo hojas
            cotaInfHojas = cotaInfRamas;
            cotaSupHojas = cotaInfRamas + (largoArchivo/(nHijos * nHijos));
            for(i=0; i<nHijos ;i++){
                if ((hojas = fork()) < 0) {
                    perror("fork:");
                    exit(1);
                }
                if (hojas == 0) {
                    //printf("Soy hoja con pid %d de la rama: %d\n", getpid(),getppid());
                    //sleep(10);
                    //printf("Soy la rama de nombre %d \n",nArchHojas);
                    break;
                }
                else{
                    cotaInfHojas  = cotaSupHojas + 1;
                    cotaSupHojas += divRoundClosest(largoArchivo,(nHijos * nHijos));
                    if (cotaSupHojas > largoArchivo) cotaSupHojas = largoArchivo;
                    nArchHojas   ++;
                }
            }
            
            if (hojas == 0){//Si soy hojas cesarizar


                contenidoActual = (char *) calloc(cotaSupHojas-cotaInfHojas+2,sizeof(char));
                
                //printf("cesar entre %d y %d \n",cotaInfHojas,cotaSupHojas);

                entrada = fopen(argv[3],"r");

                //printf("Bueno yo trabajo entre %d y %d \n",cotaInfHojas,cotaSupHojas);

                //for(i=cotaInfHojas;i<cotaSupHojas-1;i++){
                fseek(entrada,cotaInfHojas,SEEK_SET);
                for(i=0;i<cotaSupHojas-cotaInfHojas+1;i++){
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

                //printf("Sali! \n");
                //printf("Yo imprimo %s \n",contenidoActual);

                //fclose(entrada); No entiendo por que se queja de esta clausura
                //printf("Cerre!\n");
    
                nombreArchivo    = (char *) calloc(4,sizeof(char));
                nombreArchivo[0] = (char) (nArchRamas + 0x30);
                nombreArchivo[1] = (char) (nArchHojas + 0x30);
                nombreArchivo[2] = 'h';

                //printf("Mi archivo creado es el %s \n",nombreArchivo);
                //*aux = (char) nArchHojas;

                escritor = fopen( nombreArchivo ,"w");


                //printf("Hey soy una hoja y acabo de crear un archivo\n");

                fprintf(escritor, "%s",contenidoActual);
                fclose(escritor);

                free(contenidoActual);
                free(nombreArchivo);
                //printf("Libere\n");
                exit(0);
            }
            else{   //Si soy una rama espero por mis n hojas
                //Espero por hojas, uno y murcielagisar entre ramaInf y ramaSup
                for(i=0; i < nHijos;i++) wait();
    
                //Leer los archivos entre no <nArchivoRama><0..nHijos>h.txt
    
                //fclose(entrada);
                nombreArchivo = (char*) malloc(sizeof(char));
                *nombreArchivo = (char) (nArchRamas + 0x30);
                //printf("Abriendo %c \n",*nombreArchivo);
                escritor = fopen( nombreArchivo,"w");
                
                free(nombreArchivo);
    
                for(i=0;i<nHijos;i++){
                    nombreArchivo = (char *) calloc(3,sizeof(char));
                    nombreArchivo[0] = (char) (nArchRamas + 0x30);
                    nombreArchivo[1] = (char) (i + 0x30);
                    nombreArchivo[2] = 'h';
                    entrada = fopen( nombreArchivo,"r");
    
                    largoArchivo = 0;
                    fseek(entrada, 0, SEEK_END); // seek to end of file
                    largoArchivo = ftell(entrada); // get current file pointer
                    fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
                    // proceed with allocating memory and reading the file
                    //printf("%d \n",largoArchivo);
    
                    //Leer
                    contenidoActual = (char *) calloc(largoArchivo+1,sizeof(char));

                    fscanf(entrada,"%s",contenidoActual);
    
                    for (j = 0; j < largoArchivo; j++){
                        if (strcmp(argv[1],"-c")==0) {
                            murcielagisar(&contenidoActual[i]);
                        }
                        else if(strcmp(argv[1],"-d")==0){
                            descesarizar(&contenidoActual[i]);
                        }
                    }

                    //printf("Hola soy el sr. rama y termine mi archivo\n");
    
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

                //printf("Ya estoy uniendo todo, creo...\n");
                nombreArchivo = (char*) malloc(sizeof(char));
                *nombreArchivo = (char) (i + 0x30);

                //printf("%s\n",nombreArchivo);

                entrada = fopen( nombreArchivo,"r");

                

                
                largoArchivo = 0;
                fseek(entrada, 0, SEEK_END); // seek to end of file
                largoArchivo = ftell(entrada); // get current file pointer
                fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
                // proceed with allocating memory and reading the file
                //printf("%d \n",largoArchivo);
                
                //Leer
                contenidoActual = (char *) calloc(largoArchivo+1,sizeof(char));

                fscanf(entrada,"%s",contenidoActual);
                fprintf(escritor,"%s",contenidoActual);

                remove(nombreArchivo);
    
                fclose(entrada);

                free(nombreArchivo);
                //free(contenidoActual);
            }
            fclose(escritor);
        }
    
        
            
        
        

    return 0;
}