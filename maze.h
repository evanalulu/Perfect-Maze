/*
 * File: maze.h
 * ------------
 * Defines the Maze class.
 * Last modified 2022-06-01 by Jeff Lutgen
 */

#ifndef _maze_h
#define _maze_h

#include "tigr.h"
#include "util.h"

/*
 * Class: Maze
 * -----------
 * This class is used to represent a maze and provide operations to
 * query and change the maze configuration. A maze is a rectangular grid
 * of cells. Each cell identified by pointT (row-col struct).
 * Coordinates in the maze are numbered starting at (0,0) in
 * the lower left corner.
 * A location has neighbors in the four major compass directions (some
 * neighbors may not exist for cells along the outer border).  The
 * maze tracks which walls are up between a cell and its neighbor.
 */

/*
 * Type: pointT
 * ------------
 * The type pointT is used to encapsulate a pair of integer
 * coordinates into a single value with row and col components.
 */

struct pointT {
    int row;
    int col;
};

// We define "<" for pointT so that we can create a set<pointT>
inline bool operator<(pointT const &p1, pointT const &p2) {
    if (p1.row < p2.row) return true;
    if (p1.row > p2.row) return false;
    if (p1.col < p2.col) return true;
    return false;
}

const TPixel black = tigrRGB(0, 0, 0);
const TPixel white = tigrRGB(255, 255, 255);
const TPixel red = tigrRGB(255, 0, 0);
const TPixel blue = tigrRGB(0, 0, 255);

class Maze {
   public:
    /*
     * Constructor: Maze
     * Usage: Maze m(10, 20, true);
     * ----------------------------
     * The constructor initializes a new maze of the specified dimensions.
     * If the hasWalls argument is true, the maze is initially configured with
     * all walls intact. If false, the maze starts with no walls at all.
     */
    Maze(int numRows, int numCols, bool hasWalls);

    /*
     * Destructor: ~Maze
     * Usage: called implicitly
     * ----------------------------
     */
    ~Maze();

    /*
     * Member functions: numRows, numCols
     * Usage: nRows = maze.numRows();
     * ------------------------------
     * These member functions return the number of rows or columns in
     * this maze.
     */
    int numRows();
    int numCols();

    /*
     * Member functions: pointInBounds
     * Usage: if (!maze.pointInBounds(pt))
     * -----------------------------------
     * This member function returns true if p is within bounds of this
     * maze, false otherwise.
     */
    bool pointInBounds(pointT p);

    /*
     * Member function: isWall
     * Usage: if (maze.isWall(a, b))...
     * ---------------------------------
     * This member function returns true if there is a wall between
     * the two cells at points p1 and p2. If the two points are
     * not neighbors or if either is out of bounds, an error is raised.
     */
    bool isWall(pointT p1, pointT p2);

    /*
     * Member function: setWall
     * Usage: maze.setWall(a, b, true);
     * --------------------------------
     * This member function sets the wall between cells at points
     * p1 and p2 to state. It can be used to either add or remove
     * walls. The graphical display is updated to match. If the two
     * points are not neighbors or either point is out of bounds,
     * an error is raised.
     */
    void setWall(pointT p1, pointT p2, bool state);

    /*
     * Member function: draw
     * Usage: maze.draw();
     * -------------------
     * This member function draws the maze configuration to the graphics
     * window, erasing any previous contents. The lower-left corner is
     * the cell identified by 0-0. The maze itself is white and walls are
     * drawn with black lines. All previous marks are cleared.
     */
    void draw();

    /*
     * Member function: drawMark
     * Usage: maze.drawMark(p, "Red");
     * ------------------------------
     * This member function draws a mark in the cell at point p
     * in the specified color. To erase a previous mark, you
     * can use the eraseMark function.
     */
    void drawMark(pointT p, TPixel color);

    /*
     * Member function: eraseMark
     * Usage: maze.eraseMark(p);
     * ------------------------------
     * This member function erases any mark in the cell at point p.
     */
    void eraseMark(pointT p);

    /*
     * Member function: initMazeGraphics
     * Usage: maze.initMazeGraphics(300, 400);
     * ------------------------------
     */
    // void initMazeGraphics();
    void initMazeGraphics(int width, int height);

    /*
     * Member function: updateMazeGraphics
     * Usage: maze.updateMazeGraphics();
     * ------------------------------
     */
    void updateMazeGraphics();

    /*
     * Member function: waitForWindowClosed
     * Usage: maze.waitForWindowClosed();
     * ------------------------------
     */
    void waitForWindowClosed();

   private:
    enum dirT { North, East, South, West, NumDirs };
    struct cellT {
        bool walls[NumDirs];
    };
    Grid<cellT> cells;
    double originX, originY, cellSize;
    bool configured;
    double curX, curY;
    Tigr* win;
    TPixel color;  // current drawing color

    dirT neighborDir(pointT p1, pointT p2);
    void drawWallsForCell(pointT p);
    void drawLineTo(double x, double y);
    void movePen(double x, double y);
    void configureGraphics();
};

#endif
