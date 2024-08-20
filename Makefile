VERSION := 0.1.0
LIB_NAME := libc_structs

CC := gcc
C_FLAGS := -g -Wall -Wextra

LIB_DIR := ./src/lib
TEST_DIR := ./src/test
OBJ_DIR := ./obj
BIN_DIR := ./bin

all: clean app test libc_structs.so libc_structs.a;

clean:
	rm -f ./src/main.o $(LIB_OBJS) $(TEST_OBJS) $(OBJ_DIR)/* $(BIN_DIR)/*;

#------------------------------
# LIB
#------------------------------

LIB_DIRS = ./src/lib ./src/lib/array ./src/lib/matrix
LIB_SRCS = $(foreach dir, $(LIB_DIRS), $(wildcard $(dir)/*.c))
LIB_HDRS = $(foreach dir, $(LIB_DIRS), $(wildcard $(dir)/*.h))
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

libc_structs.o: $(LIB_OBJS);
	ld -relocatable -o $(OBJ_DIR)/$@ $(LIB_OBJS);

libc_structs.so: $(LIB_OBJS) $(LIB_HDRS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(OBJ_DIR)/$@ $(LIB_OBJS);

libc_structs.a: $(LIB_OBJS);
	ar rcs $(OBJ_DIR)/$@ $(LIB_OBJS);

main.o:
	$(CC) $(C_FLAGS) -c -o ./src/main.o ./src/main.c;

app: main.o libc_structs.o;
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ ./src/main.o $(OBJ_DIR)/libc_structs.o;


#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_HDRS = $(foreach dir, $(TEST_DIR), $(wildcard $(dir)/*.h))
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

test: $(LIB_OBJS) $(TEST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(LIB_OBJS) $(TEST_OBJS);


#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean app test libc_structs.so;
