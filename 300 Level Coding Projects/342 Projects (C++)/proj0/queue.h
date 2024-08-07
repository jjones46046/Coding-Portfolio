// CMSC 341 - Fall 2020 - Project 0
// queue.h: a templated, linked list-based queue implementation

// Assignment:
//   (1) Complete the copy constructor
//   (2) Complete the assignment operator
//   (3) Complete the destructor
//   (4) Write a test program (mytest.cpp) to test copy and assignment
//   (5) Verify destructor by running the test program in Valgrind

#ifndef _QUEUE_H
#define _QUEUE_H

#include <iostream>
#include <exception>

using std::ostream;
using std::cout;
using std::endl;
using std::range_error;

// Forward declarations
template <class T> class Queue;
template <class T> class Node;
template <class T> ostream& operator<<(ostream&, const Node<T>&);

// Node class for linked list
template <class T>
class Node {

  friend Queue<T>;
  
public:
  Node(T data = T(), Node<T> *next = nullptr) {
    _data = data;
    _next = next;
  }

  friend ostream& operator<< <T>(ostream &sout, const Node<T> &x);
  
private:
  T _data;
  Node *_next;
};

// Overloaded insertion operator.  Must be overloaded for the template
// class T, or this won't work! For primitive types in C++ such as string 
// or int this is alredy overloaded
template <class T>
ostream& operator<<(ostream &sout, const Node<T> &x) {
  sout << "Data: " << x._data;
  return sout;
}


// Queue class.  Linked-list implementation of a queue. Uses the Node
// class.
template <class T>
class Queue {
public:
  // Constructor
  Queue();

  // Copy constructor, assignment operator, and destructor
  // DO NOT IMPLEMENT HERE.  SEE BELOW.
  Queue(const Queue &rhs);
  const Queue& operator=(const Queue& rhs);
  ~Queue();

  // Queue operations: enqueue(), head(), and dequeue()
  void enqueue(const T& data);//inserts at the end
  const T& head() const;      //returns the head value
  void dequeue();             //removes from the head           

  // Helpful functions
  bool empty() const;  // Returns 'true' if queue is empty
  void dump() const;   // Dump contents of the linked list

private:
  Node<T> *_head;

  void clear(); //Empties the queue and resets back to default
  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************
  
};

template <class T>
Queue<T>::Queue() {
  _head = nullptr;
}

template <class T>
Queue<T>::Queue(const Queue<T>& rhs) {
  if(rhs.empty()) {
    clear();
    throw range_error("Queue<T>::operator=: Attempt to copy an empty queue");
  }
  
  Node<T> * temp = rhs._head;
  Node<T> * copier = new Node<T>();
  _head = copier;
  
  while(temp != NULL) {
      copier->_data = temp->_data;
      if(temp->_next != NULL)
	copier->_next = new Node<T>();
      temp = temp->_next;
      copier = copier->_next;
  }
  
}

template <class T>
const Queue<T>& Queue<T>::operator=(const Queue<T>& rhs) {
  if(rhs.empty()) {
    clear();
    throw range_error("Queue<T>::operator=: Attempt to copy an empty queue");
  }
  
  if(this != &rhs) { //Protecting against self assignment
    
    if(!empty()) {
      clear();
    }

    Node<T> * temp = rhs._head;
    Node<T> * copier = new Node<T>();
    _head = copier;
    
  while(temp != NULL) {
    copier->_data = temp->_data;
    if(temp->_next != NULL)
      copier->_next = new Node<T>();
    temp = temp->_next;
    copier = copier->_next;
  }
  delete temp;
  return *this;
  }
}

template <class T>
Queue<T>::~Queue() {
  if(!empty()) {
    clear();
  }
}

template <class T>
void Queue<T>::clear() {
  Node<T> * temp = _head;
  Node<T> * next;
  
  while(temp->_next != NULL) {
    next = temp->_next;
    delete temp;
    temp = next;
  }
  
  delete next;
  _head = NULL;
}

template <class T>
void Queue<T>::enqueue(const T& data) {
  Node<T> *newPtr = new Node<T>(data);
  Node<T> *tempPtr;
  if (_head == nullptr){
    _head = newPtr;
  }
  else{
    tempPtr = _head;
    while ( tempPtr->_next != nullptr ) {
        tempPtr = tempPtr->_next;
    }
    tempPtr->_next = newPtr;
  }
}

template <class T>
const T& Queue<T>::head() const {
  if ( empty() ) {
    throw range_error("Queue<T>::head(): attempt to read empty Queue.");
  }

  return _head->_data;
}

template <class T>
void Queue<T>::dequeue() {
  if ( empty() ) {
    throw range_error("Queue<T>::dequeue(): attempt to dequeue an empty Queue.");
  }

  Node<T> *tmpPtr = _head->_next;
  delete _head;
  _head = tmpPtr;
}

template <class T>
bool Queue<T>::empty() const {
  return _head == nullptr;
}

template <class T>
void Queue<T>::dump() const {
  Node<T> *nodePtr = _head;
  while ( nodePtr != nullptr ) {
    cout << nodePtr->_data << endl;
    nodePtr = nodePtr->_next;
  }
}

#endif
