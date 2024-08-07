#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Pokemon.h"
#include "PokemonList.h"
#include "Game.h"
using namespace std;

Pokemon::Pokemon() {
  m_name = "default";
  m_index = 0;
  m_type = "default";
  m_strong = "default";
  m_health = MAX_HEALTH;
  m_next = nullptr;
}

Pokemon::Pokemon(int index, string name, string type, string critical) {
  m_name = name;
  m_index = index;
  m_type = type;
  m_strong = critical;
  m_health = MAX_HEALTH;
  m_next = nullptr;
}

Pokemon* Pokemon::GetNext() {
   return m_next;
}

string Pokemon::GetName() {
  return m_name;
}

int Pokemon::GetIndex() {
  return m_index;
}

string Pokemon::GetType() {
  return m_type;
}

int Pokemon::GetHealth() {
  return m_health;
}

string Pokemon::GetStrong() {
  return m_strong;
}

void Pokemon::SetHealth(int health) {
  m_health = health;
}

void Pokemon::SetNext(Pokemon* next) {
  m_next = next;
}
