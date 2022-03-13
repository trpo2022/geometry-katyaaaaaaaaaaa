CFLAGS = -Wall -Werror 

a.out:  main.o geom.o
	gcc $(CFLAGS) main.o geom.o -lm

main.o: main.c
	gcc -c $(CFLAGS) main.c 

geom.o: geom.c
	gcc -c $(CFLAGS) geom.c 

clean:
	rm -rf *.o  *.a a.out
