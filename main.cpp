#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "renderEngine.h"

using namespace std;

// Constants for maze elements
const int PATH = 1;
const int WALL = 0;
const int DOOR = 2;

// Define directions
enum Direction { NORTH, EAST, SOUTH, WEST };

// Define the maze
const int maze[10][15] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Size of the maze
const int mazeHeight = sizeof(maze) / sizeof(maze[0]);
const int mazeWidth = sizeof(maze[0]) / sizeof(maze[0][0]);

// Player's initial position and direction
int playerX = 1;
int playerY = 1;
Direction playerDirection = EAST;

// Rendering distance
const int RENDER_DISTANCE = 4;

// Player symbols for each direction
char playerSymbols[4] = {'^', '>', 'v', '<'};

// Display mode flag
bool isMapEnabled = false;

// Function to move the player forward
void moveForward() {
    int newX = playerX;
    int newY = playerY;
    switch (playerDirection) {
        case NORTH: newY -= 1; break;
        case EAST:  newX += 1; break;
        case SOUTH: newY += 1; break;
        case WEST:  newX -= 1; break;
    }
    if (newX >= 0 && newX < mazeWidth && newY >= 0 && newY < mazeHeight && maze[newY][newX] != WALL) {
        playerX = newX;
        playerY = newY;
    }
}

// Function to turn the player right
void turnRight() {
    playerDirection = static_cast<Direction>((playerDirection + 1) % 4);
}

// Function to turn the player left
void turnLeft() {
    playerDirection = static_cast<Direction>((playerDirection + 3) % 4);
}

// Helper function to get the maze cell based on player direction and depth
int getMazeCell(int depth, int shift = 0) {
    int x = playerX;
    int y = playerY;
    if(depth > 0)
    --depth;

    if (playerDirection == NORTH) y -= depth, x -= shift;
    else if (playerDirection == EAST) x += depth, y -= shift;
    else if (playerDirection == SOUTH) y += depth, x += shift;
    else if (playerDirection == WEST) x -= depth, y += shift;
    
    if (x >= 0 && x < mazeWidth && y >= 0 && y < mazeHeight) {
        return maze[y][x];
    } else {
        return WALL;
    }
}

// Function to draw the 3D maze view
void draw3DMaze() {
    clearCanvas();
    cout << "\x1B[H";
    cout << "\x1B[3J";

    for (int depth = RENDER_DISTANCE; depth >= 1; --depth) {
        int wallCode = 0;

        if (getMazeCell(depth+1) == WALL) {
          wallCode += F_WALL;
          drawBox(depth,F_WALL);
        }

        if (getMazeCell(depth, -1) == WALL) {
            wallCode += R_WALL;
        } else {
            drawBox(depth, F_WALL, -1);
        }

        if (getMazeCell(depth, 1) == WALL) {
            wallCode += L_WALL;
        } else {
            drawBox(depth, F_WALL, 1);
        }

        drawBox(depth, wallCode);
        // if(depth == 1 && getMazeCell(depth +1) == WALL){
        //     drawBox(1, F_WALL);
        // }
    }

    displayCanvas();
}

// Function to display the 2D maze
void displayMaze() {
 cout << "\x1B[10F";//refresh the display
    
    //cout << "\x1B[1;1H";
    

    for (int i = 0; i < mazeHeight; ++i) {
        for (int j = 0; j < mazeWidth; ++j) {
            if (i == playerY && j == playerX) {
                std::cout << playerSymbols[playerDirection] << " ";
            } else if (maze[i][j] == WALL) {
                std::cout << "+ ";
            } else if (maze[i][j] == PATH) {
                std::cout << "  ";
            } else if (maze[i][j] == DOOR) {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string input;
    while (true) {
        draw3DMaze();
        if (isMapEnabled) displayMaze();
        
            
        std::cout << "Enter commands (w: move forward, d: turn right, a: turn left, m: toggle view): ";
        std::getline(std::cin, input);
        for (char command : input) {
            switch (command) {
                case 'w': moveForward(); break;
                case 'd': turnRight(); break;
                case 'a': turnLeft(); break;
                case 'm': isMapEnabled = !isMapEnabled; break;
                default: std::cout << "Invalid command!" << std::endl; break;
            }

 
            std::this_thread::sleep_for(std::chrono::milliseconds(75));
        }
    }
    return 0;
}
