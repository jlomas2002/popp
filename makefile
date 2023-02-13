CC = g++
CFLAGS = -Wall -std=c++17
SRCS = main.cpp tokeniser.cpp gparser.cpp
TARGET = parcelona

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean

clean:
	rm -f $(TARGET)