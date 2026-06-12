 bits 64
 default rel
 global contarChar
 global validarMov
 ;global calcularPuntaje pendiente
 global detectarObj 
 global contCeldasLibres

 section .text

;Funcion que devuelve cuantos caracteres de ese tipo contiene la matriz
;int contarCaracteres(char *mapa, int totCeldas, char aBuscar)
;                        rcx          rdx           r8b
 contarChar:
    xor rax,rax ;Este sera el contador
    xor r9,r9 ;Este sera i
    
    .ciclo: ;for(int i=0; i<totCeldas; i++)
    cmp r9,rdx
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
contCeldasLibres:
