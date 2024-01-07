#include <graphics.h>
#include <winbgim.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define INPUT_FILE "input.txt"

extern const int SIZE_MAX_WIDTH = getmaxwidth() * 0.75;
extern const int SIZE_MAX_HEIGHT = getmaxheight() * 0.75;
extern const float ZOOM_INCREMENT = 1.1;
extern const float DEFAULT_ZOOM = 1.0;
float zoom = 1.0;

#endif
