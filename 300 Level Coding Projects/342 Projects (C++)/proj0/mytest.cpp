#include "queue.h"
using namespace std;


int main() {  
  //Testing assignment and copy constructors
  //Expected result: Copy dumps are accurate to the original,
  //Copy constuctor dump after insert is different to original.
  {
    cout << endl << "//**Testing assignment and copy constructors**\\" << endl;
    cout << "//**Expected result: Copy dumps are accurate to the original,**\\" << endl;
    cout << "//**Copy constuctor dump after insert is different to original.**\\" << endl;

    Queue<int> intQueue;

    cout << "\nPush integers on queue and dump contents:\n";
    for (int i = 1; i <= 5; i++) {
      intQueue.enqueue(i);
    }
    
    //Initializing assingment operator copy.
    Queue<int> copy1;
    copy1 = intQueue;

    Queue<int> copy2(intQueue);

    cout << "**Original queue:**" << endl;
    intQueue.dump();
    cout << endl << endl;
    
    cout << "**Copy constructor copy dump:**" << endl;
    copy2.dump();
    cout << endl << endl;
    
    cout << "**Assignment operator copy dump:**" << endl;
    copy1.dump();
    cout << endl << endl;
    
    cout << "**Adding '7' to copy constructor copy to test deep copying using enqueue**" << endl;
    copy2.enqueue(7);
    
    cout << "**Copy constructor copy dump after adding 7**" << endl;
    copy2.dump();
    cout << endl << endl;
    
    cout << "**Original queue after dequeue**" << endl;
    intQueue.dequeue();
    intQueue.dump();
    cout << endl << endl;
    
    cout << "**Assignment operator dump: (Should be the same as the original queue before removal, this is a base case)." << endl;
    copy1.dump();
  }

  {
    Queue<int> intQueue;

    cout << "\nPush integers on queue and dump contents:\n";
    for (int i = 1; i <= 5; i++) {
      intQueue.enqueue(i);
    }

    //Initializing assingment operator copy.
    Queue<int> copy1;
    copy1 = intQueue;
    
    Queue<int> copy2;
    copy2 = copy1;

    cout << "**Testing for self assignment, shouldn't copy anything**" << endl;
    intQueue = copy2;
    intQueue.dump();
  }

  
  {
    cout << "**Testing copying empty queue**" << endl;
    cout << "**Should throw an error after attempting to copy an empty queue**" << endl;
    
    Queue<int> intQueue;
    //Initializing assingment operator copy.
    Queue<int> copy1;

    copy1 = intQueue; //Should cause an error to be thrown 
  }
 
  return 0;
}
