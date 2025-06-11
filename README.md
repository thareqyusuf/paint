# Framebuffer Paint Application

A graphics painting application that draws directly to the Linux framebuffer, featuring geometric drawing tools, transformations, and flood fill capabilities.

## Features

- **Drawing Tools**: Triangles, rectangles, and freeform polygons
- **Fill Operations**: Flood fill for coloring areas
- **Transformations**: Rotate, scale, and translate shapes
- **Color Support**: Multiple color modes
- **Real-time Interaction**: Keyboard-based controls
- **File I/O**: Save and load drawings

## Requirements

- Linux system with framebuffer support (`/dev/fb0`)
- GCC compiler
- Make build system
- Framebuffer access permissions

## Building

### Using Make
```bash
make
```

### Using Nix (recommended)
```bash
nix-shell  # or nix develop
make
```

## Running

The application requires framebuffer access and runs best in a TTY console:

1. Switch to a text console: `Ctrl+Alt+F2`
2. Navigate to the project directory
3. Run with appropriate permissions: `sudo ./paint`
4. Switch back to desktop: `Ctrl+Alt+F1`

## Controls

### Navigation
- **Arrow Keys**: Move cursor
- **+/-**: Zoom in/out
- **o/p**: Zoom polygons

### Drawing Modes
- **x**: Triangle drawing mode
- **c**: Rectangle drawing mode
- **Space**: Place points

### Colors
- **</>**: Cycle through colors (White, Red, Green, Blue)

### Operations
- **z**: Fill mode
- **e/r**: Rotate shapes left/right
- **k/l**: Scale shapes down/up
- **s**: Save drawing

## Project Structure

```
├── src/
│   ├── core/          # Core application logic
│   ├── graphics/      # Drawing and rendering
│   ├── input/         # Input handling
│   ├── physics/       # Physics simulation
│   └── utils/         # Utilities and data structures
├── include/           # Header files
├── examples/          # Example drawings and data
├── docs/             # Documentation
└── build/            # Build artifacts (created during build)
```

## Development

### Debug Build
```bash
make debug
```

### Clean Build
```bash
make clean
make
```

## License

[Add your license here]
