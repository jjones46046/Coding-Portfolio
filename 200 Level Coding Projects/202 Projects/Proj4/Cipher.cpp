#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "Cipher.h"
#include "Caesar.h"
using namespace std;


Cipher::Cipher() {
  m_message = "default";
  m_isEncrypted = 0;
}

Cipher::Cipher(string message, bool isEncrypted) {
  m_message = message;
  m_isEncrypted = isEncrypted;
}

Cipher::~Cipher() {
}

void Cipher::Decrypt() {
}

void Cipher::Encrypt() {
}

string Cipher::GetMessage() {
  return m_message;
}

bool Cipher::GetIsEncrypted() {
  return m_isEncrypted;
}

void Cipher::SetMessage(string message) {
  m_message = message;
}

void Cipher::ToggleEncrypted() {
  if (m_isEncrypted == 0) {
    m_isEncrypted = 1;
    //Makes isEncrypted true.
  }
  
  else {
    m_isEncrypted = 0;
    //Makes isEncrypted False.
  }
  
}

string Cipher::ToString() {
  return m_message;
  //Wont be used since this is a pure virtual function.
}

string Cipher::FormatOutput() {
  return "default";
  //Filler code since this is a pure virtual function.
}




