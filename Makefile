CXX = g++
CXXFLAGS = -I$(INC_DIR) -Wall -Wextra -mavx2 -O3 -g -z noexecstack 

ASM = nasm
ASMFLAGS = -f elf64

TARGET = run
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
INC_DIR = include
DUMP_DIR = dump

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/hashTable.cpp $(SRC_DIR)/hashFunction.cpp $(SRC_DIR)/benchmark.cpp
ASM_SOURCES = 
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
ASM_OBJECTS = $(ASM_SOURCES:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(OBJECTS) $(ASM_OBJECTS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp 
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(BUILD_DIR)
	@$(ASM) $(ASMFLAGS) $< -o $@

.PHONY: clean run

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR) $(DUMP_DIR)

run: $(BIN_DIR)/$(TARGET)
	@mkdir -p $(DUMP_DIR)
	@./$<