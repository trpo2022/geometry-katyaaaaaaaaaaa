CFLAGS = -Wall -Werror
BIN_DIR = ./bin
SRC_DIR = ./src
OBJ_DIR = ./obj

$(BIN_DIR)/mygeomprogram:  $(OBJ_DIR)/main.o $(OBJ_DIR)/libgeom.a
	gcc $(CFLAGS) $^ -o $@ -lm

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	gcc -c $(CFLAGS) $< -o $@ 

$(OBJ_DIR)/libgeom.a: $(OBJ_DIR)/geom.o
	ar rcs $(OBJ_DIR)/libgeom.a $(OBJ_DIR)/geom.o	

$(OBJ_DIR)/geom.o: $(SRC_DIR)/geom.c
	gcc -c $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJ_DIR)/*.o  $(OBJ_DIR)/*.a $(BIN_DIR)/mygeomprogram

