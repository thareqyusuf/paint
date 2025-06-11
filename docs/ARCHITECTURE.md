# Architecture Overview

## Design Principles

1. **Modular Design**: Functionality separated into logical modules
2. **Direct Hardware Access**: Uses Linux framebuffer for maximum performance
3. **Real-time Interaction**: Immediate visual feedback for user actions
4. **Memory Efficient**: Minimal memory footprint for embedded systems

## Component Interaction

```
┌─────────────────┐    ┌─────────────────┐
│   Input Layer   │    │  Graphics Layer │
│  (keypress.c)   │───▶│   (geometry.c)  │
└─────────────────┘    │   (filling.c)   │
                       │  (transform.c)  │
                       └─────────────────┘
                              │
                              ▼
                       ┌─────────────────┐
                       │  Core Layer     │
                       │ (framebuffer.c) │
                       │   (color.c)     │
                       └─────────────────┘
                              │
                              ▼
                       ┌─────────────────┐
                       │ Hardware Layer  │
                       │   (/dev/fb0)    │
                       └─────────────────┘
```

[Add more architectural details as needed]
