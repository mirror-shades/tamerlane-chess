# Define directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := src/include
LIB_DIR := src/lib

# Define compiler and linker
CXX := g++
CXXFLAGS := -I$(INCLUDE_DIR) -std=c++20
LDFLAGS := -L"$(LIB_DIR)" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32 -mconsole

# Find all .cpp files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate .obj file paths in the build directory
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.obj,$(SRCS))

# Output executable
EXEC := $(BUILD_DIR)/main.exe

# Default target
all: $(EXEC)

# Compile rule
$(BUILD_DIR)/%.obj: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link rule
$(EXEC): $(OBJS)
	@echo "Linking $(EXEC)"
	@echo "Objects: $(OBJS)"
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Clean command
clean:
	@if exist $(BUILD_DIR) (
		@echo Cleaning build directory...
		@for %%F in ($(BUILD_DIR)\*) do @if not "%%~xF"==".dll" del "%%F"
		@for /d %%D in ($(BUILD_DIR)\*) do @rmdir /s /q "%%D"
	)
	
.PHONY: all clean

# Debug info
$(info SRCS = $(SRCS))
$(info OBJS = $(OBJS))