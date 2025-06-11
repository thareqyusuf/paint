# Paint Application API Documentation

## Core Modules

### Framebuffer (`src/core/framebuffer.c`)
Low-level framebuffer operations for direct hardware access.

### Color System (`src/core/color.c`)
Color representation and manipulation utilities.

### Graphics Pipeline (`src/graphics/`)
- **geometry.c**: Basic geometric primitives (lines, circles, polygons)
- **filling.c**: Area filling algorithms (flood fill, boundary fill)
- **clipping.c**: Line and polygon clipping algorithms
- **transform.c**: 2D transformations (rotate, scale, translate)

### Input System (`src/input/keypress.c`)
Keyboard input handling for interactive controls.

### Data Structures (`src/utils/`)
- **point.c**: 2D point representation
- **pointqueue.c**: Queue data structure for algorithms

[Add more detailed API documentation as needed]
