#include <stdio.h>
#include "juego.h"

int main(){
// Configuracion inicial de la ventana de Windows
    system("chcp 437 > nul"); 
    
    int modoGrafico = 0;
    char motor;
    
    // Selector de Motor grafico
    system("cls");
    cambiarColor(15);
    printf("=== SELECCION DE MOTOR DE RENDERIZADO ===\n\n");
    cambiarColor(7);
    printf("1) Modo Consola\n");
    printf("2) Modo Allegro (En desarrollo)\n");
    printf("\nElige una opcion (1-2): ");
    
    motor=_getch(); // Lee la tecla sin necesidad de dar Enter
    if(motor=='2'){
        modoGrafico=1;
    }

    // Variables para el menu principal
    char opcion;
    char nombreJugador[50];
    int salir = 0;

    //Loop del menu principal
    while(!salir){
        system("cls");
        
        imprimirMenu();
        //pa saber en que modo estamos corriendo
        if(modoGrafico) printf("\n[Motor: Allegro Activo]\n");
        else printf("\n[Motor: Consola Activo]\n");
        printf("\nSelecciona una opcion: ");
        
        // toupper convierte 'p' minuscula a 'P' mayuscula automaticamente
        opcion = toupper(_getch()); 

        //El Switch Principal
        switch (opcion) {
            case 'P':
                system("cls");
                cambiarColor(10);
                imprimirTitulo("=== NUEVA PARTIDA ===",9,9);
                cambiarColor(7);
                printf("Ingresa tu nombre de jugador: ");
                
                // Leemos el nombre con espacios permitidos
                fflush(stdin);
                fgets(nombreJugador, 50, stdin);
                
                // fgets guarda el enter  al final, con esto se lo quitamos:
                nombreJugador[strcspn(nombreJugador, "\n")] = 0; 
                
                if (modoGrafico) {
                    // Aqui despues se conecta a allegro
                    cambiarColor(11);
                    printf("\nIniciando ventana de Allegro para %s...\n", nombreJugador);
                    cambiarColor(7);
                    system("pause");
                } else {
                    // Arrancamos tu funcion jugando de juego.c
                    // Le pasamos el nombre, el string del archivo txt y el entero para el HUD
                    int nivel=1;
                    int puntaje=0;
                    int jugandoNiveles=1;
                    while(jugandoNiveles){
                        char nombreNivel[20];
                        sprintf(nombreNivel,"--nivel %d",nivel);
                        int estadoCarga = cargarNivel("niveles.txt",nombreNivel);
                        if(estadoCarga==0){//Si es 0 ya no hay mas niveles por lo tanto es victoria
                            actualizarRanking(nombreJugador,puntaje);
                            imprimirVictoria(nombreJugador,puntaje);
                            jugandoNiveles=0;
                        }
                        else if(estadoCarga==-1){ //Archivo roto o incompleto
                            system("pause");
                            jugandoNiveles=0;
                        }
                        else{//El mapa ya esta cargadoo asi que empezamos a jugar
                            //jugando regresa el puntaje o -1 si ocurrio un error
                            int resultado=jugando(nombreJugador,nivel);
                            if(resultado==-1){//Ocurrio un error o no termino nivel
                                jugandoNiveles=0;
                            }else{
                                nivel++;
                                puntaje+=resultado;
                            }
                        }
                    }
                }
                    break;
            case 'R': //Para ver los rankigs
                system("cls");
                cambiarColor(14);
                printf("=== RANKINGS DE JUGADORES ===\n\n");
                cambiarColor(7);
                printf("Aqui se imprimira el archivo de puntajes en el futuro.\n\n");
                system("pause");
                break;

            case 'E': //Para salir
                salir = 1;
                system("cls");
                cambiarColor(12);
                printf("Cerrando motor...\n");
                cambiarColor(7);
                break;

            default:
                // Si presionan una tecla invalida, el while simplemente vuelve a pintar el menu
                break;
        }
    }
    return 0;
}
