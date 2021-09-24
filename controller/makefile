CC = g++
CFLAGS = -Wall -g -DNO_FREETYPE

TARGET = controller

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).cpp

clean:
			$(RM) $(TARGET)