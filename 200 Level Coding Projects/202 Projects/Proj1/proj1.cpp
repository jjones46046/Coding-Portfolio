#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

const int rows = 4;
const int colms = 4;
void startGame(string&);
int printBoard(int a[rows][colms]);
int selectTile(int&, int&);
int establishBoard(string &file, int a[rows][colms]);
int checkDirection(int &cord1, int &cord2, int a[rows][colms], int &freeCord1, int &freeCord2);
void checkVictory(int a[rows][colms], bool &check);


int board[rows][colms];
int currentCoard1, currentCoard2;
int freeCord1, freeCord2;
bool checkvictory = false;
string filename = "default.txt";

int main() {
  startGame(filename);
  board[rows][colms] = {establishBoard(filename, board)};
  do {
    cout << endl;
    printBoard(board);
    selectTile(currentCoard1, currentCoard2);
    board[rows][colms] = {checkDirection(currentCoard1, currentCoard2, board, freeCord1, freeCord2)};
    checkVictory(board, checkvictory);
  }
  while (checkvictory == false);
}//Executes the game order until a victory occurs

void checkVictory(int a[rows][colms], bool &check) {  
  int i = 0, j = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colms; j++) {
      if ((a[j][i] != 0 or 15)) { //If this number isnt zero
	if (a[j + 1][i] == 0) { //(If the next number is zero)
	  if (a[j][i] > a[0][i + 1]) { //Checks the next number to see if the original is greater
	    check = false;
	    cout << a[j][i] << endl;
	    cout << "here1" << endl;
	    break;
	  }
	}
      }
	else { //Numbers whos next isnt a zero
	  if (a[j][i] > a[j + 1][i]) { 
	    check = false;
	    cout << a[j][i] << endl;
	    cout << j << endl;
	    cout << i << endl;
	    cout << a[j + 1][i] << endl;
	    cout << "here2" << endl;
	    break;
	  }
	}
    
      if ((a[j][i] = 0)) { //For the zero
        if ((a[3][3] =! 0)) {
	  cout << "here3" << endl;
	  check = false;
	} 
      }
      else if ((a[j][i] = 15)) { //For the 15
	break;
      }
    }
  }
}

int checkDirection(int &cord1, int &cord2, int a[rows][colms], int &freeCord1, int &freeCord2) {
  int bufferX = 5, bufferY = 6;
  int i = 0, j = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colms; j++) {
      if (a[j][i] == 0) { //Finding the cordinates of the empty space
	freeCord1 = j;
	freeCord2 = i;
      }
      }
    }
  //Indicates the tile slid right
  if (cord1 + 1 == freeCord1 and cord2 == freeCord2) {
    cout << "The tile slid right" << endl;
      }
  //Indicates the tile slid left
  else if (cord1 - 1 == freeCord1 and cord2 == freeCord2) {
    cout << "The tile slid left" << endl;
      }
  //Indicates the tiles slid up
  else if (cord1 == freeCord1 and cord2 - 1 == freeCord2) {
    cout << "The tile slid up" << endl;;
      }
  //Indicates the tile slid down
  else if (cord1 == freeCord1 and cord2 + 1 == freeCord2) {
    cout << "The tile slid down" << endl;
  }
  else {
    cout << "This tile cannot be slid, please pick again" << endl;
    selectTile(currentCoard1, currentCoard2); //Fail safe if the user enters an unslideable cordinate
  }
    a[bufferX][bufferY] = a[cord1][cord2];
    a[cord1][cord2] = a[freeCord1][freeCord2];
    a[freeCord1][freeCord2] = a[bufferX][bufferY];
    cout << "swap complete" << endl; //Swaps the free space with the selected tile

    return a[rows][colms];
}


int selectTile(int &cord1, int &cord2) {
  bool Test = false;
  while (Test == false) {
    cout << "What is the row (left) and column (top) to slide" << endl;
    cin >> cord1;
    //Asks the user for tile cordinates
    if (cord1 > 4) {
      cout << "Invalid entry, please try again" << endl;
      selectTile(currentCoard1, currentCoard2);
      break;
    }
    //Prompts user to enter another cord upon illegal entry
      else {
       cin >> cord2;
      }
  
    if (cord2 > 4) {
      cout << "Invalid entry, please try again" << endl;
    } else {
	cout << "The cords are (" << cord1 << "," << cord2 << ")." << endl;
	Test = true;
	cord2 = cord2 - 1;
	cord1 = cord1 - 1;
    }
  }
return 0;
  
    }


void startGame (string &file) {
  int firstResponse = 2;
      
  cout << "Welcome to the Fifteen Game" << endl << "Would you like to load a board" << endl;
  cout << "1. Yes" << endl << "2. NO " << endl;
  cin >> firstResponse;
  //Welcoems the user to the game and prompts
  if (firstResponse == 1) {
    cout << "Which board would you like to load in?" << endl;
    cin >> file;
    cout << "You loaded this board: " << file << endl;
      }
  
  else {
    cout << "Loading default board..." << endl;
      }
 }


int printBoard(int a[rows][colms]) {
  int i = 0, j = 0;
  int topRow[] = {1, 2, 3, 4};

    for (i = 0; i < 4; i++) {
      cout << "   " << topRow[i] << "  ";
    }
    i = 0;
    //Prints out the top row of the board.
    
    for (i = 0; i < rows; i++) {
      cout << endl;
      for (j = 0; j < colms; j++) {
	cout << "   " << a[j][i] << "  ";
    }
    }
    cout << endl;
    
  return 0;
}

int establishBoard(string &file, int a[rows][colms]) {
  int i = 0, j = 0;
  ifstream gameBoard;
  
  gameBoard.open(file);
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colms; j++) {
      gameBoard >> a[j][i];
      }
  } //For loop that streams in numbers from textfile and puts them in a multidimensional array.
  gameBoard.close();  //Closing textfile
  
  return a[rows][colms];
}
