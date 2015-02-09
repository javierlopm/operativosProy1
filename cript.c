#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "criptfunc.h" // Biblioteca de funciones propias

// Implementacion secuencial para el encriptado/desencriptado de archivos 

/*  Funcion principal
 *   Parametros:
 *       argc: entero que indica numero de comandos en la linea de argumentos
 *       argv: arreglo que contiene los argumentos
 */
int main(int argc, char *argv[]) {
    clock_t tic = clock(); // contador inicial del tiempo de ejecucion
    FILE *archivo,*salida; // archivo de entrada y salida
    char *entrada;     // arreglo que contiene los caracteres del archivo de ent
    char *iterador;    // iterador sobre el arreglo de los caracteres de la ent.
    int i;             // contador
    int largoArchivo;
 
    archivo = fopen(argv[2],"r");

    fseek(archivo, 0, SEEK_END);   // busca el final del archivo
    largoArchivo = ftell(archivo); // obtiene el puntero actual en el archivo
    fseek(archivo, 0, SEEK_SET);   // regresa al inicio del archivo

    salida  = fopen(argv[3],"w");

    entrada = (char*) malloc(sizeof(char)*(largoArchivo+1));


    // Para encriptar el archivo se llama a cesarizar y luego murcielagisar
    if(strcmp(argv[1],"-c")==0){
            while(fscanf(archivo,"%s",entrada)==1){ 
            printf("Llegue a la primera linea\n");
            i = 0;

            while(i<strlen(entrada)){
                *iterador = entrada[i];
                cesarizar(iterador);
                murcielagisar(iterador);
                fprintf(salida,"%s",iterador);
                i++;
            }

            
        }
        fclose(salida);
    }
    else if(strcmp(argv[1],"-d")==0){
        
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
        fclose(salida);
    }


    
    free(entrada);
    fclose(archivo);

    clock_t toc = clock(); // contador final del tiempo de ejecucion
    printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
 
    return 0;
}