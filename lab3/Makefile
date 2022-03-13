CFLAGS = -Wall -Werror 

mygeomprogram:  main.o libgeom.a
	gcc $(CFLAGS) $^ -o $@ -lm

main.o: main.c
	gcc -c $(CFLAGS) $< -o $@ 

libgeom.a: geom.o
	ar rcs libgeom.a geom.o	

geom.o: geom.c
	gcc -c $(CFLAGS) $< -o $@ 

clean:
	rm -rf *.o  *.a a.out

