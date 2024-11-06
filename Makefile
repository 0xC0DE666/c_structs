VERSION := 0.2.0
NAME := libc_structs
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -g -Wall -Wextra -pthread

DEPS_DIR := ./src/deps
DIST_DIR := ./dist
BIN_DIR := ./bin

all: clean app test libc_structs.so libc_structs.a;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(DIST_DIR)/* $(BIN_DIR)/*;

#------------------------------
# APP
#------------------------------

APP_DIR := ./src/app
APP_SRCS := $(wildcard $(APP_DIR)/*.c)
APP_OBJS := $(patsubst %.c, %.o, $(APP_SRCS))

$(APP_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

app: $(APP_OBJS) libc_structs.o;
	echo '$(APP_OBJS)'
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(DIST_DIR)/libc_structs.o;

#------------------------------
# LIB
#------------------------------

LIB_DIR := ./src/lib
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)
LIB_HDRS = $(wildcard $(LIB_DIR)/*.h)
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

libc_structs.o: $(LIB_OBJS);
	ld -relocatable -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_DIR)/libc_errors.o;

libc_structs.so: $(LIB_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_DIR)/libc_errors.o;

libc_structs.a: $(LIB_OBJS);
	ar rcs $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_DIR)/libc_errors.o;

#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_HDRS = $(wildcard $(TEST_DIR)/*.h)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

test: $(TEST_OBJS) libc_structs.o;
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(DIST_DIR)/libc_structs.o;

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean app test libc_structs.so;
