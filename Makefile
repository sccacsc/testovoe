CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -fPIC
LDFLAGS = -shared

# Директории
BUILD_DIR = build
LIB_DIR = library
LIB_INCLUDE = $(LIB_DIR)/include
LIB_SRC = $(LIB_DIR)/source

PROG1_DIR = program1
PROG1_INCLUDE = $(PROG1_DIR)/include
PROG1_SRC = $(PROG1_DIR)/source

PROG2_DIR = program2
PROG2_INCLUDE = $(PROG2_DIR)/include
PROG2_SRC = $(PROG2_DIR)/source

# Файлы
LIB_OBJ = $(BUILD_DIR)/libit.o
LIB_SHARED = $(BUILD_DIR)/libit.so

PROG1_OBJ = $(BUILD_DIR)/program1.o $(BUILD_DIR)/client.o
PROG2_OBJ = $(BUILD_DIR)/program2.o

# Цель по умолчанию
all: $(LIB_SHARED) program1 program2

# Создание директории build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Создание динамической библиотеки
$(LIB_SHARED): $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/libit.o: $(LIB_SRC)/libit.cpp $(LIB_INCLUDE)/libit.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_INCLUDE) -c -o $@ $<

# Компиляция program1
program1: $(PROG1_OBJ) $(LIB_SHARED)
	$(CC) -o prog1 $^ -L$(BUILD_DIR) -lit -Wl,-rpath=$(BUILD_DIR)

$(BUILD_DIR)/program1.o: $(PROG1_SRC)/program1.cpp $(PROG1_INCLUDE)/program1.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG1_INCLUDE) -I$(LIB_INCLUDE) -c -o $@ $<

$(BUILD_DIR)/client.o: $(PROG1_SRC)/client.cpp $(PROG1_INCLUDE)/client.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG1_INCLUDE) -I$(LIB_INCLUDE) -c -o $@ $<	

# Компиляция program2
program2: $(PROG2_OBJ) $(LIB_SHARED)
	$(CC) -o prog2 $^ -L$(BUILD_DIR) -lit -Wl,-rpath=$(BUILD_DIR)

$(BUILD_DIR)/program2.o: $(PROG2_SRC)/program2.cpp $(PROG2_INCLUDE)/program2.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(PROG2_INCLUDE) -I$(LIB_INCLUDE) -c -o $@ $<

# Очистка
clean:
	rm -rf $(BUILD_DIR)
	rm -rf prog1 prog2
