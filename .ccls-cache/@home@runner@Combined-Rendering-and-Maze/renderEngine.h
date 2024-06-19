#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <utility>
#include <string>

 enum wallCodes { L_WALL = 2, F_WALL = 1, R_WALL = 4 };

void clearCanvas();
void displayCanvas();
void drawBox(double depth, int wallCode, double shift = 0, double wallLength = 5, double wallWidth = 5);

#endif
