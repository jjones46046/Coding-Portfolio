#include "Game.h"
0;136;0c#include "Diver.h"
#include "Material.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

Game::Game() {
  string m_myName = " ";
  
  LoadMaterials();  //1.Loaded materials into m_materials
  
  cout << "What is the Diver's Name?" << endl;
  cin >> m_myName;
  Diver User;
  User.SetName(m_myName);
  
  for (int i = 0; i < PROJ2_SIZE; i++) {
    User.AddMaterial(m_materials[i]);  //3.Populates the diver's materials.
  }
  m_myDiver = User;
}

void Game::LoadMaterials() {
  ifstream Materials;
  string matType,matName,Ingredient1,Ingredient2,depth;
  int quantity = 0;
  
  Materials.open("proj2_data.txt");
  for (int i = 0; i < PROJ2_SIZE; i++) {
    for (int j = 0;j <= 4; j++) {
      switch(j) {
      case 0:
	getline(Materials,matName,',');
	break;
      case 1:
	getline(Materials,matType,',');
	break;
      case 2:
	getline(Materials,Ingredient1,',');
	break;
      case 3: 
	getline(Materials,Ingredient2,',');
	break;
      case 4:
	getline(Materials,depth);
	//Streams in the material characteristics
	stringstream geek(depth);
	int dep = 0;
	geek >> dep;


	Material mat = Material(matName,matType,Ingredient1,Ingredient2,quantity,dep);
	m_materials[i] = mat;
	break;
      }
    }
  }
  cout << "50 ingredients loaded" << endl;
  
}

void Game::StartGame() {
  GameTitle();
  int input = 0;
  
  if (m_myDiver.CalcDepth() > MAX_DEPTH) {
    cout << "Victory!" << endl; //Checks for victory
  }
  
  while (m_myDiver.CalcDepth() < MAX_DEPTH and input != 5) { //Checks for vitory to stop the loop
    input = MainMenu();
      
    switch(input) {
    case 1:
      m_myDiver.DisplayMaterials();
      break;
    case 2:
      SearchMaterials();
      break;
    case 3:
      CombineMaterials();
      break;
    case 4:
     CalcScore();
      break;
    case 5:
      break;
    }
    }
}

void Game::DisplayMyMaterials() {
  m_myDiver.DisplayMaterials();
}

int Game::MainMenu() {
  int userInput = 0;
  cout << "What would you like to do?" << endl;
  cout << "1. Display your Diver's Materials" << endl;
  cout << "2. Search for Raw Materials" << endl;
  cout << "3. Attempt to Merge Materials" << endl;
  cout << "4. See Score" << endl;
  cout << "5. Quit(Quitter) " << endl;
  //Displays the options of the Main Game menu
  cin >> userInput;
  
  return userInput;
}

void Game::SearchMaterials() {
  srand(time(0));
  int numRaw = 0;
  for (int i = 0; i < PROJ2_SIZE; i++){
    if (m_materials[i].m_type == "raw"){
      numRaw += 1;
    }
  }
  //Classifies materials as raw
  int randNum = rand() %numRaw;
  m_myDiver.IncrementQuantity(m_materials[randNum]);
  //Gives us a random raw material
  cout << "Found some: " << m_materials[randNum].m_name << endl;
}
 
void Game::CombineMaterials() {
  int mat1 = 0;
  int mat2 = 0;
  RequestMaterial(mat1);
  RequestMaterial(mat2);
  int request1 = mat1;
  int request2 = mat2; 

  int item = SearchRecipes(m_materials[request1].m_name, m_materials[request2].m_name);
  if (item == -1) { //Recipe doesn't exist
      cout << "These materials cannot be combined" << endl;
    }
  
  else { //Recipe does exist
      if (m_materials[item].m_type == "unqiue") { //If the item is unique.
	if (m_materials[item].m_quantity >= 1) { //Checks if there is already one.
	  cout << "Cannot build more than one unqiue item" << endl;
	}
      else { //Adds the crafted item and removes the ingredients
	m_myDiver.IncrementQuantity(m_materials[item]);
	m_myDiver.DecrementQuantity(m_materials[request1]);
	m_myDiver.DecrementQuantity(m_materials[request2]);
      }
      }
  else { //The item isn't unique
      if (m_myDiver.CheckQuantity(m_materials[request1],m_materials[request2]) == true) {
	m_myDiver.IncrementQuantity(m_materials[item]);
	m_myDiver.DecrementQuantity(m_materials[request1]);
	m_myDiver.DecrementQuantity(m_materials[request2]);
      }
      else {
	cout << "Not enough material to craft this item" << endl;
      }
  }
  }
}

void Game::RequestMaterial(int& choice) {
  cout << "Which materials would you like to merge?" << endl;
  cout << "To list known materials enter -1" << endl;
  cin >> choice;
}

int Game::SearchRecipes(string item1, string item2) {
  int recipeI = -1;
  
  for (int i = 0; i < PROJ2_SIZE; i++) {
    if (((m_materials[i].m_material1 == item1) and (m_materials[i].m_material2 == item2)) or ((m_materials[i].m_material1 == item2) and (m_materials[i].m_material2 == item1))) {
      cout << item1 << " and " << item2 << " can create " << m_materials[i].m_name << endl;
      recipeI = i; //Checking for both combinations of materials
    }
    
    else {
       continue;
    }
  }
  
  if ((recipeI == -1)) {
    cout << "Sorry no recipe" << endl;
  }
  
  return recipeI;
}

void Game::CalcScore() {
  cout << "  ***Stats***" << endl;
  cout << "The Great Diver " << m_myDiver.GetName() << endl; //Gives the current stats menu
  cout << "Maximum Depth: " << MAX_DEPTH << endl;
  cout << "Current Depth: " << m_myDiver.CalcDepth() << endl;
}
