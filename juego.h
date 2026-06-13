#ifndef JUEGO_H
#define JUEGO_H
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#define FILAS 60
#define COLUMNAS 60

//Declaracion de funciones ensamblador
int contarChar(char *mapa, int totCeldas, char aBuscar);
int validarMov(char *mapa, int numColumnas, int fila, int columna);
int detectarObj(char *mapa, int columnas, int fila, int columna, char aBuscar); 
int contCeldasLibres(char *mapa, int totCeldas);
int posCaracter(char *mapa, int totCeldas,char c);
int calcularPuntaje(int numMonedas, int numPasos);
//Mapa de prueba
extern char mapa[FILAS][COLUMNAS];
//Estructuras
//struct jugador, es para guardar la fila y columna actual del jugador
typedef struct {
    char* nombre;
    int fila;
    int col;
    int monedas;
    int llaves;
    int movs;
    int puntajeTot;
    int totalMovs;
    int totalMonedas;
}Jugador;
typedef struct {
    Jugador jugador;
    int maxMonedasNivel; // Extraemos el total de monedas del nivel
    int estado;          // 1 si continuo, -1 si salio o perdio
} ResultadoNivel;
typedef struct{
    int numNivel;
    int totalMonedas;
    int totalLlaves;
    int totalEnemigos;
}Mapa;
//Estructura de la camara, para guardar indices de donde se imprimira la camara
struct Camara{
    int fila;
    int col;
};

//Funciones c
void cambiarColor(int color);
void imprimirMapaDiseno(char *mapaPtr,int camFila,int camCol,int dirJugador);
void moverCursor00();
int cargarNivel(const char* nombreArchivo, const char* nivelABuscar);
struct Camara nuevaCamara(struct Camara camActual,int fila, int col);
void imprimirHUD(int nivel, int monedas, int llaves,int totLlaves,int totMonedas);
void imprimirMenu();
void imprimirTitulo(const char* titulo, int colorTitulo,int colorBorde);

//Funciones de flujo de juego
ResultadoNivel jugando(Jugador p1, int nivelActual);
int imprimirInfo(Jugador p1,Mapa mapaInfo,int completado);
void actualizarRanking(char *nombreJugador,int puntaje);
void imprimirVictoria(Jugador p1, int maxMonedasTotales, int nivelesTotales);

#endif