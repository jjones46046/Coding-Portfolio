//Title: Diver.h
0;136;0c//Author: Jeremy Dixon
//Date: 2/24/2020
//Description: This is part of the Subnautica Project in CMSC 202 @ UMBC
// This is the non-pointer version header files

#ifndef DIVER_H //Include/Header Guard
#define DIVER_H //Include/Header Guard

#include "Material.h" //Include for the material struct
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

//Constants (allowed to add more as needed)
const int PROJ2_SIZE = 50;

// class - allowed to add HELPER functions only
class Diver{
 public:
  // Name: Diver() - Default Constructor
  // Desc: Used to build a new Diver
  // Preconditions - Requires default values
  // Postconditions - Creates a new Diver
  Diver();
  // Name: Diver(name) - Overloaded constructor
  // Desc - Used to build a new Diver
  // Preconditions - Requires name
  // Postconditions - Creates a new Diver
  Diver(string name);
  // Name: GetName()
  // Desc - Getter for Diver name
  // Preconditions - Diver exists
  // Postconditions - Returns the name of the Diver
  string GetName();
  // Name: SetName(string)
  // Desc - Allows the user to change the name of the Diver
  // Preconditions - Diver exists
  // Postconditions - Sets name of Diver
  void SetName(string name);
  // Name: DisplayMaterials()
  // Desc - Displays a numbered list of all materials known by the Diver
  // Preconditions - Diver exists
  // Postconditions - Displays all materials
  void DisplayMaterials();
  // Name: CheckMaterial(Material)
  // Desc - Checks to see if the Diver has a material
  // Preconditions - Diver already has materials
  // Postconditions - Returns index of material if Diver has it else -1
  int CheckMaterial(Material);
  // Name: AddMaterial(string)
  // Desc - Inserts a material into the Diver's list of materials
  // Preconditions - Diver exists
  // Postconditions - Add a material to the Diver's m_myMaterials with a quantity of 0
  void AddMaterial(Material);
  // Name: IncrementQuantity(Material)
  // Desc - Adds quantity to list of materials the diver knows
  // Preconditions - Diver exists
  // Postconditions - Increases quantity of material for diver
  void IncrementQuantity(Material);
  // Name: DecrementQuantity(Material)
  // Desc - Reduces quantity from diver's inventory with true, if no quantity false
  // Preconditions - Diver exists
  // Postconditions - Reduces quantity of material for diver
  bool DecrementQuantity(Material);
  // Name: CheckQuantity(Material)
  // Desc - Checks to see if quantity of two merge materials are available
  // Preconditions - Diver exists with materials
  // Postconditions - Returns true if both are available (including two of same material)
  bool CheckQuantity(Material, Material);
  // Name: GetMaterial(int)
  // Desc - Checks to see if the diver has an material
  // Preconditions - Diver already has materials
  // Postconditions - Returns true if diver has material else false
  Material GetMaterial(int);
  // Name: CalcDepth(int)
  // Desc - Checks the total depth of all unique materials known/built
  // Preconditions - Diver already has materials
  // Postconditions - Returns depth sum of all unique materials owned/built
  int CalcDepth();
  
private:
  string m_name; //Name of Diver
  int m_numMaterials; //Current number of items in m_myMaterials known to diver
  Material m_myMaterials[PROJ2_SIZE] = {}; //List of materials known to this diver
};

#endif //Exit Header Guard
