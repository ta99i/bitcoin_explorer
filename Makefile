# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Iinclude -std=c++17 -Wall

# Linker flags
LDFLAGS = -lssl -lcrypto

# Target executable
TARGET = main

# Source files
SRCS = main.cpp src/transaction.cpp src/block.cpp src/utilities.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: clean
