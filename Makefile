CXX = gcc
CXXFLAGS = -Wall -o

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
# MakeFile is stored inside the "bins" directory.
BINS_DIR = ./bins

TARGET = $(MAKECMDGOALS)

$(TARGET): $(OBJS_DIR)/$(TARGET).o
	$(info => Creating Binary File)
	$(CXX) $(CXXFLAGS) $(TARGET) $(OBJS_DIR)/$(TARGET).o

$(OBJS_DIR)/$(TARGET).o: $(SRCS_DIR)/$(TARGET).cpp
	$(info => Creating Object File)
	$(CXX) -c $(CXXFLAGS) $(OBJS_DIR)/$(TARGET).o $(SRCS_DIR)/$(TARGET).cpp
