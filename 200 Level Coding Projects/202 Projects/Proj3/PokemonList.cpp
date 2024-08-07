#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Pokemon.h"
#include "PokemonList.h"
#include "Game.h"
using namespace std;


PokemonList::PokemonList() {
  m_head = nullptr; //Sets the pointer to point at nothing
  m_size = 0; //Sets the size of the linked list to 0
}

PokemonList::~PokemonList(){
  Pokemon* curr = m_head;
  while(curr != nullptr){ //Iterates through and removes each node
    m_head = curr; //sets m_head to curr
    curr = curr->GetNext(); //moves curr to next node
    delete m_head; //delete m_head;
  }
  //Resets all of the static linked list pointers
  m_head = nullptr; //Sets the pointer to point at nothing
  m_size = 0; //Sets the size of the linked list to 
  
}

void PokemonList::InsertEnd(Pokemon* choice){
  Pokemon* temp = m_head;
  
  if ((m_size == 0)) {  //If the list is empty, puts the choice as the first.
    m_head = choice;
  }
  
  else {  //Goes through the list and adds the pokemon at the end.
    temp = m_head;
    while (temp->GetNext() != nullptr) {
      temp = temp->GetNext();  //Traversing the list
    }
    temp->SetNext(choice);
  }
  
  m_size++; //Increments the size of the pokemon lsit
  temp = nullptr;
}

void PokemonList::Display() {
  Pokemon* temp = m_head;
  if ((m_size == 0)) {
    cout << "There is no pokemon here!" << endl;
  }
  
  else {
    cout << "-----------------------------" << endl;
    while(temp != nullptr) {
      cout << "Index: " << temp->GetIndex() << "    Name: " << temp->GetName();
      cout << "    Type: " << temp->GetType() << "    Health: " << temp->GetHealth() << endl;
      temp = temp->GetNext();
    }
    cout << "**END OF INVENTORY**" << endl;
    cout << "-----------------------------" << endl;
  }
  temp = nullptr;
}


void PokemonList::Transfer(int index, PokemonList* list) {
  Pokemon* temp = m_head;
  int  newIndex = 0;
  string newName,newType,newStrong;
  
  if ((m_size == 0)) {
    cout << "There is no pokemon to transfer!" << endl;
    //Checks to make sure the list isnt empty. 
  }
  
  else {
    while(temp != NULL and index != temp->GetIndex()) {
      temp = temp->GetNext();
      //Traversing the linked list.
    }
    
    newName = temp->GetName();    newType = temp->GetType();
    newStrong = temp->GetStrong();    newIndex = temp->GetIndex();
    //Initializing variables that will be used to create the new pokemon.
    
    temp = new Pokemon(newIndex, newName, newType, newStrong);
    list->InsertEnd(temp);
  }
  temp = nullptr;
  Remove(index);
}

void PokemonList::Remove(int index) {
  Pokemon* temp = m_head;
  Pokemon* prev;

  if (index == m_head->GetIndex()) {
    //Special case for first node of linked list.
    temp = m_head->GetNext();
    delete m_head;
    //Copies over data of the next pokemon to be the new head;
    m_head = temp;
    m_size--;
    //temp = nullptr;
  }
  
  else {
    while(temp != NULL and index != temp->GetIndex()) {
      prev = temp;
      temp = temp->GetNext();
      //Traversing the linked list.
    }
    
    if (index == temp->GetIndex()) {
      //Case for the rest of the nodes
      prev->SetNext(temp->GetNext());
      delete temp;
      temp = nullptr;
      m_size--;
    }
  }
  
}
    
int PokemonList::GetSize() {
  return m_size;
}

Pokemon* PokemonList::GetHead() {
  return m_head;
    }

int PokemonList::Attack(PokemonList* TeamRocket) {
  Pokemon* userPoke = GetHead();
  Pokemon* enemyPoke = TeamRocket->GetHead();
  int newHealth = 0;
  int faintCase = 0;
  
  if (userPoke->GetStrong() == enemyPoke->GetType()) {
    //Checking for critical attacks
    newHealth = enemyPoke->GetHealth();
    newHealth = newHealth - STRONG_DAMAGE;
    cout << "Critical hit!" << endl;
    //Subtracts 5(Strong damage) from the enemy's health
  }
  
  else {
    newHealth = enemyPoke->GetHealth();
    newHealth = newHealth - DAMAGE2ENEMY;
  }
    enemyPoke->SetHealth(newHealth);
    //Changing the enemy pokemons health to relfect the attack
  if (enemyPoke->GetHealth() <= 0) {
    faintCase = 1;
    cout << enemyPoke ->GetName() << " has fainted!" << endl;
    //Indicates the enemy pokemon has fainted.
  }
  
  //------------Enemy turn---------------
  
  if(faintCase != 1) {
    if (enemyPoke->GetStrong() == userPoke->GetType()) {
      //Checking for critical attacks
      newHealth = userPoke->GetHealth();
      newHealth = newHealth - STRONG_DAMAGE;
      cout << "Critical hit!" << endl;
      //Subtracts 5(Strong damage) from the enemy's health
    }
    
    else {
      newHealth = userPoke->GetHealth();
      newHealth = newHealth - DAMAGE2USER;
    }
    
    userPoke->SetHealth(newHealth);
    //Changing health of the user's pokemon to reflect the attack
    if (userPoke->GetHealth() <= 0) {
      faintCase = 2;
      cout << userPoke ->GetName() << " has fainted!" << endl;
      //Indicates the enemy pokemon has fainted.
    }
  }

  
  switch(faintCase) {
    
  case 0:
    //Neither pokemon fainted.
    return 0;
    break;
  case 1:
    //Enemy pokemon fainted.
    return 1;
    break;
  case 2:
    //Friendly pokemon fainted.
    return 2;
    break;
  }
  delete userPoke;
  delete enemyPoke;
  
}
    
void PokemonList::SwapPokemon() {
  Pokemon* temp;
  Pokemon* prev;
  Pokemon* holder;
  Pokemon* holder2;
  int index = 0;
  
  cout << "Enter the index of the pokemon you want to swap: " << endl;
  Display();
  cin >> index;
  //Grabbing the user's request for swap
  while(Exist(index) != 1) {
    cin >> index;
  }
  
  if (index == m_head->GetIndex()) {
    cout << "This pokemon is already at the top." << endl;
    temp = nullptr;
    //Checks for pokemon that is already at the helm.
  }
  
  else {
    temp = m_head;
    while (temp != NULL and index != temp->GetIndex()) {
      prev = temp;
      temp = temp->GetNext();
      //Traversing the list while also setting 'prev' to the pokemon before temp
    }
   
    if (prev == m_head) {
      //Special case where swapping the second and first node
      holder = m_head;
      m_head = temp;
      holder->SetNext(temp->GetNext());
      m_head->SetNext(holder);
    }
    
    else {
      holder = temp->GetNext();
      holder2 = m_head;
      m_head = temp;
      m_head->SetNext(holder2->GetNext());
      prev->SetNext(holder2);
      prev->GetNext()->SetNext(holder);
      //holder2->SetNext(holder);
      }
      //Swapping the selected index with the head of the list.
      temp = nullptr;
    Display();
      }

}


bool PokemonList::Exist(int index) {
  bool found = false;
  Pokemon* temp = m_head;
  
  while (index != temp->GetIndex()) {
    temp = temp->GetNext();
    if (temp == NULL) {
      break;
    }
    //traversing the list
  }
  
  if (temp != NULL and index == temp->GetIndex()) {
    found = true;
    //Indicates the pokemon was found in the pocket
  }
  if (found == false) {
    cout << "This pokemon isn't in the pokedex!" << endl;
  }
  
  return found;
}
