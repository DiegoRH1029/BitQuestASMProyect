#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "juego.h"

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
    // Forzar maximizado de la consola nativa de Windows
    Jugador p1;
    struct Camara cam;
    Mapa mapaInfo;
    int jugando=1;
    int direccion=1;
    if(cargarNivel("niveles.txt","--nivel 2")!=1){
        return 0;
    }
    int nivelActual=2;
    //obtenemos la posisicion inicial del jugador desde el mapa 
    int indJugador = posCaracter(&mapa[0][0],FILAS*COLUMNAS,'P');
    if(indJugador!=-1){
        p1.fila = indJugador/COLUMNAS;
        p1.col = indJugador%COLUMNAS;
        p1.monedas=0;
        p1.llaves=0;
    }else{ 
        printf ("\nEl jugador no existe");
        return 1;
    } 
    mapaInfo.totalMonedas = contarChar(&mapa[0][0], FILAS*COLUMNAS, 'M');
    mapaInfo.totalLlaves = contarChar(&mapa[0][0], FILAS*COLUMNAS, 'K');
    //Centramos la camara respecto al jugador
    cam.fila = p1.fila-10;
    cam.col = p1.col -10;

    // mandamos llamar la funcion con el diseño 
    printf("Precione cualquier tecla para iniciar");
    system("pause");
    system("cls");
    while(jugando){
        moverCursor00();
        cam = nuevaCamara(cam,p1.fila,p1.col);
        imprimirHUD(nivelActual,p1.monedas,p1.llaves,mapaInfo.totalLlaves,mapaInfo.totalMonedas);
        imprimirMapaDiseno(&mapa[0][0],cam.fila,cam.col,direccion);
        if(_kbhit()){ //Detecta si una tecla se preciono
            char tecla = _getch();//guardamos tecla 
            int nuevaFila = p1.fila;//fila
            int nuevaCol = p1.col;//comulmans
            // Actualizamos coordenadas y la direccion visual
            if(tecla=='w'||tecla=='W') { nuevaFila--; direccion = 1; }
            else if(tecla=='s'||tecla=='S') { nuevaFila++; direccion = 0; }
            else if(tecla=='a'||tecla=='A') { nuevaCol--; direccion = 2; }
            else if(tecla=='d'||tecla=='D') { nuevaCol++; direccion = 3; }
            else if(tecla=='q'||tecla=='Q') break;
            //Llamamos si el movimiento es valido
            char objeto='0';
            int puedeAvanzar=0;
            int movimientoValido=validarMov(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol);
            if(movimientoValido==1){//Si es valido vamos al siguiente movimiento
                if(detectarObj(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol,'.')) objeto ='.';
                if(detectarObj(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol,'E')) objeto ='E';
                if(detectarObj(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol,'M')) objeto ='M';
                if(detectarObj(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol,'K')) objeto ='K';
                if(detectarObj(&mapa[0][0],COLUMNAS,nuevaFila,nuevaCol,'D')) objeto ='D';

                if(objeto!='0'){
                    switch (objeto){
                    case '.'://El objeto es un camino
                        puedeAvanzar=1;
                        break;
                    case 'E'://El objeto es la salida
                        break;
                    case 'M'://El objeto es una moneda
                        puedeAvanzar=1;
                        p1.monedas++;
                        break;
                    case 'K'://El objeto es una llave
                        puedeAvanzar=1;
                        p1.llaves++;
                        break;
                    case 'D'://El objeto es una puerta
                        puedeAvanzar = (p1.llaves>0);
                        if(p1.llaves!=0)p1.llaves--;
                        break;
                    default:
                        break;
                    }
                }
                    if(puedeAvanzar){
                        mapa[p1.fila][p1.col] = '.';
                        p1.fila=nuevaFila;
                        p1.col=nuevaCol;
                        mapa[p1.fila][p1.col]='P';
                    }

            }
        }
        Sleep(25);
    }
    system("pause");
    return 0;
}