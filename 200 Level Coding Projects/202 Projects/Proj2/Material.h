//Title: Material.h
//Author: Jeremy Dixon
//Date: 2/24/2020
//Description: This is part of the Subnautica in CMSC 202 @ UMBC

#ifndef MATERIAL_H //Include/Header Guard
#define MATERIAL_H //Include/Header Guard

#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

// Name: Material struct
// Desc - Used to store data about materials
struct Material{
  string m_name; //Name of the material
  string m_type; //Type of the material (raw,advanced, or unique)
  string m_material1; //Required material 1 to create 
  string m_material2; //Required material 2 to create
  int m_quantity; //Quantity of material owned
  int m_depth; //Additional depth an item adds
  Material(){} //Default constructor
  Material(string name, string type, string material1, string material2,
	   int quantity, int depth){
    m_name = name; //Name of this material
    m_type = type; //Type of the material (raw, advanced, or unique)
    m_material1 = material1; //Name of first required material to create
    m_material2 = material2; //Name of second required material to create
    m_quantity = quantity; //Number of the material
    m_depth = depth; //Depth this unique material adds
  }
};

#endif
