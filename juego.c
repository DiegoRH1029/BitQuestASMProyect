#include <stdio.h>
#include "juego.h"

//Matriz de nuestro mapa
char mapa[FILAS][COLUMNAS];
//:---------Funciones flujo del juego 
//Jugando es un bucle de juego por nivel, regresa un paquete con stats del nivel y el jugador
ResultadoNivel jugando(Jugador p1, int nivelActual){
    ResultadoNivel res;
    struct Camara cam;
    Mapa mapaInfo;
    mapaInfo.numNivel=nivelActual;
    int juego=1;
    int completado=0;
    int direccion=1;
    
    //obtenemos la posisicion inicial del jugador desde el mapa 
    int indJugador = posCaracter(&mapa[0][0],FILAS*COLUMNAS,'P');
    if(indJugador!=-1){
        p1.fila = indJugador/COLUMNAS;
        p1.col = indJugador%COLUMNAS;
    }   
    else{ 
        printf ("\nEl jugador no existe");
        res.estado = -1;
        res.jugador = p1;
        return res;
    } 
    mapaInfo.totalMonedas = contarChar(&mapa[0][0], FILAS*COLUMNAS, 'M');
    mapaInfo.totalLlaves = contarChar(&mapa[0][0], FILAS*COLUMNAS, 'K');
    
    // Guardamos la maxima capacidad de monedas del nivel actual
    res.maxMonedasNivel = mapaInfo.totalMonedas; 
    
    //Centramos la camara respecto al jugador
    cam.fila = p1.fila-10;
    cam.col = p1.col -10;
        
    while(juego){
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
            else if(tecla=='q'||tecla=='Q'){ juego = 0;}
            
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
                        juego = 0;
                        puedeAvanzar=1;
                        completado=1;
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
                    p1.movs++;
                }

            }
        }
        Sleep(25);
    }
    
    system("cls");
    
   //calculamos puntaje global y actualizamos movimientos
    p1.puntajeTot += calcularPuntaje(p1.monedas,p1.movs);
    p1.totalMovs += p1.movs;
    p1.totalMonedas+=p1.monedas;
    
    if(imprimirInfo(p1,mapaInfo,completado)){
        //si continua, limpiamos los pasos locales para que no afecten el sig nivel
        p1.movs = 0; 
        p1.monedas=0;
        p1.llaves=0;
        res.estado = 1;
    }
    else{
        res.estado = -1; //Señal que salio del juego
    }
    
    res.jugador = p1;
    return res;
}

//Funcion que imprimira la informacion del nivel completado
int imprimirInfo(Jugador p1,Mapa mapaInfo,int completado){
    char mensaje[50];
    if(completado){
        imprimirTitulo("F E L I C I D A D E S",14,9);
        sprintf(mensaje,"Nivel -%d- completado",mapaInfo.numNivel);
        imprimirTitulo(mensaje,10,9);
        printf("\n");
        imprimirTitulo("E S T A D I S T I C A S",14,9);
        sprintf(mensaje,"Llaves sobrantes: %d",p1.llaves);
        imprimirTitulo(mensaje,11,11);
        sprintf(mensaje,"Monedas: %d / %d",p1.monedas,mapaInfo.totalMonedas);
        imprimirTitulo(mensaje,14,14);
        sprintf(mensaje,"Total movimientos: %d",p1.movs);
        imprimirTitulo(mensaje,10,10);
        sprintf(mensaje,"Puntaje: %d",p1.puntajeTot);
        imprimirTitulo(mensaje,12,12);
        printf("Desea continuar? (y/n): ");
        char resp;
        do{
            resp=toupper(_getch());
        }while(resp!='Y'&&resp!='N');
        if(resp=='Y'){system("cls"); return 1;}
        else{system("cls"); return 0;}   
    }
    else{
        imprimirTitulo("SUERTE PARA LA PROXIMA",12,12);
        sprintf(mensaje,"Nivel -%d- no completado",mapaInfo.numNivel);
        imprimirTitulo(mensaje,10,9);
        printf("\n");
        imprimirTitulo("E S T A D I S T I C A S",14,9);
        sprintf(mensaje,"Llaves sobrantes: %d",p1.llaves);
        imprimirTitulo(mensaje,11,11);
        sprintf(mensaje,"Monedas: %d / %d",p1.monedas,mapaInfo.totalMonedas);
        imprimirTitulo(mensaje,14,14);
        sprintf(mensaje,"Total movimientos: %d",p1.movs);
        imprimirTitulo(mensaje,10,10);
        system("pause");
        return 0;
    }
}
void actualizarRanking(char *nombreJugador,int puntaje){
    // Abrimos el archivo en modo "a" (añadir)
    FILE *archivo = fopen("rankings.txt", "a");
    if (archivo==NULL) {
        cambiarColor(12); // Rojo
        printf("\nError: No se pudo guardar el ranking.\n");
        cambiarColor(7);
        return;
    }
    // Guardamos el puntaje primero para que sea más fácil de ordenar después
    fprintf(archivo, "%d - %s\n", puntaje, nombreJugador);
    fclose(archivo);
}
void imprimirVictoria(Jugador p1, int maxMonedasTotales, int nivelesTotales) {
    system("cls");
    printf("\n\n");
    
    // Título principal
    imprimirTitulo("V I C T O R I A !", 14, 9); // Texto amarillo, borde azul
    printf("\n");
    char mensaje[50]; 
    sprintf(mensaje, "Felicidades, %s", p1.nombre);
    imprimirTitulo(mensaje, 15, 9); // Texto blanco, borde azul
    
    imprimirTitulo("Has escapado del laberinto", 11, 9); // Texto celeste, borde azul
    printf("\n");
    
    // Estadisticas
    imprimirTitulo("R E S U M E N   F I N A L", 14, 9);
    
    sprintf(mensaje, "Monedas recolectadas: %d / %d", p1.totalMonedas, maxMonedasTotales);
    imprimirTitulo(mensaje, 14, 14); // Texto y marco amarillo
    sprintf(mensaje, "Pasos totales: %d", p1.totalMovs);
    imprimirTitulo(mensaje, 10, 10); // Texto y marco verde
    sprintf(mensaje, "Niveles completados: %d", nivelesTotales);
    imprimirTitulo(mensaje, 11, 11); // Texto y marco celeste
    sprintf(mensaje, "PUNTAJE FINAL: %d", p1.puntajeTot);
    imprimirTitulo(mensaje, 12, 12); // Texto y marco rojo brillante para resaltar
    printf("\n\n");
    cambiarColor(8); // Gris oscuro
    printf("                       Presiona cualquier tecla para continuar...\n");
    cambiarColor(7);
    
    // Limpiamos el buffer del teclado por si el jugador presionó algo extra
    while(_kbhit()) _getch(); 
    _getch();
}
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
//Funcion para impirmir nuestro menu principal
void imprimirMenu(){
            // Dibujamos un Menu tipo Arcade
    imprimirTitulo("B I T Q U E S T",14,9);
    printf("\n");
    imprimirTitulo("[P] J u g a r",10,10);
    imprimirTitulo("[R] R a n k i n g",14,14);  
    imprimirTitulo("[S] S a l i r",12,12);       
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
    printf("         BITQUEST - NIVEL %d           ", nivel); 
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
//Funcion que imprime un titulo en medio de l_____l
void imprimirTitulo(const char* titulo, int colorTitulo,int colorBorde) {
    int anchoCaja = 36;
    int largoTitulo = strlen(titulo);
    
    // Calculamos cuantos espacios en blanco van a la izquierda y a la derecha
    int espaciosIzq=(anchoCaja - largoTitulo) / 2;
    int espaciosDer=anchoCaja - largoTitulo - espaciosIzq;

    cambiarColor(colorBorde); // Color del borde 
    
    //Techo
    printf("%c", 201); 
    for(int i = 0; i < anchoCaja; i++) printf("%c", 205); 
    printf("%c\n", 187);

    //Paredes y texto
    printf("%c", 186); // Pared izquierda
    cambiarColor(colorTitulo); // Aplicamos el color de la letra
    
    for(int i=0; i<espaciosIzq; i++) printf(" ");
    printf("%s", titulo);
    for(int i=0; i<espaciosDer; i++) printf(" ");
    
    cambiarColor(colorBorde); // Regresamos al color del marco
    printf("%c\n", 186); // Pared derecha

    // Piso
    printf("%c", 200); 
    for(int i=0;i<anchoCaja; i++)printf("%c", 205); 
    printf("%c\n", 188);
}
//Funcion que se encargara de cargar un nivel desde el archivo niveles.txt
//recibe el nombre del archivo y el nivel a bscar (--nivel 1, --nivel 2 etc)
//Regresa -1 si ocurrio un error, 0 si no se encontro el nivel, 1 si el nivel fue cargado bien
int cargarNivel(const char* nombreArchivo, const char* nivelABuscar){
    FILE *archivo = fopen(nombreArchivo,"r");
    if(archivo==NULL){
        printf("Archivo no encontrado o no valido");
        return -1;
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
            return -1;
        }
        if(strstr(buffer,"--nivel ")!=NULL){
            printf("Error :Mapa incompleto");
            fclose(archivo);
            return -1;
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