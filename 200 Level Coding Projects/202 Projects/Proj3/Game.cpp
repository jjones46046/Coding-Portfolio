#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Pokemon.h"
#include "PokemonList.h"
#include "Game.h"
using namespace std;

Game::Game(string name) {
  m_filename = name;
  m_list = new PokemonList();
  m_userPocket = new PokemonList();
  m_enemyPocket = new PokemonList();
}

Game::~Game() {
  delete m_list;
  delete m_userPocket;
  delete m_enemyPocket;
}

void Game::LoadFile() {
  ifstream Pokedex;
  string Name,Type,Strong,Index;
  int Ind = 0;
  int numPokemon = 151;
  Pokedex.open("proj3_data.txt");
  for (int i = 0; i < numPokemon; i++) {
    for (int j = 0;j < 4; j++) {
      switch(j) {
      case 0:
	getline(Pokedex,Index,',');
	break;
      case 1:
	getline(Pokedex,Name,',');
	break;
      case 2:
	getline(Pokedex,Type,',');
	break;
      case 3:
	getline(Pokedex,Strong);
	//Streams in the Pokemon's characteristics
	
	stringstream geek(Index);
	geek >> Ind;
	
	Pokemon* Poke = new Pokemon(Ind,Name,Type,Strong);
	m_list->InsertEnd(Poke);
	//cout << "Poke not deleted" << endl;
	//cout << "Poke deleted" << endl;
	Poke = nullptr;
	break;
      }
    }
  }
  Pokedex.close();
} // End of function


void Game::ChooseTeam() {
  int selection = 0;
  int usedNmbrs[6];
  int avalNmbrs[145];
  bool skip = false;
  int count = 0;
  int remainingPoke = 147;
  cout << "Here are the available pokemon:" << endl;
  m_list->Display();
  
  for (int i = 0; i < NUM_TEAM; i++) {
    cout << "Pick a Pokemon by entering index# (" << NUM_TEAM - i << " left):" << endl;
    cin >> selection;
    //Asks the user for a pokemon index
    while (m_list->Exist(selection) != 1) {
      cout << "Pick a Pokemon by entering index# (" << NUM_TEAM - i << " left):" << endl;
      cin >> selection;
      //Validating user's input. Reprompts if the pokemon isn't in m_list.
    }
    
    m_list->Transfer(selection, m_userPocket);
    usedNmbrs[i] = selection;
    //Moves the user selection to their pocket.
}
  cout << "Here's your team!" << endl;
  m_userPocket->Display();
  //Printing the user's team.

cout << "Loading enemy team...." << endl;

 for (int i = 1; i <= 151; i++) {
   skip = false;
   for (int x = 0; x < 5; x++) {
     if (usedNmbrs[x] == i) {
       skip = true;
       break;
     }
   }
   //Creating an array named "avalNmbrs" that has the remaining pokemon the enemy could get.
   
   if (skip == true) {
   }
   //Skipping pokemon index that have already been choosen by the user
   else {
     avalNmbrs[count] = i;
     count++;
     //Populating avalNumbrs
   }
 }
 
 // srand(time(NULL)); //GET RID OF THIS
 for (int i = 0; i < 5; i++) {
   int randNum = rand() %remainingPoke;
   //Choosing a random number from 0 to 146 since avalNmbrs starts at 0.
   if (m_list->Exist(avalNmbrs[randNum]) != 1) {
     int newNum = rand() %remainingPoke;
     m_list->Transfer(avalNmbrs[newNum], m_enemyPocket);
     //Failsafe that prevents identical pokemon from entering the enemy's pocket
   }

   else {
     m_list->Transfer(avalNmbrs[randNum], m_enemyPocket);
   }
 }
   cout << "Your Opponents!" << endl;
   m_enemyPocket->Display();
   //Printing the enemy's team.
}


int Game::Menu() {
  int choice = 0;

  cout << "Menu:" << endl;
  cout << "1. Attack" << endl;
  cout << "2. Swap" << endl;
  cout << "3. Forfeit" << endl;
  //Displays the three option menu
  cin >> choice;
  cout << endl;
  
  while (choice > 3 or choice < 1) {
    cout << "Thats invalid please choose again:" << endl;
    cin >> choice;
  }
  //Validating user input.
  return choice;
}


int Game::Battle() {
  int winner;
  int result = 0;
  int choice = 0;
  int roundNmbr = 1;
  Pokemon* userPoke;
  Pokemon* enemyPoke;
  
  while ((m_userPocket -> GetSize() != 0) and (m_enemyPocket->GetSize() != 0) and (choice != 3)) {
    //Continues the battle into a team is deplted of all its pokemon or user forfeits.
    choice = Menu();
    switch(choice) {
    case 1:
      break;
      //Does nothing and moves straight to the attack
    case 2:
      m_userPocket->SwapPokemon();
      break;
      //Allows the user to swap their pokemon
    case 3:
      winner = 2;
      break;
      //Makes the CPU the victor
    }
    if (choice != 3) {
      //Round commences if the user doesn't forfeit
      userPoke = m_userPocket->GetHead();
      enemyPoke = m_enemyPocket->GetHead();
      //Updating the pokemon that will fight.
      cout << "----------------" << endl << "     Round     " << roundNmbr << ":" << endl;
      cout << "CPU's Pokemon: " << enemyPoke->GetName() << " (" << enemyPoke->GetType();
      cout << ":" << enemyPoke->GetHealth() << " health)" << endl;
      //Displaying the enemy fighter
      cout << "Your Pokemon: " << userPoke->GetName() << " (" << userPoke->GetType();
      cout << ":" << userPoke->GetHealth() << " health)" << endl;
      cout << "----------------" << endl;
      
      result = m_userPocket->Attack(m_enemyPocket);
      if (result == 1) {
	m_enemyPocket->Remove(enemyPoke->GetIndex());
	  //Removes the enemy's pokemon if they fainted.
	  }
      else if (result == 2) {
	m_userPocket->Remove(userPoke->GetIndex());
	  //Removes the user's pokemon if they fainted.
	  }
      
      if(m_userPocket->GetSize() == 0) {
	winner = 2;
	//returns 2 for a cpu win.
      }

      else if(m_enemyPocket->GetSize() == 0) {
	winner = 1;
	//Returns 1 for a user win
      }
     
      roundNmbr++;
    }
  }

  return winner;
} //End of function


int Game::Start() {
  int result = 0;
  LoadFile();
  ChooseTeam();
  result = Battle();
  
  return result;
}
