CXX = g++
CXXFLAGS = -I$(INC_DIR) -Wall -Wextra -mavx2 -O3 -g

TARGET = run
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
INC_DIR = include
DUMP_DIR = dump

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/hashTable.cpp $(SRC_DIR)/hashFunction.cpp $(SRC_DIR)/std.cpp $(SRC_DIR)/benchmark.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(OBJECTS) 
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR) $(DUMP_DIR)

run: $(BIN_DIR)/$(TARGET)
	@mkdir -p $(DUMP_DIR)
	@./$<