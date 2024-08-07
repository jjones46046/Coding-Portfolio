//Title: proj3.cpp
//Author: Yangyin Xu/Jeremy Dixon
//Date: 3/9/2020
//Description: This is main for project 3.
#include "Pokemon.h"
#include "PokemonList.h"
#include "Game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

//Inside the parameters for main, we can pass arguments
//argc is the count of arguments
//argv are the parameters themselves

int main(int argc, char* argv[]) {
  srand(time(NULL)); //Seeds the random number generator
  cout << "Welcome to the Game" << endl;
  while (argc < 2) { //Checks to make sure the user passes a file to load
    cout << "You are missing a data file." << endl;
    cout << "Expected usage ./proj3 proj3_data.txt" << endl;
    cout << "File 1 should be a file with one or more Pokemon" << endl;
    return 0;
  }
    Game myGame(argv[1]); //Creates a new game passing the filename
  int result = myGame.Start(); //Returns the result of the battle
  //The user wins if the result is 1, cpu wins if the result is 2, error if result 0
  if (result == 0){ 
    cout << "ERROR occured in battle" << endl;
  }
  else if (result == 1) {
    cout  << "You won!!!" << endl;
  } else{
    cout << "CPU won!!!" << endl;
  }
  return 0;
}
/*
int main() {
  Game testGame("testGame");
  testGame.LoadFile();
  testGame.ChooseTeam();
  testGame.Battle();
  return 0;
}
*/
