/**************************************
 ** File: Caesar.cpp
 ** Project: CMSC 202 Project 4, Spring 2020
 ** Author: Jared Jones
 ** Date: 4/20/20
 ** Section: 11
 ** Email: jjones20@umbc.edu
 **
 ** This file contains the functions for caesar.h
 ** This function constructs the caesar cipher, defines its encryption and decryption,
 ** And returns the cipher type
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
#include "Caesar.h"
using namespace std;

Caesar::Caesar() {
  m_shift = 3;
}

Caesar::Caesar(string message, bool isEncrypted, int shift) {
  m_shift = shift;
  SetMessage(message);
  
  //Changes encryption status to true from the default false if the message is encrypted.
  if (isEncrypted == 1) {
    ToggleEncrypted();
  }
}

Caesar::~Caesar() {
}

void Caesar::Encrypt() {
  string message = GetMessage();
  for (int i = 0; i < message.size(); i++) {
    //Gets only letters
    if ('A' <= message[i] and message[i] <= 'z') {
      int newletter = message[i];
      
      //Encrypting the Uppercase letters.
      if (newletter <= 'Z' and newletter >= 'A') {
	newletter = newletter + m_shift;
	if (newletter > 'Z') {
	  //Subtracting multiple times for cases with a large shift value
	  while (newletter > 'Z') {
	    newletter -= 'A';
	  }
	  newletter = (newletter % 26) + 'A';
	}
	message[i] = newletter; newletter = 0;
      }
      
      //Encrypting the lowercase letters.
      if (newletter <= 'z' and newletter >= 'a') {
	newletter = newletter + m_shift;
	if (newletter > 'z') {
	  while (newletter > 'z') {
	    newletter -= 'a';
	  }
	  newletter = (newletter % 26) + 'a';
	}
	//Changing the current character in message to the encrypted letter.
	message[i] = newletter; newletter = 0;
      }
    }
  }
  ToggleEncrypted();
  SetMessage(message);
}


void Caesar::Decrypt() {
  string message = GetMessage();
  for (int i = 0; i < message.size(); i++) {
    //Gets only letters
    if ('A' <= message[i] and message[i] <= 'z') {
      int newletter = message[i];
      //Decrypting the Uppercase letters.
      if (newletter <= 'Z' and newletter >= 'A') {
	newletter = newletter - m_shift;
	if (newletter < 'A') {
	  //Subtracting multiple times for cases with a large shift value
	  while (newletter < 'A') {
	    newletter += 'A';
	  }
	  newletter = (newletter % 26) + 'A';
	}
	message[i] = newletter;
      }

      //Decrypting the lowercase letters.
      if (newletter <= 'z' and newletter >= 'a') {
	newletter = newletter - m_shift;
	if (newletter < 'a') {
	  while (newletter < 'a') {
	    newletter += 'h';
	  }
	  newletter = (newletter % 26) + 'h';
	}
	//Changing the current character in message to the encrypted letter.
	message[i] = newletter;
      }
    }
  }
  ToggleEncrypted();
  SetMessage(message);
}


string Caesar::ToString() {
  return "Caesar";
}

string Caesar::FormatOutput() {
  stringstream msg;
  char cipherType = 'c';
  msg << cipherType << "|" << GetIsEncrypted() << "|" << GetMessage() << "|" << m_shift << endl;
  return msg.str();
}


