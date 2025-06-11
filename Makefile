# Professional Framebuffer Graphics Engine Makefile

# Compiler and flags
CC = gcc
CFLAGS = -O2 -lm -std=gnu99 -w -Iinclude
LDFLAGS = -lpthread -lm
DEBUG_FLAGS = -g -DDEBUG -Wall -Wextra

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

# Source files organized by module
CORE_SOURCES = $(SRCDIR)/core/paint.c $(SRCDIR)/core/framebuffer.c $(SRCDIR)/core/color.c
GRAPHICS_SOURCES = $(SRCDIR)/graphics/geometry.c $(SRCDIR)/graphics/filling.c $(SRCDIR)/graphics/clipping.c $(SRCDIR)/graphics/transform.c $(SRCDIR)/graphics/game.c
INPUT_SOURCES = $(SRCDIR)/input/keypress.c
PHYSICS_SOURCES = $(SRCDIR)/physics/physics.c
UTILS_SOURCES = $(SRCDIR)/utils/point.c $(SRCDIR)/utils/pointqueue.c $(SRCDIR)/utils/grafika.c

ALL_SOURCES = $(CORE_SOURCES) $(GRAPHICS_SOURCES) $(INPUT_SOURCES) $(PHYSICS_SOURCES) $(UTILS_SOURCES)

# Object files
CORE_OBJECTS = $(CORE_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
GRAPHICS_OBJECTS = $(GRAPHICS_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
INPUT_OBJECTS = $(INPUT_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
PHYSICS_OBJECTS = $(PHYSICS_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
UTILS_OBJECTS = $(UTILS_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ALL_OBJECTS = $(CORE_OBJECTS) $(GRAPHICS_OBJECTS) $(INPUT_OBJECTS) $(PHYSICS_OBJECTS) $(UTILS_OBJECTS)

# Target executable
TARGET = framebuffer_graphics_engine

# Default target
.PHONY: all clean debug install help profile

all: $(TARGET)

# Create build directories
$(OBJDIR):
	@mkdir -p $(OBJDIR)/core
	@mkdir -p $(OBJDIR)/graphics
	@mkdir -p $(OBJDIR)/input
	@mkdir -p $(OBJDIR)/physics
	@mkdir -p $(OBJDIR)/utils

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link final executable
$(TARGET): $(ALL_OBJECTS)
	$(CC) -o $(TARGET) $(ALL_OBJECTS) $(LDFLAGS)
	@echo "✅ Professional framebuffer graphics engine built successfully!"
	@echo "🚀 Run with: sudo ./$(TARGET) (in TTY console)"

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "🐛 Debug build complete with symbols and additional warnings"

# Performance profiling build
profile: CFLAGS += -pg -O2
profile: clean $(TARGET)
	@echo "📊 Profiling build complete (use gprof for analysis)"

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR)
	@rm -f $(TARGET)
	@rm -f gmon.out *.txt
	@echo "🧹 Build artifacts cleaned"

# Install system-wide (requires sudo)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "📦 Installed to /usr/local/bin/$(TARGET)"

# Show available targets
help:
	@echo "🔧 Available build targets:"
	@echo "  all      - Build optimized release version (default)"
	@echo "  debug    - Build with debug symbols and warnings"
	@echo "  profile  - Build with profiling support"
	@echo "  clean    - Remove all build artifacts"
	@echo "  install  - Install system-wide (requires sudo)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "📋 Usage:"
	@echo "  make                    # Standard optimized build"
	@echo "  make debug             # Debug build for development"
	@echo "  sudo ./$(TARGET)       # Run (requires TTY console)"

# Dependency tracking (automatic header dependency detection)
-include $(ALL_OBJECTS:.o=.d)

$(OBJDIR)/%.d: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@
