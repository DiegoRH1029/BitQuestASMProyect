#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#define FILAS 20
#define COLUMNAS 20
//Declaracion de funciones ensamblador
int contarChar(char *mapa, int totCeldas, char aBuscar);
int validarMov(char *mapa, int numColumnas, int fila, int columna);
int detectarObj(char *mapa, int columnas, int fila, int columna, char aBuscar); 
int contCeldasLibres(char *mapa, int totCeldas);
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
void cambiarColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void imprimirMapaDiseno(char *mapaPtr, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            // sacamos el caracter de la matriz usando el indice 1D
            char caracter = *(mapaPtr + (i * columnas) + j);
            
            switch (caracter) {
                case '#': // PARED: bloque doble gris oscuro
                    cambiarColor(8); 
                    printf("%c%c", 219, 219); 
                    break;
                    
                case '.': // CAMINO: un puntito para guiar al jugador
                    cambiarColor(7); 
                    printf(" ."); 
                    break;
                    
                case 'P': // JUGADOR: la carita feliz verde brillante
                    cambiarColor(10); 
                    printf("%c ", 2); 
                    break;
                    
                case 'M': // MONEDA: una O amarilla para el oro
                    cambiarColor(14); 
                    printf("O "); 
                    break;
                    
                case 'K': // LLAVE: color cian para que resalte
                    cambiarColor(11); 
                    printf("K "); 
                    break;
                    
                case 'D': // PUERTA: color rojo porque esta cerrada
                    cambiarColor(12); 
                    printf("D "); 
                    break;
                    
                case 'E': // SALIDA: color morado para el final
                    cambiarColor(13); 
                    printf("E "); 
                    break;
                    
                default: // por si sale otra cosa rara
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
int main(){
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
    system("chcp 437 > nul"); 
    
    printf("\n=== BITQUEST - NIVEL DE PRUEBA ===\n\n");
    
    // mandamos llamar la funcion con el diseño chido
    imprimirMapaDiseno(&mapa[0][0], FILAS, COLUMNAS);
    return 0;
}