CXX = g++
CXXFLAGS = -Wall -o

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
BINS_DIR = ./bins

TARGET = $(MAKECMDGOALS)
TARGET_SRC = $(SRCS_DIR)/$(TARGET).*

$(TARGET): $(OBJS_DIR)/$(TARGET).o
	$(info => Creating Binary File)
	$(CXX) $(CXXFLAGS) $(BINS_DIR)/$(TARGET) $(OBJS_DIR)/$(TARGET).o

$(OBJS_DIR)/$(TARGET).o: $(TARGET_SRC)
	$(info => Creating Object File)
	$(CXX) -c $(CXXFLAGS) $(OBJS_DIR)/$(TARGET).o $(TARGET_SRC)
