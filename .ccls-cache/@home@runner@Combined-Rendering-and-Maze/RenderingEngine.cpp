#include <iostream>
#include <cmath>
#include <utility>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

using namespace std;

#define WIDTH 100
#define HEIGHT 40

const int CENTER_X = WIDTH / 2;
const int CENTER_Y = HEIGHT / 2;

#define WALL_CHAR '#'
#define VERTICAL_EDGE_CHAR '|'
#define ROOF_CHAR '-'
#define FLOOR_CHAR '='
#define EMPTY_CHAR ' '

#define RELATIVE_TOLERANCE 0.15
#define WALL_LENGTH 5
#define WALL_WIDTH 5
#define FOCAL_LENGTH 5

char canvas[HEIGHT][WIDTH];

pair<int, int> convertGrid(double x, double y) {
    int x_grid = static_cast<int>(x * 5) + CENTER_X;
    int y_grid = CENTER_Y - static_cast<int>(y * 5);
    return make_pair(x_grid, y_grid);
}

void clearCanvas() {
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            canvas[i][j] = EMPTY_CHAR;
}

void displayCanvas() {
    cout << "\x1B[H";
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << canvas[i][j];
        }
        cout << "\n";
    }
    cout << flush;
}

pair<int, int> calculateWallPoint(double u, double v, double w, double t, double s, double f) {
    double sin_w = sin(w);
    double cos_w = cos(w);
    double denominator = (-(u) * cos_w + f) - s;
    if (denominator == 0) denominator = 0.0001;  // Avoid division by zero

    double x_prime = (f * ((u) * sin_w + t)) / denominator;
    double y_prime = (f * (v)) / denominator;

    return convertGrid(x_prime, y_prime);
}

pair<int, int> calculateRoofFloorPoint(double u, double v, double w, double s, double t, double k, double f) {
    double sin_w = sin(w);
    double cos_w = cos(w);
    double denominator = -(u) * cos_w + f - s;
    if (denominator == 0) denominator = 0.0001;  // Avoid division by zero

    double x_prime = (f * (v + t)) / denominator;
    double y_prime = (f * ((u) * sin_w + k)) / denominator;

    return convertGrid(x_prime, y_prime);
}

void drawWall(double position, double depth, double w = 0, char character = WALL_CHAR) {
    for (double u = -WALL_LENGTH / 2.0; u <= WALL_LENGTH / 2.0; u += 0.1) {
        for (double v = -WALL_WIDTH / 2.0; v <= WALL_WIDTH / 2.0; v += 0.1) {
            pair<int, int> coords = calculateWallPoint(u, v, w, position, depth, FOCAL_LENGTH);
            int x = coords.first;
            int y = coords.second;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                if (fabs(fabs(u) - WALL_LENGTH / 2.0) < RELATIVE_TOLERANCE) {
                    canvas[y][x] = VERTICAL_EDGE_CHAR;
                } else {
                    canvas[y][x] = character;
                }
            }
        }
    }
}

void drawRoofFloor(double t, double k, double s, double w = 0, char character = ROOF_CHAR) {
    for (double u = -WALL_LENGTH / 2.0; u <= WALL_LENGTH / 2.0; u += 0.1) {
        for (double v = -WALL_WIDTH / 2.0; v <= WALL_WIDTH / 2.0; v += 0.1) {
            pair<int, int> coords = calculateRoofFloorPoint(u, v, w, s, t, k, FOCAL_LENGTH);
            int x = coords.first;
            int y = coords.second;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                canvas[y][x] = character;
            }
        }
    }
}

enum wallCodes { L_WALL = 2, F_WALL = 1, R_WALL = 4 };

void drawBox(double depth, int wallCode, double shift = 0, double wallLength = WALL_LENGTH,
    double wallWidth = WALL_WIDTH) {
    drawRoofFloor(shift * wallLength, -wallWidth / 2, depth * wallLength, 0, ROOF_CHAR);
    drawRoofFloor(shift * wallLength, wallWidth / 2, depth * wallLength, 0, FLOOR_CHAR);

    if (wallCode >= R_WALL) {
        drawWall(-wallLength / 2 + (shift * wallLength), depth * wallLength, 0, WALL_CHAR);
        wallCode -= R_WALL;
    }
    if (wallCode >= L_WALL) {
        drawWall(wallLength / 2 + (shift * wallLength), depth * wallLength, 0, WALL_CHAR);
        wallCode -= L_WALL;
    }
    if (wallCode >= F_WALL) {
        drawWall((wallLength * shift), (depth * wallLength) + wallLength / 2, M_PI / 2, WALL_CHAR);
    }
}
