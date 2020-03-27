# A simple Makefile for compiling small SDL projects

theGame: main.o
	gcc -o theGame main.o -lSDL2 -lSDL2_image
main.o: main.c
	gcc -c main.c
all: $(EXEC)
