#include "Diver.h"
#include "Material.h"
using namespace std;

//Create a new diver with default name, Phillip
Diver::Diver() {
  m_name;
  m_myMaterials[PROJ2_SIZE];
  m_numMaterials;
}

//Creates a new diver with a user set name
Diver::Diver(string name) {
  m_name = name;
  m_myMaterials[PROJ2_SIZE] = {};
  m_numMaterials = 0;
}

//Retrieves the diver's name
string Diver::GetName() {
  return m_name;
}

//Changes the diver's name
void Diver::SetName(string name) {
  m_name = name;
}

//Displays the materials in the divers inventory
void Diver::DisplayMaterials() {
  for (int i = 0, count = 1; i < m_numMaterials; i++,count++) {
    cout << count << ".  " <<  m_myMaterials[i].m_name << " " << m_myMaterials[i].m_quantity << endl;
  }
}

//Checks the Diver's inventory for a material
int Diver::CheckMaterial(Material mat) {
  int value = -1;
  for (int i = 0; i < PROJ2_SIZE; i++) {
    if (m_myMaterials[i].m_name == mat.m_name) {
      value = i;
      cout << "We found him" << "(" << mat.m_name << ") at: " << value << endl;
    }
  }
  
  return value;
}

//Adds a Material to the Diver's inventory
void Diver::AddMaterial(Material mat) {
  m_myMaterials[m_numMaterials] = mat;
  m_numMaterials += 1;
  // cout << "The number of mats is now: " << m_numMaterials << endl;
}

void Diver::IncrementQuantity(Material mat) {
  for (int i = 0; i < m_numMaterials; i++) {
    if (mat.m_name == m_myMaterials[i].m_name) {
      m_myMaterials[i].m_quantity += 1;
      cout << "Added one mat to:" << m_myMaterials[i].m_name << endl;
    }
    else {
      continue;
    }
  }
}

bool Diver::DecrementQuantity(Material mat) {
 for (int i = 0; i < m_numMaterials; i++) {
   if (mat.m_name == m_myMaterials[i].m_name) {
     if (m_myMaterials[i].m_quantity >= 1) {
       m_myMaterials[i].m_quantity -= 1;
       cout << "Subtracted one from mat :" << m_myMaterials[i].m_name << endl;
       return true;
    }
     else {
       return false;
     }
   }
   else {
      continue;
    }
 } //End of for loop
 
} //End of function

bool Diver::CheckQuantity(Material mat1, Material mat2) {
  int index1 = CheckMaterial(mat1);
  int index2 = CheckMaterial(mat2);

  if (index1 == -1 or index2 == -1) { //The materials arent in the divers inventory
    cout << "The diver doesn't have one of these materials" << endl;
    return false;
  }
  else if (index1 != -1 and index2 != -1)  {  //These materials are in the inventory
    if (m_myMaterials[index1].m_name == m_myMaterials[index2].m_name) { //If your combining the same two
      if (m_myMaterials[index1].m_quantity < 2) {
	cout << "Not enough materials of same type to combine these two" << endl;
	return false;
      }
      else {
	cout << "There is enough materials" << endl;
	return true;
      }
    }
    else if (m_myMaterials[index1].m_name != m_myMaterials[index2].m_name) { //Not the same two mats
      if (m_myMaterials[index1].m_quantity >= 1 and m_myMaterials[index2].m_quantity >= 1) {
	cout << "There is enough materials #2" << endl;
	return true;
      }
      else {
	cout << "There isn't enough materials #2" << endl;
	return false;
      }
    }
  }
  
}

Material Diver::GetMaterial(int index) {
  Material mat;
    mat = m_myMaterials[index];
  
  return mat;
}

int Diver::CalcDepth() {
  int sum = 0;
  for (int i = 0; i < m_numMaterials; i++) {
    if (m_myMaterials[i].m_type == "unique") {
      sum = sum + m_myMaterials[i].m_depth*m_myMaterials[i].m_quantity;
    }
    else {
      continue;
    }
  }
  return sum; 
}
