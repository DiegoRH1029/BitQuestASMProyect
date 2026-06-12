#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#define FILAS 20
#define COLUMNAS 20
//Declaracion de funciones ensamblador
int contarChar(char *mapa, int totCeldas, char aBuscar);
int validarMov(char *mapa, int numColumnas, int fila, int columna);
int detectarObj(char *mapa, int columnas, int fila, int columna, char aBuscar); 
int contCeldasLibres(char *mapa, int totCeldas);
int posCaracter(char *mapa, int totCeldas,char c);
//Mapa de prueba
char mapa[FILAS][COLUMNAS] = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','P','.','.','.','.','.','.','#','.','.','.','.','.','.','.','.','.','.','#'},
    {'#','#','#','.','#','#','#','#','.','#','.','#','#','#','#','#','#','#','.','#'},
    {'#','.','.','.','.','.','#','.','#','.','#','M','.','.','.','.','.','.','.','#'},
    {'#','.','#','#','#','#','.','#','.','#','.','#','.','#','#','#','#','#','.','#'},
    {'#','.','#','M','.','#','.','#','.','.','.','#','.','.','.','.','.','#','.','#'},
    {'#','.','#','.','#','#','.','#','#','#','#','#','.','#','#','#','.','#','.','#'},
    {'#','.','#','.','.','.','.','.','.','.','.','#','.','#','.','.','.','#','.','#'},
    {'#','.','#','#','#','#','#','#','#','#','.','#','.','#','.','#','#','#','.','#'},
    {'#','.','.','.','.','.','.','.','#','.','.','.','#','.','#','M','#','.','.','#'},
    {'#','#','#','#','.','#','#','#','.','#','#','#','#','#','.','#','.','#','.','#'},
    {'#','M','.','.','.','#','.','.','.','.','.','.','.','.','.','#','.','#','.','#'},
    {'#','#','#','#','#','#','.','#','#','#','#','#','#','#','#','#','.','#','.','#'},
    {'#','.','.','.','.','.','.','.','.','.','.','#','K','.','.','.','.','#','.','#'},
    {'#','.','#','#','#','#','#','#','#','#','.','#','#','#','#','#','#','#','.','#'},
    {'#','.','#','.','.','.','.','.','#','.','#','.','.','.','.','.','.','.','.','#'},
    {'#','.','#','.','#','#','#','#','.','#','.','#','.','#','#','#','#','#','.','#'},
    {'#','.','#','.','#','M','.','.','.','#','D','#','.','.','.','.','.','#','.','#'},
    {'#','.','#','.','#','#','#','#','#','#','.','#','#','#','#','#','.','#','E','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
//Estructuras
typedef struct{
    int fila;
    int col;
} Jugador;

//Funciones c
void cambiarColor(int color);
void imprimirMapaDiseno(char *mapaPtr, int filas, int columnas);
void moverCursor00();

int main(){
    /*
    //Proband si funciona funcion contar caracteres
    int totalMonedas = contarChar(&mapa[0][0], FILAS*COLUMNAS, 'M');   
    printf("Total monedas : %d\n",totalMonedas);
    printf("Total paredes: %d\n",contarChar(&mapa[0][0], FILAS*COLUMNAS, '#'));
    printf("Total Caminos: %d\n",contarChar(&mapa[0][0], FILAS*COLUMNAS, '.'));
    //Probando si funciona funcion validar movimiento
    printf("movimiento posible : %d %c\n", validarMov(&mapa[0][0],COLUMNAS,1,3),mapa[1][3]);
    printf("movimiento posible : %d %c\n", validarMov(&mapa[0][0],COLUMNAS,0,0),mapa[0][0]);
    //Probando funcion validar objeto en casilla 
    printf("El objeto a buscar . es:%d %c\n",detectarObj(&mapa[0][0],COLUMNAS,1,3,'.'),mapa[1][3]);
    printf("El objeto a buscar # es:%d %c\n",detectarObj(&mapa[0][0],COLUMNAS,0,0,'#'),mapa[0][0]);
    printf("Numero celdas libres: %d",contCeldasLibres(&mapa[0][0],FILAS*COLUMNAS));
    // esto configura la consola para los caracteres extendidos
    */
    system("chcp 437 > nul"); 
    printf("\n=== BITQUEST - NIVEL DE PRUEBA ===\n\n");
    Jugador p1;
    int jugando=1;
    //obtenemos la posisicion inicial del jugador desde el mapa 
    int indJugador = posCaracter(&mapa[0][0],FILAS*COLUMNAS,'P');
    if(indJugador!=-1){
        p1.fila = indJugador/COLUMNAS;
        p1.col = indJugador%COLUMNAS;
    }else{ 
        printf ("\nEl jugador no existe");
        return 1;
    } 
    // mandamos llamar la funcion con el diseño 
    while(jugando){
        moverCursor00();
        imprimirMapaDiseno(&mapa[0][0], FILAS, COLUMNAS);
        char tecla = _getch();
        int nuevaFila = p1.fila;
        int nuevaCol = p1.col;
        if(tecla=='w'||tecla=='W') nuevaFila--;
        else if(tecla=='s'||tecla=='S') nuevaFila++;
        else if(tecla=='a'||tecla=='A') nuevaCol--;
        else if(tecla=='d'||tecla=='D') nuevaCol++;
        else if(tecla=='q'||tecla=='Q') break;
        int movimientoValido=validarMov(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol);
        if(movimientoValido==1){
            mapa[p1.fila][p1.col] = '.';
            p1.fila=nuevaFila;
            p1.col=nuevaCol;
            mapa[p1.fila][p1.col]='P';
        }

    }
    system("pause");
    return 0;
}
void cambiarColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void imprimirMapaDiseno(char *mapaPtr, int filas, int columnas) {
    for (int i = 0;i<filas;i++) {
        for (int j = 0;j<columnas; j++) {
            // sacamos el caracter de la matriz usando el indice 1D
            char caracter = *(mapaPtr+(i*columnas)+j);
                switch (caracter) {
                case '#': // PARED: Pintando el fondo
                    cambiarColor(128); // 128 es el fondo gris oscuro
                    printf("  "); // 
                    break;
                    
                case '.': // CAMINO: Totalmente vacio
                    cambiarColor(7);
                    printf("  "); 
                    break;
                    
                case 'P': // JUGADOR
                    cambiarColor(10); 
                    printf("P "); 
                    break;
                    
                case 'M': // MONEDA
                    cambiarColor(14); 
                    printf("O "); 
                    break;
                    
                case 'K': // LLAVE
                    cambiarColor(11); 
                    printf("K "); 
                    break;
                    
                case 'D': // PUERTA: Dos barras verticales
                    cambiarColor(12); 
                    printf("||"); 
                    break;
                    
                case 'E': // SALIDA
                    cambiarColor(13); 
                    printf(">>"); 
                    break;
                    
                default: 
                    cambiarColor(7);
                    printf("%c ", caracter);
                    break;
            }
        }
        // salto de linea al terminar la fila
        printf("\n");
    }
    
    // regresamos al color blanco de siempre para no buguear la consola
    cambiarColor(7);
}
void moverCursor00(){
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0,0};
    SetConsoleCursorPosition(hconsole,pos);
}