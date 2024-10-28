# Define the path to the SystemC library
SYSTEMC = $(CONDA_PREFIX)
SYSTEMC_INCLUDE = $(SYSTEMC)/include
SYSTEMC_LIB = $(SYSTEMC)/lib

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -I$(SYSTEMC_INCLUDE) -L$(SYSTEMC_LIB) -lsystemc -std=c++17

# Output executable file
TARGET = sim.out

# Source files
SRCS = main.cpp Canny_Edge.cpp test.cpp UART_XMTR_WRAP.cpp UART_XMTR.cpp SRAM.cpp SRAM_WRAP.cpp Canny_Edge_WRAP.cpp Arbiter.cpp test.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build rules
all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

