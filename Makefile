CC = gcc
C_FLAGS = -g -Wall -Wextra

SRC_D = ./src
OBJ_D = ./obj
BIN_D = ./bin

all: 	clean \
			array.o array_test.o array_test array.so \
			matrix.o matrix_test.o matrix_test matrix.so \
			libc_structs.o libc_structs_test libc_structs.so \
			main.o main;

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS = -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: 	clean \
			array.o array_test.o array_test array.so \
			matrix.o matrix_test.o matrix_test matrix.so \
			libc_structs.o libc_structs_test libc_structs.so \
			main.o main;


#------------------------------
# LIBC_STRUCTS
#------------------------------

libc_structs.o: array.o matrix.o;
	ld -relocatable -o $(OBJ_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o;

libc_structs.so: array.o matrix.o
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(OBJ_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o;

libc_structs_test: array.o array_test.o matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/array_test.o $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o;


#------------------------------
# MAIN
#------------------------------

main.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/main.c;

main: array.o matrix.o main.o;
	$(CC) $(C_FLAGS) -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o $(OBJ_D)/main.o;


#------------------------------
# LIB ARRAY
#------------------------------

ARRAY_D = $(SRC_D)/array

array.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(ARRAY_D)/array.c;

array.so: array.o
	$(CC) $(C_FLAGS) -fPIC -shared -o $(OBJ_D)/$@ $(OBJ_D)/array.o;

array_test.o: array.o;
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(ARRAY_D)/array_test.c;

array_test: array.o array_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/array_test.o;


#------------------------------
# LIB MATRIX
#------------------------------

MATRIX_D = $(SRC_D)/matrix

matrix.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(MATRIX_D)/matrix.c;

matrix.so: matrix.o
	$(CC) $(C_FLAGS) -fPIC -shared -o $(OBJ_D)/$@ $(OBJ_D)/matrix.o;

matrix_test.o: matrix.o;
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(MATRIX_D)/matrix_test.c;

matrix_test: matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o;


#------------------------------
# MISC
#------------------------------

clean:
	rm -f $(OBJ_D)/* $(BIN_D)/*;
