// CMSC 341 - Fall 2020 - Project 4
// heap.h
// Templated, vector-based heap implementation

// To work with Heap, the template class T must provide the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) overloaded insertion operator
// To work with Heap *and* HashTable, it also needs:
//  (d) key function (string key())

#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <exception>
#include <utility>   // swap

using std::vector;
using std::endl;
using std::cout;
using std::swap;
using std::range_error;

// To work with Heap and HashTable, class T must provide the
// following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

class Grader;

template <class T>
class Heap {

  friend Grader;
  
 public:
  // Constructor
  Heap() {
    _used = false;
    T trashItem;
    _heap.push_back(trashItem);
  }

  // Inline functions: size, empty, used 
  unsigned size() const {
    return _heap.size() - 1;
  }
  
  bool empty() const {
    return _heap.size() <= 1;
  }
  bool used() const {
    return _used;
  }

  // Main heap operations: insert, read, remove
  void inserth(const T& object);
  T readTop() const;
  void removeTop();

  // Dump the entire heap
  void dump() const;
  
  // Root node always has index 1
  static const unsigned ROOT = 1;
  
private:
  vector<T> _heap;   // Vector-based heap
  bool _used;        // Has the heap ever been used?  Needed for
		     // linear probing.

  // *********************************************
  // Private helper function declarations go here!
  // *********************************************
  void upHeap(int anIndex);

  void downHeap(int anIndex);
};

// ***************************************
// Templated function definitions go here!
// ***************************************
template <typename T>
void Heap<T>::inserth(const T& object) {
  //Updating the value of used if this is the first time the heap is used.
  if(!_used) {
    _heap.push_back(object);
    _used = true;
  }

  else {
    _heap.push_back(object);
    upHeap(_heap.size() - 1);
  }
}

template <typename T>
void Heap<T>::upHeap(int anIndex) {
  if(anIndex == 1) {
    return;
  }

  int position = anIndex;

  //Terminate if the root node is reached, or a parent's key is less than our index's key.
  while(position != 1 and _heap[((position)/2)].priority() < _heap[position].priority()) {
    swap(_heap[position], _heap[((position)/2)]);
    position = ((position)/2);
  }
}


template <typename T>
T Heap<T>::readTop() const {
  //Preventing accessing the top of an empty heap.
  if(empty()) {
    throw range_error("Cannot access top of empty heap.");
  }
  
  return _heap[ROOT];
  
}

template <typename T>
void Heap<T>::removeTop() {
  if(empty()) {
    throw range_error("Cannot access top of empty heap.");
  }
  
  T temp;
  if((_heap.size() - 1) == 1) {
    _heap.pop_back();
}  
  else if(!empty()) {
    //Copying the last element to the top of the heap
    _heap[ROOT] = _heap[size()];
    _heap.pop_back();
    downHeap(ROOT);
  }
};

template <typename T>
void Heap<T>::downHeap(int anIndex){
  if(!empty()) {
    
    int position = anIndex;
    
    while(_heap[position*2].priority() > _heap[position].priority() or _heap[(position*2) + 1].priority() > _heap[position].priority()) {
      int rightChldKey = _heap[(position*2) + 1].priority();
      int leftChldKey = _heap[position*2].priority();
      
      //Left child is larger therefore it is swapped
      if(rightChldKey < leftChldKey) {
	swap(_heap[position], _heap[position*2]);
	position = (position*2) + 1;
      }
      //Swap with right child
      else {
	swap(_heap[position], _heap[(position*2) + 1]);
	position = (position*2);
      }
      
    }
  } //End of "if(!empty)"
}

template <typename T>
void Heap<T>::dump() const {
  for (unsigned i = ROOT;i < _heap.size(); i++) {
    cout << i << ": " << _heap[i] << ", ";
  }
  
};

#endif
