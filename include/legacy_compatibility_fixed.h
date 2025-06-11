#ifndef LEGACY_COMPATIBILITY_H
#define LEGACY_COMPATIBILITY_H

// Only define legacy types if new types aren't already defined
#ifndef COLOR_H_INCLUDED
#ifndef Point
typedef struct coordinate_point Point;
#endif

#ifndef Color  
typedef struct color_rgba Color;
#endif
#endif

#endif // LEGACY_COMPATIBILITY_H
