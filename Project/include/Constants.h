#include <graphics.h>
#include <winbgim.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define INPUT_FILE "input.txt"

extern const int SIZE_MAX_WIDTH = getmaxwidth() * 0.75;
extern const int SIZE_MAX_HEIGHT = getmaxheight() * 0.75;
extern const float ZOOM_INCREMENT = 0.5;
extern const float ZOOM_DEFAULT = 1.0;

#endif
