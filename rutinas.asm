 bits 64
 default rel
 global contarChar

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