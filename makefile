CC = g++
CFLAGS = -Wall -std=c++17
SRCS = src/main.cpp src/tokeniser.cpp src/gparser.cpp src/fileWriter.cpp
TARGET = popp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean

clean:
	rm -f $(TARGET)