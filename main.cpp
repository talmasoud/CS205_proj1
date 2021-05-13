#include <iostream>
#include "Puzzle.h"

using namespace std;

int main() {
    Puzzle puzzle_game;
    int input, row, column;
    int search_type;
    int result;
    cout << "enter 1 to use a default 8 puzzle, or 2 to build yours" << endl;
    cin >> input;
    // Creating the game board
    if(input == 1)
        //setting up the default board
        puzzle_game = Puzzle();
    else if(input == 2){
        cout << "Enter number of rows : "<<endl;
        cin >> row;
        cout << "Enter number of columns: "<<endl;
        cin >> column;
        cout << "Building a : " << row << " X  " << column<<" sliding puzzle game" << endl;
        //setting up a customized board..
        puzzle_game = Puzzle(row, column);
        //letting the user filling the board
        puzzle_game.build();
    }
    else{
        cout<<"Invalid entry "<<endl;
    }
    //Choosing the type of algorithm
    cout << "choose a search algorithm:" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile Heuristic" << endl;
    cout << "3. A* with the Manhattan Distance Heuristic" << endl;
    cin >> search_type;
    //........................Find the solution ..................................
    result = Puzzle::general_search(puzzle_game,search_type );// Calling the search algorithm
    if (result == -1)
    cout<<"failure"<<endl;
    return 0;
}
