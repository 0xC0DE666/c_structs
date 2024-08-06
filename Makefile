CC = gcc
C_FLAGS = -std=c99 -O2 -g -Wall -Wextra

SRC_D = ./src
OBJ_D = ./obj
BIN_D = ./bin

# all: clean array.o array.so array_test.o array_test matrix.o matrix.so matrix_test.o matrix_test lib.o lib.so lib_test main.o main;
all: clean array.o array.so array_test.o array_test matrix.o matrix.so matrix_test.o matrix_test lib_test main.o main;

#------------------------------
# MAIN
#------------------------------

main.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/main.c -o $(OBJ_D)/$@;

main: array.o matrix.o main.o;
	$(CC) $(C_FLAGS) $(OBJ_D)/array.o $(OBJ_D)/matrix.o $(OBJ_D)/main.o -o $(BIN_D)/$@;


#------------------------------
# LIB
#------------------------------

# lib.o: array.o matrix.o;
# 	$(CC) $(C_FLAGS) $(OBJ_D)/array.o $(OBJ_D)/matrix.o -o $(OBJ_D)/$@;
# 
# lib.so:
# 	$(CC) $(C_FLAGS) -fPIC -shared $(SRC_D)/array/array.c $(SRC_D)/matrix/matrix.c -lc -o $(OBJ_D)/$@;

lib_test: array.o array_test.o matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) $(OBJ_D)/array.o $(OBJ_D)/array_test.o $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o -lcriterion -o $(BIN_D)/$@;


#------------------------------
# LIB ARRAY
#------------------------------

array.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/array/array.c -o $(OBJ_D)/$@;

array.so:
	$(CC) $(C_FLAGS) -fPIC -shared $(SRC_D)/array/array.c -o $(OBJ_D)/$@;

array_test.o: array.o;
	$(CC) $(C_FLAGS) -c $(SRC_D)/array/array_test.c -o $(OBJ_D)/$@;

array_test: array.o array_test.o;
	$(CC) $(C_FLAGS) $(OBJ_D)/array.o $(OBJ_D)/array_test.o -lcriterion -o $(BIN_D)/$@;


#------------------------------
# LIB MATRIX
#------------------------------
matrix.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/matrix/matrix.c -o $(OBJ_D)/$@;

matrix.so:
	$(CC) $(C_FLAGS) -fPIC -shared $(SRC_D)/matrix/matrix.c -o $(OBJ_D)/$@;

matrix_test.o: matrix.o;
	$(CC) $(C_FLAGS) -c $(SRC_D)/matrix/matrix_test.c -o $(OBJ_D)/$@;

matrix_test: matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) $(OBJ_D)/matrix.o $(OBJ_D)/matrix_test.o -lcriterion -o $(BIN_D)/$@;


#------------------------------
# MISC
#------------------------------

clean:
	rm -f $(OBJ_D)/* $(BIN_D)/*;
