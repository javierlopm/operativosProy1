#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void cesarizar(char *c){
    if (*c == 121){
        *c = 'a';
        //printf("%s",c);
    }
    else if (*c == 0x7A){
        *c = 'b';
        //printf("%s",c);
    }
    else{
        *c = *c + 2;
        //printf("%s",c);
    }
}

void descesarizar(char *c){
    if (*c == 97){
        *c = 'y';
        //printf("%s",c);
    }
    else if (*c == 98){
        *c = 'z';
        //printf("%s",c);
    }
    else{
        *c = *c - 2;
        //printf("%s",c);
    }
}

void murcielagisar(char *c){
    switch(*c){
        case 'm':
            *c = 0x30;
            break;
        case 'u':
            *c = 0x31;
            break;
        case 'r':
            *c = 0x32;
            break;
        case 'c':
            *c = 0x33;
            break;
        case 'i':
            *c = 0x34;
            break;
        case 'e':
            *c = 0x35;
            break;
        case 'l':
            *c = 0x36;
            break;
        case 'a':
            *c = 0x37;
            break;
        case 'g':
            *c = 0x38;
            break;
        case 'o':
            *c = 0x39;
            break;
    }
}

void desmurcielagisar(char *c){
    switch(*c){
        case 48:
            *c = 'm';
            break;
        case 49:
            *c = 'u';
            break;
        case 50:
            *c = 'r';
            break;
        case 51:
            *c = 'c';
            break;
        case 52:
            *c = 'i';
            break;
        case 53:
            *c = 'e';
            break;
        case 54:
            *c = 'l';
            break;
        case 55:
            *c = 'a';
            break;
        case 56:
            *c = 'g';
            break;
        case 57:
            *c = 'o';
            break;
    }
}

int main(int argc, char const *argv[])
{
    pid_t ramas,hojas;
    int i,j,nHijos,cotaInfRamas,cotaSupRamas,cotaInfHojas;
    int cotaSupHojas,largoArchivo,nArchHojas,nArchRamas;
    char *nombreArchivo, *aux,*contenidoActual;
    FILE *escritor,*entrada;

    nHijos = atoi(argv[2]);

    if(strcmp(argv[1],"-c")==0){

        //printf("EH........\n");

        entrada = fopen(argv[3],"r");
        
        fseek(entrada, 0, SEEK_END); // seek to end of file
        largoArchivo = ftell(entrada); // get current file pointer
        fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
        // proceed with allocating memory and reading the file
        printf("%d \n",largoArchivo);
        
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
                    break;
                }
                else{
                    cotaInfHojas  = cotaSupHojas + 1;
                    cotaSupHojas += largoArchivo/(nHijos * nHijos);
                    if (cotaSupHojas > largoArchivo) cotaSupHojas = largoArchivo;
                    nArchHojas   ++;
                }
            }
            
            if (hojas == 0){//Si soy hojas cesarizar

                contenidoActual = (char *) calloc(cotaSupHojas-cotaInfHojas+1,sizeof(char));
                
                //printf("cesar entre %d y %d \n",cotaInfHojas,cotaSupHojas);

                entrada = fopen(argv[3],"r");

                for(i=cotaInfHojas;i<cotaSupHojas;i++){
                    fread(&contenidoActual[i],1,1,entrada);
                    cesarizar(&contenidoActual[i]);
                }

                fclose(entrada);
                //fclose(entrada);//Maybe jejeps
    
                nombreArchivo = (char *) calloc(4,sizeof(char));
                nombreArchivo[0] = (char) (nArchRamas + 0x30);
                nombreArchivo[1] = (char) (nArchHojas + 0x30);
                nombreArchivo[2] = 'h';

                //printf("%s \n",nombreArchivo);
                
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
                        murcielagisar(&contenidoActual[i]);
                    }

                    //printf("Hola soy el sr. rama y termine mi archivo\n");
    
                    fprintf(escritor,"%s",contenidoActual);
    
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

                printf("%s\n",nombreArchivo);

                entrada = fopen( nombreArchivo,"r");

                free(nombreArchivo);

                
                largoArchivo = 0;
                fseek(entrada, 0, SEEK_END); // seek to end of file
                largoArchivo = ftell(entrada); // get current file pointer
                fseek(entrada, 0, SEEK_SET); // seek back to beginning of file
                // proceed with allocating memory and reading the file
                printf("%d \n",largoArchivo);
                
                //Leer
                contenidoActual = (char *) calloc(largoArchivo,sizeof(char));

                //printf("About to print\n");

                fscanf(entrada,"%s",contenidoActual);

                printf("Acabo de hacer el escan de %s \n",contenidoActual);
    
                fprintf(escritor,"%s",contenidoActual);
    
                //fclose(entrada);
                //free(nombreArchivo);
                free(contenidoActual);
    
            }
            fclose(escritor);
        }
    }
        
            
        
        

    return 0;
}