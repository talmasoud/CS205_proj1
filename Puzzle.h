//
// Created by Thamer Almasoud on 5/8/21.
//
#include <vector>
#include <iostream>
#include <string.h>
#include <queue>
using namespace std;
#ifndef PROJECT_1_CS205_PUZZLE_H
#define PROJECT_1_CS205_PUZZLE_H


class Puzzle {

private:
    vector<vector<int> > current_board;
    vector<vector<int> > goal;
    int row;
    int column;
    bool hasMoved;                      // changes when blank moves
    int misplacedTiles;                 // # of misplaced tiles
    int manhattanDistance;              // manhattan distance
    int depth;                          // depth of the node
    string move;
    // to keep track of where the blank is
    int row_blank;
    int column_blank;
public:
    Puzzle();                               //Default puzzle.
    Puzzle(int row, int column);            //create a board based on user's preference
    void build ();                       //populate the board
    bool equalGoalState();                  // checks if current board = goal
    static int general_search(Puzzle& p, int& queueing_function);
    static queue<Puzzle> queueing_algorithm(queue<Puzzle>& q, Puzzle& n, vector<Puzzle>& v, int& s);
    //Defining blank operators
    bool moveBlankUp();                 // slide blank up
    bool moveBlankDown();               // slide blank down
    bool moveBlankLeft();               // slide blank left
    bool moveBlankRight();              // slide blank right
    Puzzle& operator=(const Puzzle& rhs);
    bool operator==(const Puzzle& rhs) const;

    int calcMisplaced();                    // return number of misplaced tiles
    int calcManhattan();                    // return manhattan distance

    int getMisplacedHeurstic();
    int getManhattanHeurstic();

    //defining accessors and getters
    bool getHasMoved();
    int getDepth();                     // gets the depth
    string getMove();
    void setMove(string s);

    void printBoard();                  // output current board state
    void printGoalState();              // output the goal state
    static void SortMisplaced(vector<Puzzle>& v, int l, int r);
    static void SortManhattan(vector<Puzzle>& v, int l, int r);
   static bool isRepeated(const vector<Puzzle>& v, const Puzzle& p);


};


#endif //PROJECT_1_CS205_PUZZLE_H
