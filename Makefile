CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -fPIC
LDFLAGS = -shared

# Директории
BUILD_DIR = build
BIN_DIR = bin

LIB_DIR = library
LIB_SRC = $(LIB_DIR)/src

PROG1_DIR = program1
PROG1_SRC = $(PROG1_DIR)/src

PROG2_DIR = program2
PROG2_SRC = $(PROG2_DIR)/src

# Файлы
LIB_OBJ = $(BUILD_DIR)/libit.o
LIB_SHARED = $(BUILD_DIR)/libit.so

PROG1_OBJ = $(BUILD_DIR)/program1.o $(BUILD_DIR)/client.o
PROG2_OBJ = $(BUILD_DIR)/program2.o

# Цель по умолчанию
all: $(LIB_SHARED) $(BIN_DIR)/program1 $(BIN_DIR)/program2

# Создание директории bin
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Создание директории build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Создание динамической библиотеки
$(LIB_SHARED): $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/libit.o: $(LIB_SRC)/libit.cpp $(LIB_SRC)/libit.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_SRC) -c -o $@ $<

# Компиляция program1
$(BIN_DIR)/program1: $(PROG1_OBJ) $(LIB_SHARED) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/prog1 $^ -L$(BUILD_DIR) -lit -Wl,-rpath=$(BUILD_DIR)

$(BUILD_DIR)/program1.o: $(PROG1_SRC)/program1.cpp $(PROG1_SRC)/program1.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG1_SRC) -I$(LIB_SRC) -c -o $@ $<

$(BUILD_DIR)/client.o: $(PROG1_SRC)/client.cpp $(PROG1_SRC)/client.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG1_SRC) -I$(LIB_SRC) -c -o $@ $<	

# Компиляция program2
$(BIN_DIR)/program2: $(PROG2_OBJ) $(LIB_SHARED) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/prog2 $^ -L$(BUILD_DIR) -lit -Wl,-rpath=$(BUILD_DIR)

$(BUILD_DIR)/program2.o: $(PROG2_SRC)/program2.cpp $(PROG2_SRC)/program2.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG2_SRC) -I$(LIB_SRC) -c -o $@ $<

# Очистка
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
