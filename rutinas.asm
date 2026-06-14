bits 64
default rel
global contarChar
global validarMov
global calcularPuntaje
global detectarObj 
global contCeldasLibres
;funcion que devolvera la posicion de un caracter (primera coincidencia)
global posCaracter
;Funcion que lee una celday devuelve lo que tiene
global leerChar
;Funcion que escribe en una celda
global escribirChar

 section .text

;Funcion que devuelve cuantos caracteres de ese tipo contiene la matriz
;int contarCaracteres(char *mapa, int totCeldas, char aBuscar)
;                        rcx          edx           r8b
contarChar:
    xor rax,rax ;Este sera el contador
    xor r9d,r9d ;Este sera i
    
    .ciclo: ;for(int i=0; i<totCeldas; i++)
    cmp r9d,edx
    jge .fin 

    mov r10b, byte [rcx+r9] ;r10b = mapa[i]
    cmp r10b,r8b ; es igual?
    jne .siguiente ;no, salta al siguientee
    inc rax ;Es igual, incrementa rax

    .siguiente:
    inc r9
    jmp .ciclo        
    
    .fin:
    ret 

;Funcion 2: validara que el movimiento sea posible (que no sea #)
;int validarMov(char *mapa, int numColumnas, int fila, int columna)
;                       rcx        rdx             r8         r9
validarMov:
    ;Obtendremos el indice con la sig formula
    ; indice = (fila*numColumnas) + columna 
    mov rax,r8 ; mandamos fila a rax para multiplicar
    imul rax,rdx ; rax = (fila*numColumnas)
    add rax,r9 ; (fila*numColumnas)+columna
    ;Aqui ya tenemos el indice de la casilla
    mov r10b, byte [rcx +rax];r10b=mapa[indice]
    cmp r10b,'#' ;Comparamos si es una pared
    je .esPared 
    ;Si no es pared regresamos 1
    mov rax,1
    ret
    .esPared
    mov rax,0
    ret

;calcularPuntaje pendiente
;Funcion que verificara que en la posicion especifica exista ese objeto
;int detectarObj(char *mapa, int columnas. int fila, int columna, char aBuscar)
                ;   rcx             rdx         r8         r9           [rsp+40]
detectarObj:
    ;Obtendremos el indice con la sig formula
    ; indice = (fila*numColumnas) + columna 
    mov rax,r8 ; mandamos fila a rax para multiplicar
    imul rax,rdx ; rax = (fila*numColumnas)
    add rax,r9 ; (fila*numColumnas)+columna
    ;Aqui ya tenemos el indice de la casilla
    mov r10b, byte [rcx +rax];r10b=mapa[indice]
    mov r11b, byte [rsp+40] ;Obtenemos el objeto a buscar desde la pila
    cmp r10b,r11b ;Comparamos si es el objeto a buscar
    je .siEs
    ;Si no es el objeto regresamos 0
    mov rax,0
    ret
    .siEs ;Si es el objejo regresamos 1
    mov rax,1
    ret
;Funcion que simplemente cuenta la cantidad de celdas libres
;int contarLibres(char *mapa, int totalCeldas)
;                   rcx            rdx
contCeldasLibres:
    xor rax,rax ;nuestro contador
    xor r9,r9 ; i

    .ciclo: ;for(i=0 i<totalCeldas i++)
    cmp r9,rdx
    jge .fin

    mov r10b, byte [rcx+r9] ;r10b = mapa[i]
    mov r11b, '.'
    cmp r10b,r11b;Comparamos si es igual a .
    jne .siguiente ;Si no es igual pasamos al siguiente
    inc rax ;contador++ si es igual
    .siguiente:
    inc r9 ;i++
    jmp .ciclo

    .fin:  
    ret
;int calcularPuntaje(int numMonedas, int numPasos)
    ;                     ecx              edx
;Nuestra formula cera (puntaje = (monedas*100)-pasos)
calcularPuntaje:
    mov eax, ecx ;pasamos las monedas a eax
    imul eax,100 ;las multiplicamos por 100
    sub eax,edx ;les restamos los pasos
    cmp eax,0 ;verificamos que sea mayor a 0
    jge .fin
    xor eax,eax ;sino devolvera 0
    .fin:
    ret 

;int posCaracter(char *mapa, int totCeldas, char c)
                ;  rcx          rdx         r8
posCaracter:
    xor rax,rax ;i

    .ciclo:;desde i=0 hasta total celdas
        cmp rax,rdx
        jge .noEncontrado
        mov r10b, byte [rcx+rax];guardamos el caracter actual en r10b
        cmp r10b,r8b ;lo comparamos con el caracter a buscar
        je .encontrado ;si es igual se encontro
        inc rax ;si no es igual repite el ciclo
        jmp .ciclo 

        .encontrado:
        ret 
        .noEncontrado:
        mov rax,-1
        ret 
;Funcion que lee una celda y devuelve lo que tiene
;char leerChar(char *mapa, int columnas, int fila,int col)
                    ; rcx          rdx        r8       r9
leerChar:
    mov rax,r8      ; rax = fila
    imul rax,rdx    ; rax = fila * columnas
    add rax,r9      ; rax = (fila*columnas) + columna
    xor r10,r10     ; Limpiamos r10
    mov r10b, byte [rcx+rax] ; Leemos el caracter en esa posicion de memoria  
    mov rax,r10    
    ret
;Funcion que escribe en una celda
;char escribirChar(char *mapa, int columnas, int fila, int col,char c)
                    ; rcx          rdx          r8         r9       [rsp+40]
escribirChar:
    mov rax, r8      ; rax = fila
    imul rax,rdx    ; rax = fila * columnas
    add rax,r9      ; rax = (fila*columnas) + columna
    mov r10b, byte [rsp+40]
    mov byte [rcx+rax],r10b   ; Sobreescribimos el caracter en el mapa
    ret
