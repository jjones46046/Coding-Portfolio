//Title: Game.h
//Author: Jeremy Dixon
//Date: 2/24/2020
//Description: This is part of the Subnautica Project in CMSC 202 @ UMBC

#ifndef GAME_H //Header Guard
#define GAME_H //Header Guard
#include <fstream>
#include "Material.h"
#include "Diver.h"

//Constants (additional constants allowed)
const string PROJ2_DATA = "proj2_data.txt"; //File constant
const int MAX_DEPTH = 1000; // Win condition (when unique item's depth exceeds 1000 total)

//Additional HELPER functions allowed to be added as needed

class Game{
public:
  /* Name: Game() Default Constructor
  // Desc - Builds a new game by: 
  // 1. Loads all materials and their recipes into m_materials
  // 2. Asks user for their diver's name (store in m_myDiver as m_myName)
  // 3. Copies all materials from m_materials into the diver's material array
  // Preconditions - None
  // Postconditions - m_materials is populated
  */ 
  Game(); //Default Constructor
  // Name: LoadMaterials
  // Desc - Loads each material into m_materials from file
  // Preconditions - Requires file with valid material data
  // Postconditions - m_material is populated with material structs
  void LoadMaterials();
  // Name: StartGame()
  // Desc - Manages the game itself including win conditions continually
  //         calling the main menu 
  // Preconditions - Player is placed in game
  // Postconditions - Continually checks to see if player has entered 5
  void StartGame();
  // Name: DisplayMyMaterials()
  // Desc - Displays the diver's materials
  // Preconditions - Player's diver has materials
  // Postconditions:
  void DisplayMyMaterials();
  // Name: MainMenu()
  // Desc - Displays and manages menu. Also checks win condition (diver reaches 1000 meters)
  // Preconditions - Player has an Diver
  // Postconditions - Returns number including exit
  int MainMenu();
  // Name: SearchMaterials()
  // Desc - Attempts to search for raw materials (must be type "raw")
  // Preconditions - Raw materials loaded
  // Postconditions - Increases quantity in Diver's posession of materials
  void SearchMaterials();
  // Name: CombineMaterials()
  // Desc - Attempts to combine known materials. If combined, quantity of material decreased
  // Preconditions - Diver is populated with materials
  // Postconditions - Increments quantity of item "made", decreases quantity of source items
  void CombineMaterials();
  // Name: RequestMaterial()
  // Desc - Requests material to use to merge
  // Preconditions - Diver has materials to try and merge
  // Postconditions - Returns integer of item selected by user
  void RequestMaterial(int &choice);
  // Name: SearchRecipes()
  // Desc - Searches recipes for two strings (name of item)
  // Preconditions - m_materials is populated
  // Postconditions - Returns int index of matching recipe
  int SearchRecipes(string, string);
  // Name: CalcScore()
  // Desc - Displays current score for Diver
  // Preconditions - Diver is populated with materials
  // Postconditions - Displays total
  void CalcScore();
  // Name: GameTitle()
  // Desc - Title for Game (Do not edit)
  // Preconditions - None
  // Postconditions - None
  void GameTitle(){
    cout << "*****************************************************************" << endl;
    cout << " SSSS  U   U  BBBB   N    N   AAA   U   U  TTTTT  II  CCCC   AAA " << endl;
    cout << " S     U   U  B   B  NN   N  A   A  U   U    T    II  C     A   A" << endl;
    cout << " S     U   U  B   B  N N  N  A   A  U   U    T    II  C     A   A" << endl;
    cout << " SSSS  U   U  BBBB   N  N N  AAAAA  U   U    T    II  C     AAAAA" << endl;
    cout << "    S  U   U  B   B  N  N N  A   A  U   U    T    II  C     A   A" << endl;
    cout << "    S  U   U  B   B  N   NN  A   A  U   U    T    II  C     A   A" << endl;
    cout << " SSSS  UUUUU  BBBB   N    N  A   A  UUUUU    T    II  CCCC  A   A" << endl;
    cout << "*****************************************************************" << endl;
  }
 Material m_materials[PROJ2_SIZE]; //All materials in the game (loaded from file)
  Diver m_myDiver; //Player's Diver for the game
private:
};

#endif //Exit Header Guard
