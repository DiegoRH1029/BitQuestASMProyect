echo compilar asm
nasm -f win64 rutinas.asm -o rutinas.obj

echo compilar compilar
gcc main.c juego.c rutinas.obj -o BitQuest.exe

echo Ejecutando..
start /MAX BitQuest.exe
