CC = gcc
C_FLAGS = -g -Wall -Wextra

SRC_D = ./src
OBJ_D = ./obj
BIN_D = ./bin

# all: clean array.o array.so array_test.o array_test matrix.o matrix.so matrix_test.o matrix_test lib.o lib.so lib_test main.o main;
all: 	clean \
			array.o array.so array_test.o array_test \
			matrix.o matrix.so matrix_test.o matrix_test \
			libc_structs.so lib_test \
			main.o main;

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS = -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: 	clean \
			array.o array.so array_test.o array_test \
			matrix.o matrix.so matrix_test.o matrix_test \
			libc_structs.so lib_test \
			main.o main;


#------------------------------
# MAIN
#------------------------------

main.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/main.c;

main: array.o matrix.o main.o;
	$(CC) $(C_FLAGS) -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o $(OBJ_D)/main.o;


#------------------------------
# LIB
#------------------------------

#lib.o: array.o matrix.o;
#	$(CC) $(C_FLAGS) -o $(OBJ_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o;

libc_structs.so: array.o matrix.o
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(OBJ_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/matrix.o;

lib_test: array.o array_test.o matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/array_test.o $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o;


#------------------------------
# LIB ARRAY
#------------------------------

array.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/array/array.c;

array.so:
	$(CC) $(C_FLAGS) -fPIC -shared -o $(OBJ_D)/$@ $(SRC_D)/array/array.c;

array_test.o: array.o;
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/array/array_test.c;

array_test: array.o array_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/array.o $(OBJ_D)/array_test.o;


#------------------------------
# LIB MATRIX
#------------------------------
matrix.o:
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/matrix/matrix.c;

matrix.so:
	$(CC) $(C_FLAGS) -fPIC -shared -o $(OBJ_D)/$@ $(SRC_D)/matrix/matrix.c;

matrix_test.o: matrix.o;
	$(CC) $(C_FLAGS) -c -o $(OBJ_D)/$@ $(SRC_D)/matrix/matrix_test.c;

matrix_test: matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_D)/$@ $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o;


#------------------------------
# MISC
#------------------------------

clean:
	rm -f $(OBJ_D)/* $(BIN_D)/*;
