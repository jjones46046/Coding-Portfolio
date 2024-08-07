/**************************************
 ** File: Ong.cpp
 ** Project: CMSC 202 Project 4, Spring 2020
 ** Author: Jared Jones
 ** Date: 4/20/20
 ** Section: 11
 ** Email: jjones20@umbc.edu
 **
 ** This file contains the functions for Ong.h
 ** This function constructs the Ong cipher, defines its encryption and decryption,
 ** And returns the cipher type. It also allows for checking of vowels.
 **
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
#include "Ong.h"
using namespace std;


Ong::Ong() {
}

Ong::Ong(string message, bool isEncrypted) {
  SetMessage(message);
  //Toggling isEncrypted to true based on what is brought in.
  if (isEncrypted == 1) {
    ToggleEncrypted();
  }
}

Ong::~Ong() {
}

bool Ong::IsVowel(char letter) {
  char vowels[17] = {'a','e','i','o','u','A','E','I','O','U',';',':','.','(',')','"'};
  bool vowel = false;

  for (int i = 0;i < (int)sizeof(vowels); i++) {
    //Returns true if the letter is within the array "vowels"
    if (letter == vowels[i]) {
      vowel = true;
    }
  }
  
  return vowel;
}

void Ong::Encrypt() {
  string message = GetMessage();
  string newMessage = " ";
  bool isSpace;

  for (int i = 0; i < (int)message.size(); i++) {
    //Making sure spaces aren't given a hyphen or 'ong'
    isSpace = false;
    if(isspace(message[i])) {
      isSpace = true;
      newMessage += " ";
    }
    
    //For cases where i is a character we skip.
    if(IsVowel(message[i]) == true and isSpace == false) {
      string newLetter(1, message[i]);
      //Adds only the vowel if the the vowel is the end of a word.
      if(isspace(message[i + 1]))
	newMessage += newLetter; 
      
      else {
	newLetter = newLetter + "-";
	newMessage += newLetter;
      }
    }
    
    //Adds "ong-" to the remaining characters.
    else if(isSpace == false) {
      string newLetter(1, message[i]);
      if(isspace(message[i + 1])) {  //For characters at the end of a word.
	newLetter += "ong";
	newMessage += newLetter;
      }

      else {
      newLetter = newLetter + "ong-";
      newMessage += newLetter;
      }
    }
  }
  ToggleEncrypted();
  SetMessage(newMessage);
}


void Ong::Decrypt() {
  string message = GetMessage();
  string newMessage;
  bool firstval = false;
  bool isSpace = false;
  
  for (int i = 0; i < (int)message.size(); i++) {
    isSpace = false;
    //Identifying spaces in the message.
    if(isspace(message[i])) {
      isSpace = true;
      newMessage += " ";
    }
    
    //Special case for the very first letter.
    if(message[i] == message[0]) {
      string newLetter(1, message[i]);
      newMessage += newLetter;
    }

    //Runs the if statement body if the letter isnt the first leetter or a space
    //and is following a '-'.
    else if((message[i - 1] == '-' or isspace(message[i - 1])) and firstval != true and isSpace != true) {
      string newLetter(1, message[i]);
      newMessage += newLetter;
    }
    
  }
  //Toggling encryption value
  ToggleEncrypted();
  SetMessage(newMessage);
}

string Ong::ToString() {
  return "Ong";
}

string Ong::FormatOutput() {
  stringstream msg;
  char cipherType = 'o';
  msg << cipherType << "|" << GetIsEncrypted() << "|" << GetMessage() << "|" << endl;
  return msg.str();
  
}
