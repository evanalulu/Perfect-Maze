/*
 * File: maze.cpp
 * --------------
 * A maze is mostly a grid of walls, plus a little drawing code.
 *
 * Last modified 2022-06-01 by Jeff Lutgen
 *
 * You should not need to modify this file.
 */

#include <cstdlib>
#include "maze.h"
#include "util.h"
#include "tigr.h"

Maze::Maze(int numRows, int numCols, bool hasWalls) : cells(numRows, numCols) {
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++)
            for (int d = 0; d < NumDirs; d++)
                cells[r][c].walls[d] = hasWalls;
    configured = false;
    win = nullptr;
    color = black;
}

Maze::~Maze() {
    if (win != nullptr) {
        delete win;
    }
}

int Maze::numRows() {
    return cells.numRows();
}

int Maze::numCols() {
    return cells.numCols();
}

bool Maze::pointInBounds(pointT p) {
    return (p.row >= 0 && p.row < numRows() && p.col >= 0 && p.col < numCols());
}

void Maze::draw() {
    tigrClear(win, white);
    if (!configured)
        configureGraphics();
    for (int r = 0; r < cells.numRows(); r++) {
        for (int c = 0; c < cells.numCols(); c++) {
            pointT p = {r, c};
            drawWallsForCell(p);
        }
    }
    tigrUpdate(win);
}

bool Maze::isWall(pointT p1, pointT p2) {
    if (!pointInBounds(p1) || !pointInBounds(p2))
        error("Point is not in bounds for maze");
    return cells[p1.row][p1.col].walls[neighborDir(p1, p2)];
}

void Maze::setWall(pointT p1, pointT p2, bool state) {
    if (!pointInBounds(p1) || !pointInBounds(p2))
        error("Point is not in bounds for maze");
    cells[p1.row][p1.col].walls[neighborDir(p1, p2)] = state;
    cells[p2.row][p2.col].walls[neighborDir(p2, p1)] = state;
    if (!configured)
        configureGraphics();
    drawWallsForCell(p1);
}

void Maze::drawMark(pointT p, TPixel color) {
    if (!pointInBounds(p))
        error("Point is not in bounds for maze");
    if (!configured)
        configureGraphics();
    double margin = cellSize * .3;
    double length = cellSize - 2 * margin;
    this->color = color;
    movePen(originX + p.col * cellSize + margin,
            originY - (p.row * cellSize + margin));
    drawLineTo(length, -length);
    movePen(originX + p.col * cellSize + margin,
            originY - (p.row * cellSize + cellSize - margin));
    drawLineTo(length, length);
}

void Maze::eraseMark(pointT p) {
    if (!pointInBounds(p))
        error("Point is not in bounds for maze");
    if (!configured)
        configureGraphics();
    double margin = cellSize * .15;
    tigrFillRect(win, originX + p.col * cellSize + margin,
                 originY - (p.row + 1) * cellSize + margin,
                 cellSize - 2 * margin, cellSize - 2 * margin, white);
}

Maze::dirT Maze::neighborDir(pointT p1, pointT p2) {
    if ((abs(p1.row - p2.row) + abs(p1.col - p2.col)) != 1)
        error("Points are not neighbors");
    if (p1.row != p2.row)
        return (p1.row < p2.row ? North : South);
    else
        return (p1.col < p2.col ? East : West);
}

void Maze::drawWallsForCell(pointT p) {
    movePen(originX + p.col * cellSize, originY - p.row * cellSize);
    this->color = cells[p.row][p.col].walls[South] ? black : white;
    drawLineTo(cellSize, 0);
    this->color = cells[p.row][p.col].walls[East] ? black : white;
    drawLineTo(0, -cellSize);
    this->color = cells[p.row][p.col].walls[North] ? black : white;
    drawLineTo(-cellSize, 0);
    this->color = cells[p.row][p.col].walls[West] ? black : white;
    drawLineTo(0, cellSize);
}

void Maze::configureGraphics() {
    int padding = 2;
    double width = win->w - padding;
    double height = win->h - padding;
    cellSize = std::min(width / numCols(), height / numRows());
    originX = (width - numCols() * cellSize) / 2;
    originY = numRows() * cellSize + (height - numRows() * cellSize) / 2;
    configured = true;
}

// void Maze::initMazeGraphics() {
//     if (win != nullptr) {
//         win->close();
//         delete win;
//     }
//     win = new GWindow();
// }

void Maze::initMazeGraphics(int width, int height) {
    if (win != nullptr) {
        tigrFree(win);
        // win->close();
        // delete win;
    }
    win = tigrWindow(width, height, "Maze", TIGR_AUTO);
}

void Maze::movePen(double x, double y) {
    curX = x;
    curY = y;
}

void Maze::drawLineTo(double x, double y) {
    tigrLine(win, curX, curY, curX + x, curY + y, color);
    curX += x;
    curY += y;
}

void Maze::updateMazeGraphics() {
    tigrUpdate(win);
}

void Maze::waitForWindowClosed() {
    while (!tigrClosed(win)) {
        tigrUpdate(win);
    }
}
