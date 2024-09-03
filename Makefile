# Define directories
SRC_DIR := ./src
BUILD_DIR := build
INCLUDE_DIR := src/include
LIB_DIR := src/lib

# Define compiler and linker
CXX := g++
CXXFLAGS := -I$(INCLUDE_DIR) -c -std=c++20
LDFLAGS := -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-audio

# Check build type
ifeq ($(BUILD_TYPE),release)
    CXXFLAGS += -O2 -DNDEBUG
    LDFLAGS += -mwindows
endif

# Find all .cpp files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate .o file paths in the build directory
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Output executable
EXEC := $(BUILD_DIR)/main

# Default target
all: $(EXEC)

# Compile rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Link rule
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean command, excluding .dll files
clean:
	if exist $(BUILD_DIR) (del /Q /F $(BUILD_DIR)\*.o $(BUILD_DIR)\main.exe)

.PHONY: all clean