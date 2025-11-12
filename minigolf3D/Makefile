CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = -lglfw -lGL -lGLU -lm
SOURCES = src/main.c src/game.c src/renderer.c src/physics.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = minigolf3D

# For software renderer version
TARGET_SOFTWARE = minigolf3D_software

all: $(TARGET) $(TARGET_SOFTWARE)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(TARGET_SOFTWARE): CFLAGS += -DSOFTWARE_RENDERER
$(TARGET_SOFTWARE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET_SOFTWARE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET_SOFTWARE)

.PHONY: all clean