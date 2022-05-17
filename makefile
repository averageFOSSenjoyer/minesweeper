flags=-g -std=c99 -Wall

ms: main.o
	gcc $(flags) main.o -o minesweeper

main.o: main.c
	gcc $(flags) -c main.c

clean:
	del /f main.o minesweeper