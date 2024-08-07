/**************************************
 ** File: LL.cpp
 ** Project: CMSC 202 Project 4, Spring 2020
 ** Author: Jared Jones
 ** Date: 5/1/20
 ** Section: 11
 ** Email: jjones20@umbc.edu
 **
 ** This file contains the functions for LL.cpp.
 ** This file contains two classes, Node and LL. Node is a node used in linked list and 
 ** its data consist of a templated pair of data. LL is a dynamically allocated list
 ** that is sorted alphabettically.
 **
 *************************************/
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

// Templated Node class
// Normal node except that each node can hold a pair <T,int>
// where the first value is templated and the second value is the frequency
// (or quantity) of the first
// For example, if T was a string then it would hold string,int
template <class T>
class Node {
public:
  Node( const T& data ); //Node Constructor - Assume each quan
  pair<T,int>& GetData(); //Node Data Getter
  void SetData( const pair<T,int>& ); //Node Data Setter
  Node<T>* GetNext(); //Node m_next getter
  void SetNext( Node<T>* next ); //Node m_next setter
private:
  pair <T,int> m_data; //Holds templated data
  Node<T>* m_next; //Pointer to next node
};

template <class T>
Node<T>::Node( const T& data ) {
  m_data = make_pair(data,1);
  m_next = nullptr;
}

template <class T>
pair<T,int>& Node<T>::GetData() {
   return m_data;
}

template <class T>
void Node<T>::SetData( const pair<T,int>& data ) {
   m_data = data;
}

template <class T>
Node<T>* Node<T>::GetNext() {
   return m_next;
}

template <class T>
void Node<T>::SetNext( Node<T>* next ) {
   m_next = next;
}

//Templated Linked List class
template <class T>
class LL {
 public:
  // Name: LL() (Linked List) - Default Constructor
  // Desc: Used to build a new linked list
  // Preconditions: None
  // Postconditions: Creates a new linked list where m_head points to nullptr
  LL();
  // Name: ~LL() - Destructor
  // Desc: Used to destruct a LL
  // Preconditions: There is an existing LL with at least one node
  // Postconditions: A LL is deallocated (including dynamically allocated nodes)
  //                 to have no memory leaks!
 ~LL();
  // Name: LL (Copy Constructor)
  // Desc: Creates a copy of existing LL
  //       Requires a LL - REQUIRED to be implemented even if not used
  // Preconditions: Source LL exists
  // Postconditions: Copy of source LL
  LL(const LL&);
  // Name: operator= (Overloaded Assignment Operator)
  // Desc: Makes two LL of the same type have identical number of nodes and values
  // Preconditions: Requires two linked lists of the same templated type
  //                REQUIRED to be implemented even if not used
  // Postconditions: Two idenetical LL
  LL<T>& operator= (const LL&);
  // Name: Find
  // Desc: Iterates through LL and returns node if data found
  // Preconditions: LL Populated
  // Postconditions: Returns nullptr if not found OR Node pointer if found
  Node<T>* Find(const T& data);
  // Name: Insert
  // Desc: Either inserts a node to the linked list OR increments frequency of first
  //       Takes in data. If "first" (of pair) NOT in list, adds node
  //       If "first" (of pair) is already in list, increments quantity
  //       Inserts "first" in order with no duplicates
  // Preconditions: Requires a LL.
  // Postconditions: Node inserted in LL based on first value (or quantity incremented)
  void Insert(const T&);
  // Name: RemoveAt
  // Desc: Removes a node at a particular position based on data passed (matches first)
  // Preconditions: LL with at least one node. 
  // Postconditions: Removes first node with passed value (in first)
  void RemoveAt(const T&);
  // Name: Display
  // Desc: Display all nodes in linked list
  // Preconditions: Outputs the LL
  // Postconditions: Displays the pair in each node of LL 
  void Display();
  // Name: GetSize
  // Desc: Returns the size of the LL
  // Preconditions: Requires a LL
  // Postconditions: Returns m_size
  int GetSize();
  // Name: operator<< (Overloaded << operator)
  // Desc: Returns the ostream of the data in each node
  // Preconditions: Requires a LL
  // Postconditions: Returns an ostream with the data from each node on different line
  template <class U>
  friend ostream& operator<<(ostream& output, const LL<U>&);
  // Name: Overloaded [] operator
  // Desc: When passed an integer, returns the data at that location
  // Precondition: Existing LL
  // Postcondition: Returns pair from LL using []
  pair<T,int>& operator[] (int x);//Overloaded [] operator to pull data from LL
private:
  Node <T> *m_head; //Node pointer for the head
  int m_size; //Number of nodes in queue
};
//*****************************************************************
//Implement LL here

//Constructor
template <class T>
LL<T>::LL() {
  m_head = nullptr;
  m_size = 0;
}
//Destructor
template <class T>
LL<T>::~LL() {
  Node<T>* curr = m_head;  
  while(curr != nullptr) {
    m_head = curr;
    curr = curr->GetNext();
    delete m_head;
  }

  m_head = nullptr;
  m_size = 0;
}

template <class T>
LL<T>::LL(const LL& list) { //Copy constructor
  Node<T>* temp = list.m_head;
  
  while(temp != nullptr) {
    int quantity = temp->GetData().second;
    for (int i = 0; i < quantity; i++) //Runs insert multiple times based on quantity
    Insert(temp->GetData().first);
    
    temp = temp->GetNext();
  }
}


template <class T>
LL<T>& LL<T>::operator= (const LL& list) {
  Node<T>* temp = list.m_head;
  Node<T>* curr = m_head;

  while(curr != nullptr) {
    m_head = curr;
    curr = curr->GetNext();
    delete m_head;
  }

  m_head = nullptr;
  m_size = 0;


  LL<T> copy = LL<T>();
  while(temp != nullptr) {
    int quantity = temp->GetData().second;
    cout << quantity << endl;
    cout << temp->GetData().first << endl;
    for (int i = 0; i < quantity; i++) //Runs insert multiple times based on quantity
      copy.Insert(temp->GetData().first);
    
    temp = temp->GetNext();
  }
  return copy;
}


template <class T>
Node<T>* LL<T>::Find(const T& data) {
  Node<T>* curr = m_head;
  
  if(m_size == 0) {   //For cases with empty list
    return nullptr;
  }

  else {
    //Iterating through the LL
    while(curr->GetData().first != data and curr->GetNext() != nullptr) {
      curr = curr->GetNext();
    }
    //Indicates a word that was found
    if (curr->GetData().first == data) {
      return curr;
    }
    //Indicated a word that wasn't found
    else if (curr->GetNext() == nullptr) {
      return nullptr;
    }
  }
}


template <class T>
void LL<T>::Insert(const T& data) {
  bool stop = false;
  if((Find(data)) != nullptr) { //If that key already exists
    //cout << "found worked" << endl;
    
    Node<T>* curr = m_head;
    while(curr->GetData().first != data) {
      curr = curr->GetNext();
    }
    pair<T,int> newdata = curr->GetData(); //Incrementing quantity
    newdata.second += 1;
    curr->SetData(newdata);
  }
  
  else { //If the key doesn't exist already
    
    if(m_size == 0) {  //Special case for empty list
      m_head = new Node<T>(data);
      m_size++;
    }

    else if (m_head->GetData().first > data) {
      Node<T>* newnode = new Node<T>(data);
      // Node<T>* temp = m_head;
      newnode->SetNext(m_head);
      m_head = newnode;
      m_size++;
    }
    
    else { //Adding a new node
      Node<T>* curr = m_head;
      //Iterating through the list.
      while(curr->GetNext() != nullptr and stop != true) {
	if(curr->GetData().first > data) { //stopping the iteration at the node before.
	  stop = true;
	}
	else
	  curr = curr->GetNext();
      }
      //Adding node to end of the list.
      if (curr->GetNext() == nullptr) {
	//Creating a new node that will be added to the list.
	Node<T>* newnode = new Node<T>(data);
	curr->SetNext(newnode);
	m_size++;
      }
      //Adding node to middle.
      else if (stop == true) { //Inserting the word we brought in
	Node<T>* newnode = new Node<T>(data);
	newnode->SetNext(curr->GetNext());
	curr->SetNext(newnode);
	m_size++;
      }
      
    }
  }
}

template <class T>
void LL<T>::RemoveAt(const T& data) {
  Node<T>* curr = m_head;
  Node<T>* prev;
  
  if(m_head->GetData().first == data) { //Removing the head
    m_head = m_head->GetNext();
    m_size--;
  }
  
  else {
    
    while(curr != nullptr and data != curr->GetData().first) {
      //Iterating through the list while keeping track of the previous node
      prev = curr;
      curr = curr->GetNext();
    }
    
    if (data == curr->GetData().first) {
      //Removing the node by setting previous's next to the next next.
      prev->SetNext(curr->GetNext());
      delete curr;
      curr = nullptr;
      m_size--;
    }
  }
  
  
}
  

template <class T>
void LL<T>::Display() {
  Node<T>* curr = m_head;  
  while(curr != nullptr) {
    cout << "There is:" << curr->GetData().second << " of " << curr->GetData().first << endl;
    curr = curr->GetNext();
  }
  cout << "***END OF LIST***" << endl;
}

template <class T>
int LL<T>::GetSize() {
  return m_size;
}


template <class T>
pair<T,int>& LL<T>::operator[] (int x) {
  //Has a segfault error
  cout << "Here1" << endl;
  int count = 0;
  Node<T>* curr = m_head;
  string word;
  int num;
  //Iterating until the index we want is found
  while(count != x) {
    curr = curr->GetNext();
    count++;
  }

  if (count == x) {
    word = curr->GetData().first;
    num = curr->GetData().second;
    //Creating a new pair to export
    pair <T,int> output = make_pair(word,num);
  
  return output;
  }
}

template <class U>
ostream& operator<< (ostream& output, const LL<U>& list) {
  Node<U>* curr = list.m_head;
  while (curr != nullptr) {
    //Defining ostream object 'output'
    output << curr->GetData().first << ":" << curr->GetData().second << endl;
    curr = curr->GetNext();
  }
  
  return output;
}

//****************************************************************
// To test just LL follow these instructions:
//   1.  Uncomment out int main below
//   2.  make LL
//   3.  ./LL (try valgrind too!)

/*
int main () {
  //Test 1 - Default Constructor and Push
  cout << "Test 1 - Default Constructor and Push Running" << endl;
  //Test Default Constructor
  LL <string>* newLL1 = new LL<string>();
  //Push 4 nodes into LL
  newLL1->Insert("candy");
  newLL1->Insert("cookies");
  newLL1->Insert("candy");
  newLL1->Insert("bananas");
  newLL1->Insert("dogs");
  newLL1->Insert("apples");
  newLL1->Insert("elephants");
  newLL1->Insert("barf");
  newLL1->Insert("candy");
  newLL1->Insert("cookies");
  newLL1->Insert("candy");
  newLL1->Insert("bananas");
  newLL1->Insert("dogs");
  newLL1->Insert("apples");
  newLL1->Insert("elephants");
  newLL1->Insert("barf");
  newLL1->Display();

  //  delete newLL1;

 
  //Test 2 - Copy Constructor and Assignment Operator
  cout << "Test 2 - Copy Constructor and Assignment Operator Running" << endl;
  //Test Copy constructor
  LL <string>* newLL2 = new LL<string>(*newLL1);
  cout << "*******Original*********" << endl;
  newLL1->Display();
  cout << "*******Copy*********" << endl;
  newLL2->Display();
 
  //Test Overloaded Assignment Operator
  LL <string>* newLL3 = new LL<string>();
  *newLL3 = *newLL1;
  cout << "*******Assignment*********" << endl;
  newLL3->Display();
  cout << endl;
  //Test 3 - Test Display and Overloaded <<
  cout << "Test 3 - Display and Overloaded << Running" << endl;
  cout << "newLL1 Display Function" << endl;
  newLL1->Display();
  cout << "newLL1 Overloaded" << endl;
  cout << *newLL1;

  cout << "RemoveAt(candy)" << endl;
  newLL1->RemoveAt("candy");
  cout << "newLL1 Display Function" << endl;
  newLL1->Display();
  cout << newLL1[1] << endl;
  cout << (*newLL1)[2].first << endl;
  return 0;
}
*/
