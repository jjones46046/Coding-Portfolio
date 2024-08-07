/**************************************
 ** File: Vignere.cpp
 ** Project: CMSC 202 Project 4, Spring 2020
 ** Author: Jared Jones
 ** Date: 4/20/20
 ** Section: 11
 ** Email: jjones20@umbc.edu
 **
 ** This file contains the functions for Vigenere.h
 ** This function constructs the Vigenere cipher, defines its encryption and decryption,
 ** And returns the cipher type. It uses a private variable, m_key, to define its 
 ** encryption of different messages.
 *************************************/


#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "Cipher.h"
#include "Vigenere.h"
using namespace std;



Vigenere::Vigenere() {
  m_key = "test";
}


Vigenere::Vigenere(string message, bool isEncrypted, string key) {
  m_key = key;
  SetMessage(message);
  //Toggling encryption status from the default 0 to 1 if the message is encrypted.
  if (isEncrypted == 1) {
    ToggleEncrypted();
  }
}

Vigenere::~Vigenere() {
}

void Vigenere::Encrypt() {
  int len = m_key.size();
  int newletter = 0;
  string message = GetMessage();
  
  for (int i = 0; i < (int)message.size(); i++) {
    if ('A' <= message[i] and message[i] <= 'z') {       //Ensuring only letters are used.
      //Encrypting Uppercase letters.
      if ('A' <= message[i] and message[i] <= 'Z') {
	newletter = ((int)message[i] - 'A' + (int)toupper(m_key[i % len]) - 'A') % 26 + 'A';
	message[i] = (char)newletter;
      }

      //Encrypting Lowercase letters.
      else if (message[i] <= 'z' and message[i] >= 'a') {
	newletter = ((int)message[i] - 'a' + (int)tolower(m_key[i % len]) - 'a') % 26 + 'a';
	message[i] = (char)newletter;
      }
    }
  }
  SetMessage(message);
  ToggleEncrypted();
}


void Vigenere::Decrypt() {
  int kLen = m_key.size();
  int newletter = 0;
  string message = GetMessage();

  for (int i = 0; i < (int)message.size(); i++) {
    if ('A' <= message[i] and message[i] <= 'z') {       //Ensuring only letters are used.
      //Decrypting uppercase letters
      if(isupper(message[i])) {
	newletter = ((int)message[i] - (int)toupper(m_key[i % kLen]) + 26) % 26 + 'A';
	message[i] = (char)newletter;
      }
      
      //Decrypting lowercase letters
      if(islower(message[i])) {
	newletter = ((int)message[i] - (int)tolower(m_key[i % kLen]) + 26) % 26 + 'a';
	message[i] = (char)newletter;
      }
    }
  }
  SetMessage(message);
  ToggleEncrypted();
}


string Vigenere::ToString() {
  return "Vigenere";
}

string Vigenere::FormatOutput() {
  stringstream msg;
  char cipherType = 'v';
  msg << cipherType << "|" << GetIsEncrypted() << "|" << GetMessage() << "|" << m_key << endl;
  return msg.str();
}


