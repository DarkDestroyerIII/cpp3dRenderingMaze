#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <utility>
#include <string>

void clearCanvas();
void displayCanvas();
void drawBox(double depth, int wallCode, double shift = 0, double wallLength = 5, double wallWidth = 5);

#endif
