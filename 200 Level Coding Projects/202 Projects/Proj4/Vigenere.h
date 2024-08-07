#ifndef VIGENERE_H
#define VIGENERE_H
#include "Cipher.h"
#include <istream>
#include <string>
using namespace std;

class Vigenere: public Cipher{
 public:
  // Name: Vigenere (Default Constructor)
  // Desc: Constructor to build an empty Vigenere Cipher (Defaults key to "test")
  // Preconditions - None
  // Postconditions - Creates a Vigenere cipher to be encrypted
  Vigenere();
  // Name: Vigenere (Overloaded Constructor)
  // Desc: Constructor to build a populated Vigenere Cipher
  // Preconditions - Pass it the message, isEncrypted, and key
  // Postconditions - Creates a Vigenere cipher to be encrypted
  Vigenere(string, bool, string);
  // Name: Vigenere (Destructor)
  // Desc: Destructor - Anything specific to delete in Vigenere?
  // Preconditions - ~Vigenere exists
  // Postconditions - Vigenere destroyed
  ~Vigenere();
  // Name: Encrypt
  // Desc: Encrypts using key (See project document for details)
  // Preconditions - Message exists
  // Postconditions - Encrypts using key
  void Encrypt();
  // Name: Decrypt
  // Desc: Decrypts using key (See project document for details)
  // Preconditions - Message exists
  // Postconditions - Uses key to decrypt.
  void Decrypt();
  // Name: ToString
  // Desc - A function that returns the string of the object type
  // Preconditions - The object exists
  // Postconditions - The subtype is returned (Vigenere in this case)
  string ToString();
  // Name: FormatOutput()
  // Desc - A function that returns the formatted output for Output function
  // Preconditions - The object exists (use stringstream)
  // Postconditions - v, delimiter, isencrypted, delimiter,
  //                  message, delimiter, key are returned for output
  string FormatOutput();
private:
  string m_key; //Key to encrypt or decrypt message
};


#endif
