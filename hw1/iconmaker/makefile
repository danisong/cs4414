CC = g++
CFLAGS = -Wall -g -DNO_FREETYPE

TARGET = iconmaker

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).cpp pngwriter.cpp -lpng -lz

clean:
			$(RM) $(TARGET)