APP_NAME = mygeomprogram
LIB_NAME = libgeom

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj/src

CFLAGS =   -Wall -Werror -I$(SRC_DIR)/$(LIB_NAME) -Ithirdpart
APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(LIB_NAME)/$(LIB_NAME).a


$(APP_PATH):  $(OBJ_DIR)/$(APP_NAME)/main.o $(OBJ_DIR)/$(LIB_NAME)/libgeom.a
	gcc $(CFLAGS) $^ -o $@ -lm

$(OBJ_DIR)/$(APP_NAME)/main.o: $(SRC_DIR)/$(APP_NAME)/main.c
	gcc -c $(CFLAGS) $< -o $@ 

$(LIB_PATH): $(OBJ_DIR)/$(LIB_NAME)/geom.o
	ar rcs  $@ $< 

$(OBJ_DIR)/$(LIB_NAME)/geom.o: $(SRC_DIR)/$(LIB_NAME)/geom.c
	gcc -c $(CFLAGS) $< -o $@ 
	
test: obj/test/mytests.o obj/test/main.o $(OBJ_DIR)/$(LIB_NAME)/libgeom.a
	gcc $(CFLAGS) $^ -o $(APP_PATH)-test -lm

obj/test/mytests.o: test/mytests.c
	gcc -c $(CFLAGS) $< -o $@
	
obj/test/main.o: test/main.c
	gcc -c $(CFLAGS) $< -o $@

.PHONY: clean test

clean:
	rm -rf $(OBJ_DIR)/*/*.o  $(OBJ_DIR)/$(LIB_NAME)/*.a $(BIN_DIR)/$(APP_NAME)

