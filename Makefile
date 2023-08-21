EXEC = main
CC = clang++

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

LIBS = 
CFLAGS = -Wall -Werror -Wno-unused -Wconversion -g -std=c++20 -I./include

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC)
