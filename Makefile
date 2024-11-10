VERSION := 0.0.0
NAME := libc_structs
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -g -Wall -Wextra -pthread

define GET_VERSIONED_NAME
$(NAME).$(1).$(VERSION)
endef

SRC_DIR := ./src
DEPS_ROOT_DIR := $(SRC_DIR)/deps
DEPS_OBJS := $(shell find $(DEPS_ROOT_DIR) -type f -name "*.o")

BUILD_DIR := ./build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
RELEASE_DIR := $(BUILD_DIR)/release
RELEASE_O := $(RELEASE_DIR)/$(NAME).o
VERSIONED_RELEASE_ASSETS := $(call GET_VERSIONED_NAME,o) $(call GET_VERSIONED_NAME,a) $(call GET_VERSIONED_NAME,so)
UNVERSIONED_RELEASE_ASSETS := $(NAME).o $(NAME).a $(NAME).so

all: clean libc_structs.o libc_structs.a libc_structs.so app test;

#------------------------------
# APP
#------------------------------

APP_SRC_DIR := $(SRC_DIR)/app
APP_OBJ_DIR := $(OBJ_DIR)/app
APP_HDRS := $(wildcard $(APP_SRC_DIR)/*.h)
APP_SRCS := $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJS := $(patsubst $(APP_SRC_DIR)/%.c, $(APP_OBJ_DIR)/%.o, $(APP_SRCS))

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.c | $(APP_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

app: $(APP_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(RELEASE_O);

#------------------------------
# LIB
#------------------------------

LIB_DIR := ./src/lib
LIB_HDRS = $(wildcard $(LIB_DIR)/*.h)
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_OBJS):
	$(CC) $(C_FLAGS) -c -o $@ $(patsubst %.o, %.c, $@);

libc_structs.o: $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_structs.a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_structs.so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_HDRS = $(wildcard $(TEST_DIR)/*.h)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_OBJS):
	$(CC) $(C_FLAGS) -c -o $@ $(patsubst %.o, %.c, $@);

test: $(TEST_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(RELEASE_O);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean libc_structs.o libc_structs.a libc_structs.so app test;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(RELEASE_DIR)/* $(BIN_DIR)/*;
