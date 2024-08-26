# Define directories
SRC_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./src/include
LIB_DIR := ./src/lib

# Find all .cpp files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate .o file paths in the build directory
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Output executable
EXEC := $(BUILD_DIR)/main

# Default build type to debug
BUILD_TYPE ?= debug

# Define compiler and linker
CXX := g++

# Define flags based on the build type
ifeq ($(BUILD_TYPE), release)
    CXXFLAGS := -I$(INCLUDE_DIR) -c -O2 -DNDEBUG
    LDFLAGS := -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-audio -mwindows
else
    CXXFLAGS := -I$(INCLUDE_DIR) -c -g
    LDFLAGS := -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lsfml-audio
endif

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

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean