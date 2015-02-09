#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "criptfunc.h"

int main(int argc, char const *argv[])
{
    int tic,toc; // contador final del tiempo de ejecucion
    FILE *archivo,*salida;
    char entrada[255];
    char *iterador;
    int i;

    tic =Tomar_Tiempo();


 
    archivo = fopen(argv[2],"r");
    salida = fopen(argv[3],"w");
    if(strcmp(argv[1],"-c")==0){
        while(fscanf(archivo,"%s",entrada)==1){
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                cesarizar(iterador);
                murcielagisar(iterador);
                fprintf(salida,"%s",iterador);
                i++;
            }
        }
        printf("\n");
    }
 
    if(strcmp(argv[1],"-d")==0){
        while(fscanf(archivo,"%s",entrada)==1){
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                desmurcielagisar(iterador);
                descesarizar(iterador);
                fprintf(salida,"%s",iterador);
                i++;
            }
        }
        printf("\n");
    }
 
    fclose(archivo);
    fclose(salida);

    toc = Tomar_Tiempo(); 
    printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
    return 0;
}