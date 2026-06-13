#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "juego.h"

//Matriz de nuestro mapa
char mapa[FILAS][COLUMNAS];
//Funcion que retorna la nueva estrucura de camara;
void cambiarColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
struct Camara nuevaCamara(struct Camara camActual,int fila, int col){
        if(fila<camActual.fila+5) camActual.fila = fila-5;
        if(fila>camActual.fila+15) camActual.fila=fila-15;
        if(col<camActual.col+5) camActual.col = col-5;
        if(col>camActual.col+15) camActual.col=col-15;
        //Nos aseguramos que la camara no este en un limite del mapa invalido (60-20)
        if(camActual.fila<0)camActual.fila=0;
        if(camActual.fila>FILAS-20)camActual.fila=FILAS-20;
        if(camActual.col<0)camActual.col=0;
        if(camActual.col>COLUMNAS-20)camActual.col=COLUMNAS-20;
    return camActual;
}
//Esta funcion ahora solo imprimira 20*20 de la matriz, recibe el indice de la camara fila y columna
// Ahora recibe la direccion del jugador (0=Abajo, 1=Arriba, 2=Izq, 3=Der)
void imprimirMapaDiseno(char *mapaPtr, int camFila, int camCol, int dirJugador) {
    cambiarColor(9); 
    
    // 1. Techo: 20 bloques * 6 de ancho = 120 caracteres de largo
    printf("%c", 201); 
    for(int i=0; i<120; i++) printf("%c", 205); 
    printf("%c\n", 187); 

    // 2. Mapa
    for (int i = camFila; i < camFila + 20; i++) {
        cambiarColor(9);
        printf("%c", 186); // Pared izquierda del marco
        
        for (int j=camCol; j<camCol+20; j++) {
            char caracter = *(mapaPtr+(i*COLUMNAS)+j);
            
            switch (caracter) {
                case '#': //PAREDES: 6 bloques solidos pegados
                    cambiarColor(8); 
                    printf("%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219); 
                    break;
                    
                case '.': // CAMINO: 6 espacios vacios
                    cambiarColor(7);
                    printf("      "); 
                    break;
                    
                case 'P': // JUGADOR 
                    cambiarColor(10); 
                    if (dirJugador == 0) printf("( ._.)"); // Abajo
                    else if (dirJugador == 1) printf("( '-')"); // Arriba 
                    else if (dirJugador == 2) printf("('_' )"); // Izquierda
                    else if (dirJugador == 3) printf("( '_')"); // Derecha
                    break;
                    
                case 'M': // MONEDA
                    cambiarColor(14); 
                    // Para que parezca mas solida, usamos el caracter 254 (un cuadro centrado)
                    printf("  %c   ", 254); 
                    break;
                    
                case 'K': // LLAVE
                    cambiarColor(11); 
                    printf(" O->  "); 
                    break;
                    
                case 'D': // PUERTA
                    cambiarColor(12); 
                    printf(" |||| "); 
                    break;
                    
                case 'E': // SALIDA
                    cambiarColor(13); 
                    printf("  >>  "); 
                    break;
                    
                default: 
                    cambiarColor(7);
                    printf("  %c   ", caracter);
                    break;
            }
        }
        cambiarColor(9); 
        printf("%c\n", 186); // Pared derecha del marco
    }
    
    // 3. Piso de la ventana
    cambiarColor(9);
    printf("%c", 200); 
    for(int i = 0; i < 120; i++) printf("%c", 205); 
    printf("%c\n", 188); 
    
    cambiarColor(7);
}
void moverCursor00(){
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0,0};
    SetConsoleCursorPosition(hconsole,pos);
}
//Funcion que se encarga de imprimir la HUD (total llaves, nivel actual etc)
void imprimirHUD(int nivel, int monedas, int llaves, int totLlaves, int totMonedas) {
    cambiarColor(9); // Azul para el marco

    //Techo (39 de ancho)
    printf("%c", 201); 
    for(int i=0; i<39; i++) printf("%c", 205); 
    printf("%c\n", 187); 

    //Titulo centrado
    printf("%c", 186); 
    cambiarColor(15);  
    printf("         BITQUEST - NIVEL %-2d           ", nivel); 
    cambiarColor(9); 
    printf("%c\n", 186); 

    //Divisor (19 izquierda, 1 centro, 19 derecha)
    printf("%c", 204); 
    for(int i=0; i<19; i++) printf("%c", 205); 
    printf("%c", 203); 
    for(int i=0; i<19; i++) printf("%c", 205);
    printf("%c\n", 185); 

    //Estadisticas con los parametros
    printf("%c", 186); // Pared izq
    
    cambiarColor(14); // Amarillo
    // El %2d alinea los numeros a la derecha y %-2d a la izq, asi no se mueve el texto
    printf(" Monedas (O): %2d/%-2d", monedas, totMonedas); 
    
    cambiarColor(9);// Regresamos al marco
    printf("%c", 186);// Pared central
    
    cambiarColor(11); // Celeste
    printf(" Llaves (K): %2d/%-2d ", llaves, totLlaves); 
    
    cambiarColor(9); 
    printf("%c\n", 186); // Pared der

    //Piso
    printf("%c", 200); 
    for(int i=0; i<19; i++) printf("%c", 205);
    printf("%c", 202); 
    for(int i=0; i<19; i++) printf("%c", 205);
    printf("%c\n", 188); 

    cambiarColor(7); // Color normal
}
//Funcion que se encargara de cargar un nivel desde el archivo niveles.txt
//recibe el nombre del archivo y el nivel a bscar (--nivel 1, --nivel 2 etc)
int cargarNivel(const char* nombreArchivo, const char* nivelABuscar){
    FILE *archivo = fopen(nombreArchivo,"r");
    if(archivo==NULL){
        printf("Archivo no encontrado o no valido");
        return 0;
    }
    char buffer[100]; //Buffer para guardar una linea completa del archivo
    int nivelEncontrado=0;
    //Validamos primeramente en que linea esta nuestra etiqueta del nivel 
    while(fgets(buffer,sizeof(buffer),archivo)!=NULL){
        if(strstr(buffer,nivelABuscar)!=NULL){
            nivelEncontrado=1;
            break;
        }
    }
    if(!nivelEncontrado){
        printf("Nivel encontrado");
        fclose(archivo);
        return 0;
    }
    //leemos las 60 filas y las metemos en la matriz
    for(int i=0; i<FILAS;i++){
        if(fgets(buffer,sizeof(buffer),archivo)==NULL){
            printf("Error faltan filas");
            fclose(archivo);
            return 0;
        }
        if(strstr(buffer,"--nivel ")!=NULL){
            printf("Error :Mapa incompleto");
            fclose(archivo);
            return 0;
        }
        //Metemos caracter por caracter
        for(int j=0;j<COLUMNAS;j++){
            //Filtramos los saltos de linea o otros caracteres
            if(buffer[j]=='\n'||buffer[j]=='\r'||buffer[j]=='\0'){
                mapa[i][j] = '.'; 
            }else{
                mapa[i][j]=buffer[j];
            }
        }
    }
    fclose(archivo);
    return 1;
}