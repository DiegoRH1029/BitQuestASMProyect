#include <stdlib.h>
#include <stdio.h>

//Declaracion de funciones ensamblador
int contarChar(char *mapa, int totCeldas, char aBuscar);

int main(){
    //Proband si funciona funcion contar caracteres
    char mapa[] = "MMMM####.....";
    int totCeldas = sizeof(mapa)-1;
    int totalMonedas = contarChar(mapa, totCeldas, 'M');   
    printf("Total monedas : %d",totalMonedas);
    printf("Total paredes: %d",contarChar(mapa, totCeldas, '#'));
    printf("Total Caminos: %d",contarChar(mapa, totCeldas, '.'));
    return 0;
}