VERSION := 0.0.0
NAME := libc_structs
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -g -Wall -Wextra -pthread

BIN_DIR := ./build/bin
DIST_DIR := ./build/dist
DIST_OBJS := $(wildcard $(DIST_DIR)/*.o)

all: clean libc_structs.o libc_structs.a libc_structs.so app test;

DEPS_DIR := ./src/deps
DEPS_OBJS := $(wildcard $(DEPS_DIR)/*.o)

#------------------------------
# APP
#------------------------------

APP_DIR := ./src/app
APP_SRCS := $(wildcard $(APP_DIR)/*.c)
APP_OBJS := $(patsubst %.c, %.o, $(APP_SRCS))

$(APP_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

app: $(APP_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(DIST_OBJS);

#------------------------------
# LIB
#------------------------------

LIB_DIR := ./src/lib
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)
LIB_HDRS = $(wildcard $(LIB_DIR)/*.h)
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

libc_structs.o: $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_structs.a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_structs.so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_HDRS = $(wildcard $(TEST_DIR)/*.h)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

test: $(TEST_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(DIST_OBJS);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean app test libc_structs.so;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(DIST_DIR)/* $(BIN_DIR)/*;
