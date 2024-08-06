CC = gcc
C_FLAGS = -std=c99 -O2 -g -Wall -Wextra

SRC_D = ./src
OUT_D = ./out
BIN_D = ./bin

all: clean array.o array_test.o array_test matrix.o matrix_test.o matrix_test main.o main all_test;

#------------------------------
# MAIN
#------------------------------

main.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/main.c -o $(OUT_D)/main.o;

main: array.o matrix.o main.o;
	$(CC) $(C_FLAGS) $(OUT_D)/array.o $(OUT_D)/matrix.o $(OUT_D)/main.o -o $(BIN_D)/main;

all_test: array.o array_test.o matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) $(OUT_D)/array.o $(OUT_D)/array_test.o $(OUT_D)/matrix.o $(OUT_D)/matrix_test.o -lcriterion -o $(BIN_D)/all_test;



#------------------------------
# LIB ARRAY
#------------------------------

array.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/array/array.c -o $(OUT_D)/array.o;

array_test.o: array.o;
	$(CC) $(C_FLAGS) -c $(SRC_D)/array/array_test.c -o $(OUT_D)/array_test.o;

array_test: array.o array_test.o;
	$(CC) $(C_FLAGS) $(OUT_D)/array.o $(OUT_D)/array_test.o -lcriterion -o $(BIN_D)/array_test;


#------------------------------
# LIB MATRIX
#------------------------------
matrix.o:
	$(CC) $(C_FLAGS) -c $(SRC_D)/matrix/matrix.c -o $(OUT_D)/matrix.o;

matrix_test.o: matrix.o;
	$(CC) $(C_FLAGS) -c $(SRC_D)/matrix/matrix_test.c -o $(OUT_D)/matrix_test.o;

matrix_test: matrix.o matrix_test.o;
	$(CC) $(C_FLAGS) $(OUT_D)/matrix.o $(OUT_D)/matrix_test.o -lcriterion -o $(BIN_D)/matrix_test;


#------------------------------
# MISC
#------------------------------

clean:
	rm -f $(BIN_D)/* $(BIN_D)/*;
