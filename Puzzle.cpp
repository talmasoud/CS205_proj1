//
// Created by Thamer Almasoud on 5/8/21.
//

#include "Puzzle.h"
#include <iostream>
#include <cstdlib>
#include<queue>
using namespace std;
Puzzle::Puzzle() {
    vector<vector<int> > temp(3, vector<int>(3, 0));
    current_board = temp;
    goal = temp;
    row = 3;
    column = 3;
    hasMoved = false;
    depth = 0;
    move = "";

    //definitely a better way of doing this
    // hard code default board
    current_board = {{1,2,0},{4,5,3},{7,8,6}};
    // hard coded goal state
    goal ={{1,2,3},{4,5,6},{7,8,0}};
    // update location of blank
    row_blank = 0;
    column_blank = 2;

    // after populating board and storing goal state
    // we can calculate the misplaced tiles & manhattan distance
    misplacedTiles = calcMisplaced();
    manhattanDistance = calcManhattan();
}

// define a row x column board with 0's
// definitely a better way to do this?
Puzzle::Puzzle(int row, int column){
    // declare a row x column vector fill with 0
    vector<vector<int> > temp(row, vector<int>(column, 0));
    current_board = temp;
    goal = temp;
    this->row = row;
    this->column = column;
    hasMoved = false;
    depth = 0;
    move = "";
    // after populating board and storing goal state
    // we can calculate the misplaced tiles & manhattan distance
    misplacedTiles = 0;
    manhattanDistance = 0;

    // generate goal state
    int counter = 1;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(counter != (row * column)){
                goal[i][j] = counter;
                counter++;
            } else {       // last spot is a blank
                goal[i][j] = 0;
            }
        }
    }
}

void Puzzle::build (){
    int input;

    cout << "Enter your puzzle, use a zero to represent the blank" << endl;
    for(int i = 0; i < row; i++){
        cout << "Enter row " << i + 1 << ", use space between numbers:\t\t";
        for(int j = 0; j < column; j++){
            cin >> input;
            current_board[i][j] = input;
            if(input == 0){
                row_blank = i;
                column_blank = j;
            }
        }
    }

    // after populating the board we can calculate misplaced tiles & manhattan
    misplacedTiles = calcMisplaced();
    manhattanDistance = calcManhattan();
}

int Puzzle::general_search(Puzzle& p, int& queueing_function){
    // make queue and initalize with inital state
    //Not That Option 1 makes the search algo. works as BFS since g(n) =0 & g(n)=1
    queue<Puzzle> nodes;
    // keeps track of which state we've seen
    // that way we don't add repeated states into our queue
    // also checking this seems pretty costly since it's 9! states
    // for a 3x3 board...
    vector<Puzzle> repeatedStates;
        nodes.push(p);
    repeatedStates.push_back(p);
    unsigned int max = 0;            // keep track of maximum size of queue
    int nodesExpanded = 0;  // keeps track of # of nodes we expanded

    cout << "The board has the following element" << endl;
    p.printBoard();
    cout << endl;

    // basically an infinite loop
    while(1){
        // if queue is empty then return failure
        if(nodes.empty())
            return -1;

        // remove front of queue
        Puzzle node = nodes.front();
        nodes.pop();

        // if the node we're checking is in goal state return success
        if(node.equalGoalState()){
            cout << "Success!" << endl;
            node.printBoard();
            cout << endl;
            cout << "Expanded Node =  " << nodesExpanded << endl;
            cout << "The maximum number of nodes in the queue at any one time was " << max<< endl;
            cout << "The depth = " << node.getDepth()<< endl;
            cout << "Steps: " << node.getMove() << endl;
            return 1;
        }
        //node.printBoard();
        //cout<<"-------------------------"<<endl;
        // string queueing function to determine which to use:
        // 1 == uniform cost; 2 == misplacedTile; 3 == manhattanDistance
        // repeatedStates vector to check for repeated states
        nodes = queueing_algorithm(nodes, node, repeatedStates, queueing_function);

        nodesExpanded++;        // increment number of times to expand

        // keep track of maximum number of nodes at any one time
        if(nodes.size() > max)
            max = nodes.size();
    }
}
queue<Puzzle> Puzzle::queueing_algorithm(queue<Puzzle>& q, Puzzle& n, vector<Puzzle>& v, int& s){
    // we want to store what's in the queue so it can be sorted
    vector<Puzzle> tempVec;
    while(!q.empty()){
        tempVec.push_back(q.front());
        q.pop();
    }

    // expand operators move blank UP/DOWN/LEFT/RIGHT
    Puzzle temp = n;
    temp.moveBlankUp();
    Puzzle childUp = temp;

    temp = n;
    temp.moveBlankDown();
    Puzzle childDown = temp;

    temp = n;
    temp.moveBlankLeft();
    Puzzle childLeft = temp;

    temp = n;
    temp.moveBlankRight();
    Puzzle childRight = temp;

    if(childUp.getHasMoved() && !isRepeated(v, childUp)){
        tempVec.push_back(childUp);
        v.push_back(childUp);   // push into vector to keep track of repeats
    }
    if(childDown.getHasMoved() && !isRepeated(v, childDown)){
        tempVec.push_back(childDown);
        v.push_back(childDown);
    }
    if(childLeft.getHasMoved() && !isRepeated(v, childLeft)){
        tempVec.push_back(childLeft);
        v.push_back(childLeft);
    }
    if(childRight.getHasMoved() && !isRepeated(v, childRight)){
        tempVec.push_back(childRight);
        v.push_back(childRight);
    }

    // sort temp bc we want best heuristic to go first
    // if queueing function is 1 don't need to sort bc it's just BFS
    if(s == 2)   // misplaced
        SortMisplaced(tempVec, 0, tempVec.size() - 1);
    else if(s == 3) // manhattan
        SortManhattan(tempVec, 0, tempVec.size() - 1);

    //Enqueue the children nodes......
    for(int i = 0; i < tempVec.size(); i++){
            q.push(tempVec[i]);
    }

    return q;
}

// operators
bool Puzzle::moveBlankUp(){
    // if on the first row, you can't move the blank up
    if(row_blank == 0){
        //cout << "can't move up" << endl;
        hasMoved = false;
        return false;
    } else {
        // swapping blank up
        int temp = current_board[row_blank - 1][column_blank];
        current_board[row_blank - 1][column_blank] = 0;
        current_board[row_blank][column_blank] = temp;

        // update blank location
        row_blank -= 1;
        hasMoved = true;
        depth++;
        if(move == "")
            move = "up";
        else
            move += " up";
    }
    return true;
}

bool Puzzle::moveBlankDown(){
    // if on the bottom row, you can't move the blank down
    if(row_blank == row - 1){
        //cout << "can't move down" << endl;
        hasMoved = false;
        return false;
    } else {
        // // swapping blank down
        int temp = current_board[row_blank + 1][column_blank];
        current_board[row_blank + 1][column_blank] = 0;
        current_board[row_blank][column_blank] = temp;

        // update blank location
        row_blank += 1;
        hasMoved = true;
        depth++;
        if(move == "")
            move = "down";
        else
            move += " down";
    }
    return true;
}

bool Puzzle::moveBlankLeft(){
    // if on the left column, you can't move it left
    if(column_blank == 0){
        //cout << "can't move left" << endl;
        hasMoved = false;
        return false;
    } else {
        // // swapping blank left
        int temp = current_board[row_blank][column_blank - 1];
        current_board[row_blank][column_blank - 1] = 0;
        current_board[row_blank][column_blank] = temp;

        // update blank location
        column_blank -= 1;
        hasMoved = true;
        depth++;
        if(move == "")
            move = "left";
        else
            move += " left";
    }
    return true;
}

bool Puzzle::moveBlankRight(){
    // if on the right column, you can't move it right
    if(column_blank == column - 1){
        //cout << "can't move right" << endl;
        hasMoved = false;
        return false;
    } else {
        // // swapping blank right
        int temp = current_board[row_blank][column_blank + 1];
        current_board[row_blank][column_blank + 1] = 0;
        current_board[row_blank][column_blank] = temp;

        // update blank location
        column_blank += 1;
        hasMoved = true;
        depth++;
        if(move == "")
            move = "right";
        else
            move += " right";
    }
    return true;
}
Puzzle& Puzzle::operator=(const Puzzle& rhs){
    // HOW IS THIS THING WORKING???
    // these are all private data fields??
    current_board = rhs.current_board;
    goal = rhs.goal;
    row = rhs.row;
    column = rhs.column;
    row_blank = rhs.row_blank;
    column_blank = rhs.column_blank;
    depth = rhs.depth;
    hasMoved = rhs.hasMoved;
    move = rhs.move;
    return *this;
}

bool Puzzle::operator==(const Puzzle& rhs) const{
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(current_board[i][j] != rhs.current_board[i][j])
                return false;
        }
    }
    return true;
}

// calculates # of misplaced tiles
// g(n) = depth/cost, h(n) = # of misplaced tiles
int Puzzle::calcMisplaced(){
    misplacedTiles = 0;

    // iterate through the current board and compare [i][j] with the goal state
    // if it's different then increment misplace tiles.
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++)
            // don't count blanks as one
            if(current_board[i][j] != 0 && current_board[i][j] != goal[i][j]){
                misplacedTiles++;
            }
    }
    //cout << "misplaced Tiles: " << misplacedTiles << endl;
    return misplacedTiles;
}
// calculate the heuristic so misplaced + depth
int Puzzle::getMisplacedHeurstic(){
    return calcMisplaced() + depth;
}
// calculates manhattan distance
// f(n) = g(n) + h(n); g(n) = depth/cost to get to node h(n) = manhattan distance
int Puzzle::calcManhattan() {
    manhattanDistance = 0;

    int x1=0, y1 = 0, x2=0, y2=0, temp=0, dist=0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            // don't want to include blank space
            if (current_board[i][j] != goal[i][j] && current_board[i][j] != 0) {
                x1 = i;
                y1 = j;
                temp = current_board[i][j];
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < column; j++)
                        if (temp == goal[i][j])
                            dist = abs(x2 - x1) + abs(y2 - y1);
                    manhattanDistance += dist;
                }
            }
        }


    }
    return manhattanDistance;
}

int Puzzle::getManhattanHeurstic(){
    return calcManhattan() + depth;
}
//Implementing setters and getters
bool Puzzle::getHasMoved(){ return hasMoved; }
int Puzzle::getDepth(){ return depth; }
string Puzzle::getMove(){ return move; }
void Puzzle::setMove(string n){ move += " " + n; }

// prints n x n board
void Puzzle::printBoard() {
    for(int i = 0; i < current_board.size(); i++){
        for(int j = 0; j < current_board[i].size(); j++){
            cout << current_board[i][j] << " ";
            if(j == current_board[i].size() - 1){
                cout << endl;
            }
        }
    }
}

// print goal state
void Puzzle::printGoalState() {
    for(int i = 0; i < goal.size(); i++){
        for(int j = 0; j < goal[i].size(); j++){
            cout << goal[i][j] << " ";
            if(j == goal[i].size() - 1){
                cout << endl;
            }
        }
    }
}
// check if board is in goal State
bool Puzzle::equalGoalState(){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(current_board[i][j] != goal[i][j])
                return false;
        }
    }
    return true;
}
//sorting ............
void Puzzle::SortMisplaced(vector<Puzzle>& v, int l, int r){
    int i = l, j = r;
    Puzzle tmp;
    int pivot = v[(l + r) / 2].getMisplacedHeurstic();

    /* partition */
    while (i <= j) {
        while (v[i].getMisplacedHeurstic() < pivot)
            i++;
        while (v[j].getMisplacedHeurstic() > pivot)
            j--;
        if (i <= j) {
            tmp = v[i];
            v[i] = v[j];
            v[j] = tmp;
            i++;
            j--;
        }
    }

    /* recursion */
    if (l < j)
        SortMisplaced(v, l, j);
    if (i < r)
        SortMisplaced(v, i, r);
}
void Puzzle::SortManhattan(vector<Puzzle>& v, int l, int r){
    int i = l, j = r;
    Puzzle tmp;
    int pivot = v[(l + r) / 2].getManhattanHeurstic();

    /* partition */
    while (i <= j) {
        while (v[i].getManhattanHeurstic() < pivot)
            i++;
        while (v[j].getManhattanHeurstic() > pivot)
            j--;
        if (i <= j) {
            tmp = v[i];
            v[i] = v[j];
            v[j] = tmp;
            i++;
            j--;
        }
    }

    /* recursion */
    if (l < j)
        SortManhattan(v, l, j);
    if (i < r)
        SortManhattan(v, i, r);
}
// checks if the state is same as one inside the queue
bool Puzzle::isRepeated(const vector<Puzzle>& v, const Puzzle& p){
    for(int i = 0; i < v.size(); i++){
        if(v[i] == p)
            return true;
    }
    return false;
}
