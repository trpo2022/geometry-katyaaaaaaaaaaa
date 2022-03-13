CFLAGS = -Wall -Werror 

a.out:  main.o libgeom.a
	gcc $(CFLAGS) main.o libgeom.a -lm

main.o: main.c
	gcc -c $(CFLAGS) main.c 

libgeom.a: geom.o
	ar rcs libgeom.a geom.o	

geom.o: geom.c
	gcc -c $(CFLAGS) geom.c 

clean:
	rm -rf *.o  *.a a.out

