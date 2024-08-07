#include <iostream>
#include "hash.h"
#include "car.h"

unsigned cyclic_hash16(string key);
const char* FAIL = "*****TEST FAILED: ";
const char* PASS = "     TEST PASSED: ";

int main() {
  
  //Normal Operations
  
  //Inserting into empty HashTable:
  //Expected: Table isn't empty
  {
    HashTable<Car> ht(2, cyclic_hash16);
    ht.insert(Car("miuras", 28, "mega car"));

    if(!(ht.numEntries() == 0))
      cout << PASS << "insert(): empty list" << endl;
      
  }

  //Inserting into occupied HashTable:
  //Expected result: size = 2
    {
      HashTable<Car> ht(2, cyclic_hash16);
      ht.insert(Car("miuras", 28, "mega car"));
      ht.insert(Car("mimoc", 10, "rate car"));
      
      if(!(ht.numEntries() < 2))
	cout << PASS << "insert(): occupied list" << endl;

    }

    //Testing collison handling
    //Expected result: mimoc should chain.
    {
      HashTable<Car> ht(2, cyclic_hash16);
      ht.insert(Car("miuras", 28, "mega car"));
      ht.insert(Car("mimoc", 10, "rate car"));
      ht.insert(Car("mimor", 6, "face car"));
      //ht.dump();
    
      if(!(ht.numEntries() > 3))
	cout << PASS << "insert(): collision handling" << endl;
    }

    //Testing number of entries and size tracking.
    //Expected result: 3 entries, 4 entries, then 2 entries
    {
      cout << endl << "//**Testing number of entries and size tracking. **\\" << endl;
      cout << "//**Only inserting entries that will create new buckets to see if _n will update. **\\" << endl;
      cout << "//**Expected result: 3 entries, 4 entries, then 2 entries**\\" << endl;
      HashTable<Car> ht(5, cyclic_hash16);
      ht.insert(Car("miuras", 28, "mega car"));
      ht.insert(Car("nissan", 10, "rate car"));
      ht.insert(Car("mimor", 6, "face car"));
      cout << "numEntries() after three inserts: " << ht.numEntries() << endl;
      
      ht.insert(Car("x101", 4, "shack of cars"));
      cout << "numEntries() after four inserts: " << ht.numEntries() << endl;


      
      Car p;
      ht.getNext("miuras", p);
      cout << "numEntries() after one remove top: " << ht.numEntries() << endl;

      ht.dump();
    }

    //Testing assignment and copy constructors
    //Expected result: Copy dumps are accurate to the original,
    //Copy constuctor dump after insert is different to original. 
    {
      cout << endl << "//**Testing assignment and copy constructors**\\" << endl;
      cout << "//**Expected result: Copy dumps are accurate to the original,**\\" << endl;
      cout << "//**Copy constuctor dump after insert is different to original.**\\" << endl;

      
      HashTable<Car> ht(5, cyclic_hash16);
      ht.insert(Car("miuras", 28, "mega car"));
      ht.insert(Car("nissan", 10, "rate car"));
      ht.insert(Car("mimor", 6, "face car"));

      //Initializing copy.
      HashTable<Car> hashcopy(7, cyclic_hash16);
      hashcopy = ht;
      
      HashTable<Car> ht2(ht);
      cout << "**Copy constructor copy dump:**" << endl;
      ht2.dump();
      cout << "**Assignment Operator copy dump:**" << endl;
      hashcopy.dump();
      
      cout << "**Adding and removing from copy to test deep copying**" << endl;
      ht2.insert(Car("x101", 5, "super car"));
      Car p;
      ht.getNext("miuras", p);

      cout << "**Copy constructor copy dump after adding x101: **" << endl;
      ht2.dump();
      cout << "**Original dump after removal of miuras: **" << endl;
      ht.dump();
      cout << "**Assignment operator dump: (Should be the same as previous dump, this is a base case)." << endl;
      hashcopy.dump();
    }

    
    //Testing getNext functionality and sorting of remaining heap 
    {
      cout << endl << "//**Testing getNext functionality and sorting of remaining heap**\\" << endl;
      cout << "//**Expected Result: Ford with priority 10 removed and remaining heap sorted**\\" << endl;
      
      HashTable<Car> ht(5, cyclic_hash16);
      ht.insert(Car("Ram", 28, "Fast car"));
      ht.insert(Car("Brooklyn", 10, "Nas car"));
      ht.insert(Car("Ford", 6, "Case car"));
      ht.insert(Car("Ford", 10, "truck car"));
      ht.insert(Car("Ford", 4, "glass car"));

      Car p;
      ht.getNext("Ford", p);
      cout << "Car that was removed from the Ford bucket: " << endl;
      cout << p << endl;

      ht.dump();

    }

  return 0;
}


unsigned cyclic_hash16(string key) {
  unsigned usize = 16;
  unsigned s = 5; // shift by 5
  unsigned h = 0;
  for (unsigned i = 0; i < key.length(); i++ ){
    h = (( h << s ) | ( h >> (usize - s) ));
    h += key[i];
    h = h & 0xffff;
  }
  return h;
}


