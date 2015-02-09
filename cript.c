#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "criptfunc.h" // Contiene los algoritmos: cesarizar,murcielagisar,descesarizar, desmurcielagisar

// Implementacion secuencial para el encriptado/desencriptado de archivos 

int main(int argc, char *argv[])
    /*  Funcion principal
        Parametros:
            argc: entero que indica numero de comandos en la linea de argumentos
            argv: arreglo que contiene los argumentos
        
        Devuelve nada
    */    

{
	clock_t tic = clock(); // contador inicial del tiempo de ejecucion
    FILE *archivo,*salida;         // archivo de entrada y salida
    char entrada[255];     // arreglo que contiene los caracteres del archivo de entrada
    char *iterador;        // iterador sobre el arreglo de los caracteres de la entrada
    int i;				   // contador
 
    archivo = fopen(argv[2],"r");
    salida = fopen(argv[3],"w");


    // Para encriptar el archivo se llama a cesarizar y luego murcielagisar
    if(strcmp(argv[1],"-c")==0){
        while (!feof(archivo)){
            fscanf(archivo,"%s",entrada); 
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                cesarizar(iterador);
                murcielagisar(iterador);
                fprintf(salida,"%s",iterador);
                i++;
            }
            
        }
        fprintf(salida,"\n");
        fclose(salida);
    }


 	
 	// Para desencriptar el archivo se llama a desmurcielagisar y luego descesarizar
    if(strcmp(argv[1],"-d")==0){
        while (!feof(archivo)){
            fscanf(archivo,"%s",entrada); 
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                desmurcielagisar(iterador);
                descesarizar(iterador);
                fprintf(salida,"%s",iterador);
                i++;
            }
            fclose(salida);
        }
        //printf("\n");
    }
 
    fclose(archivo);

    clock_t toc = clock(); // contador final del tiempo de ejecucion
    printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
 
    return 0;
}