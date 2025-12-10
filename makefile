# Makefile for CSI 4337 Multithreaded Web Server
# Cross-platform support: Windows and Linux/Mac

# Detect OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    TARGET := webserver.exe
    LDFLAGS := -lws2_32
    RM := del /Q
else
    DETECTED_OS := $(shell uname -s)
    TARGET := webserver
    LDFLAGS :=
    RM := rm -f
endif

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -pthread

# Source directory
SRCDIR = src

# Source files
SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/server.cpp $(SRCDIR)/threadpool.cpp $(SRCDIR)/http.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compilation step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove compiled files
clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: all clean
