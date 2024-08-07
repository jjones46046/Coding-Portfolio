#include "bookTree.h"
using namespace std;

int main(){
  auto wt = WordTree();
  string element;
  cout << "Enter a new element: ";
  cin >> element;
  while (element != "STOP")
    {
      wt.insert(element);
      wt.dump();
      cout << endl << "Enter a new element: ";
      cin >> element;
      }

  
  return 0;
}

