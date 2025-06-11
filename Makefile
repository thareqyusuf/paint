# Paint Application Makefile

# Compiler settings
CC = gcc
CFLAGS = -O2 -lm -std=gnu99 -w -Iinclude
LDFLAGS = -lpthread

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Source files
CORE_SOURCES = $(SRCDIR)/core/paint.c $(SRCDIR)/core/framebuffer.c $(SRCDIR)/core/color.c
GRAPHICS_SOURCES = $(SRCDIR)/graphics/geometry.c $(SRCDIR)/graphics/filling.c $(SRCDIR)/graphics/clipping.c $(SRCDIR)/graphics/transform.c $(SRCDIR)/graphics/game.c
INPUT_SOURCES = $(SRCDIR)/input/keypress.c
PHYSICS_SOURCES = $(SRCDIR)/physics/physics.c
UTILS_SOURCES = $(SRCDIR)/utils/point.c $(SRCDIR)/utils/pointqueue.c $(SRCDIR)/utils/grafika.c

ALL_SOURCES = $(CORE_SOURCES) $(GRAPHICS_SOURCES) $(INPUT_SOURCES) $(PHYSICS_SOURCES) $(UTILS_SOURCES)

# Target
TARGET = paint

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(ALL_SOURCES)
	$(CC) -o $(TARGET) $(ALL_SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -rf $(BUILDDIR)/*

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Development targets
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the paint application (default)"
	@echo "  clean   - Remove built files"
	@echo "  debug   - Build with debug information"
	@echo "  install - Install to /usr/local/bin (requires sudo)"
	@echo "  help    - Show this help message"
