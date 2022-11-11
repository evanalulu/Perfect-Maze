/*
 * File: mazeSolution.cpp
 * --------------
 * Evana Pradhan
 *
 * Write your code for Assignment 4 (Maze) in the places indicated
 * in this file.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>
#include <vector>
#include "tigr.h"
#include "util.h"
#include "maze.h"

using namespace std;

bool allIncluded(Grid<bool>& g);
void makePerfect(Maze& m);
vector<pointT> neighbors(Maze& m, pointT p);
pointT randomNeighbor(Maze& m, pointT p);
vector<pointT> solveMaze(Maze& m);
void drawPath(Maze& m, stack<pointT> path, TPixel color);
void erasePath(Maze& m, stack<pointT> path);

const bool DRAW_PARTIAL_SOLUTIONS = true;
const bool PAUSE = true;
const int PAUSE_MS = 20; // milliseconds

int main() {
    srand(time(nullptr));
    Maze mz(20, 30, true);  // feel free to try different dimensions
    mz.initMazeGraphics(600, 450);
    mz.draw();
    getLine("Press ENTER and I will generate a maze");
    cout << "Generating maze...";
    makePerfect(mz);
    cout << "done." << endl;
    getLine("Press ENTER and I will solve the maze");
    cout << "Solving maze...";
    vector<pointT> solution = solveMaze(mz);
    cout << "done." << endl;
    getLine("Press ENTER and I will draw the solution");
    cout << "Drawing solution path...";
    for (pointT p : solution) {
        mz.drawMark(p, blue);
        mz.updateMazeGraphics();
        pause_ms(1);
    }
    cout << "done." << endl;
    cout << "Please close the maze window to exit the program" << endl;
    mz.waitForWindowClosed();
    return 0;
}

void makePerfect(Maze& m) {
    pointT p;
    Grid<bool> included(m.numRows(), m.numCols());
    // Mark all cells as "excluded" from the maze
    for (int i = 0; i < m.numRows(); i++)
        for (int j = 0; j < m.numCols(); j++)
            included[i][j] = false;

    // Set the current point (p) to be a randomly chosen cell,
    // and mark it as "included"
    p.row = rand() % m.numRows();
    p.col = rand() % m.numCols();
    included[p.row][p.col] = true;

    int iterations = 0;
    while (!allIncluded(included)) {
        // Randomly choose one of the neighbors of the current point
        pointT n = randomNeighbor(m, p);


        // If the chosen neighbor (n) is "excluded",
        // remove the wall between the current point (p) and the neighbor,
        // and mark the neighbor as "included" 
        if (included[n.row][n.col] == false) {
            m.setWall(p, n, false);
            included[n.row][n.col] = true;
        }

        // Set current point (p) to be the chosen neighbor
        p = n;

        // Re-render after every 100 iterations
        if (++iterations % 100 == 0)
            m.updateMazeGraphics();
    }
    // Re-render in case there were less than 100 iterations
    m.updateMazeGraphics();
}

bool allIncluded(Grid<bool>& g) {
    // Returns true if every element of g is true ("included"),
    // and returns false otherwise

    for (int i = 0; i < g.numRows(); i++) {
        for (int j = 0; j < g.numCols(); j++)
            if (g[i][j] == false) {
                return false;
            }
    }

    return true;
}


vector<pointT> neighbors(Maze& m, pointT p) {
    // Returns a vector containing all of the neighbors
    // of the point p. You can use m.pointInBounds(q) to 
    // test whether a point q is within the bounds of the maze.

    vector<pointT> nbrs;

    pointT q;
    // South
    q.row = p.row - 1;
    q.col = p.col;
    if (m.pointInBounds(q)) {
        nbrs.push_back(q);
    }

    // North
    q.row = p.row + 1;
    if (m.pointInBounds(q)) {
        nbrs.push_back(q);
    }

    // West
    q.row = p.row;
    q.col = p.col - 1;
    if (m.pointInBounds(q)) {
        nbrs.push_back(q);
    }

    // East
    q.col = p.col + 1;
    if (m.pointInBounds(q)) {
        nbrs.push_back(q);
    }

    return nbrs;
}

pointT randomNeighbor(Maze& m, pointT p) {
    // Returns a randomly chosen neighbor of the point p in the given maze.
    // Remember that you can use the `neighbors` function defined above
    // to get a vector containing all the neighbors of p.
    
    vector<pointT> allNeighbors = neighbors(m, p);

    pointT n;
    int randomN = rand() % allNeighbors.size(); // 0,1,2,3
    n = allNeighbors[randomN];

    return n;
}

vector<pointT> validNeighbors(vector<pointT> neighbors, Maze m, pointT p) {
    // Returns a vector of all neighbors of point p considering the
    // wall. If a wall blocks off an actual neigbor, it is not a neighbor!

    vector<pointT> validNbrs;

    for (pointT n : neighbors) {
        if (!m.isWall(n, p)) {
            validNbrs.push_back(n);
        }
    }

    return validNbrs;
}


vector<pointT> solveMaze(Maze& m) {
    pointT start = {0, 0}; // lower-left cell
    pointT end = {m.numRows() - 1, m.numCols() - 1}; // upper-right cell
    stack<pointT> path;
    queue<stack<pointT>> q;
    path.push(start);
    q.push(path);


    // Implement the breadth-first search algorithm
    // as described in the assignment. You can use the functions `drawPath` and `erasePath`
    // (defined below) to visualize partial solutions. You can control the 
    // speed of the drawing by modifying the constants PAUSE and PAUSE_MS
    // at the top of this file.

    // Set that stores in all visited points.
    // Initially, all points are not visited/excluded
    Grid<bool> included(m.numRows(), m.numCols());
    for (int i = 0; i < m.numRows(); i++) {
        for (int j = 0; j < m.numCols(); j++) {
            included[i][j] = false;
        }
    }

    // Boolean to run while loop that stores if the solution has been found.
    bool solFound = false;

    while (!solFound) {

        path = q.front();
        // drawPath(m, path, black);

        // Dequeing the path
        q.pop();

        // Tracking current point curr and marking it as visited/included
        pointT curr = path.top();
        included[curr.row][curr.col] = true;
        // erasePath(m, path);

        // If the current path ends at the end point, solution is found.
        if ((curr.row == end.row) && (curr.col == end.col))
            solFound = true;

        // Storing valid neighbors using vector container
        vector<pointT> validNeighbors = neighbors(m, curr);

        for (pointT neighbor:validNeighbors) {
            pointT n = neighbor;

            // If no wall and has not been visited yet
            if ((!m.isWall(curr, n)) && (!included[n.row][n.col])) {

                // If n equal to the end point
                if (n.row == end.row && n.col == end.col) {
                    path.push(n);
                    solFound = true;
                }

                // Making a copy of path, and storing paths in that copy to push into the queue
                stack<pointT> pathCopy = path;
                pathCopy.push(n);
                q.push(pathCopy);
                included[n.row][n.col] = true;
            }
        }


        // Code that I do not want to delete
        // visited.insert(curr);

        // for (pointT n : neighbors(m, curr)) {
        //     if (!m.isWall(n, curr)) {
        //         bool is_visited = visited.find(curr) != visited.end();
        //         if (!is_visited) {
        //             visited.insert(curr);
        //         }

        //         cout << "valid neighbors: " << n.row << ", " << n.col << endl;
        //         if (!(visited.find(n) != visited.end()))
        //             validNeighbors.push_back(n);
        //     }
        // }

        // for (pointT n: validNeighbors) {
        //     path.push(n);
        // }
    }

    // At this point, `path` should be a solution of the maze, so we
    // convert this stack into a vector and return the vector as our solution
    vector<pointT> solution;
    while (!path.empty()) {
        solution.insert(solution.begin(), path.top());
        path.pop();
    }
    return solution;
}

void drawPath(Maze& m, stack<pointT> path, TPixel color) {
    vector<pointT> vpath;
    while (!path.empty()) {
        vpath.insert(vpath.begin(), path.top());
        path.pop();
    }
    for (pointT p : vpath) {
        m.drawMark(p, color);
        m.updateMazeGraphics();
        if (PAUSE)
            pause_ms(PAUSE_MS);
    }
}

void erasePath(Maze& m, stack<pointT> path) {
    vector<pointT> vpath;
    while (!path.empty()) {
        vpath.insert(vpath.begin(), path.top());
        path.pop();
    }
    for (pointT p : vpath) {
        m.eraseMark(p);
    }
    m.updateMazeGraphics();
}
