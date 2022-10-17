
main: PADRE_TRENI.o REGISTRO.o TRENO.o main.o
	gcc -o $ main PADRE_TRENI.o REGISTRO.o TRENO.o main.o

PADRE_TRENI.o REGISTRO.o TRENO.o: PADRE_TRENI.h REGISTRO.h TRENO.h
	gcc -c $ *.c

clean:
	rm *.o
