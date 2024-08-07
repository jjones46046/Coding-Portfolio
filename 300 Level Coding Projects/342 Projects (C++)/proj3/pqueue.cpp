// CMSC 341 - Fall 2020 - Project 3
// PQueue: an ER triage queue using a skew heap and function pointers

#include "pqueue.h"
#include <stack>

using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

PQueue::PQueue(prifn_t priFn) {
  priority = priFn;
  _size = 0;
  _heap = nullptr;
}

PQueue::~PQueue() {
  clear();
  _size = 0;
  _heap = NULL;
}

PQueue::PQueue(const PQueue& rhs) { //Reuse the preorder algorithim and insert
  clear();
  _size = rhs._size;
  _heap = rhs._heap;
  preOrderCopy(rhs._heap, _heap);
}

void PQueue::preOrderCopy(Node* rhsroot, Node* node) {
  if (rhsroot == NULL) {
    return;
  }

  else {
    node = rhsroot;
    preOrderCopy(rhsroot->_left, node->_left);
    preOrderCopy(rhsroot->_right, node->_right);
  }
  
}


PQueue& PQueue::operator=(const PQueue& rhs) {
  //clear();

  _heap = preOrderOp(rhs._heap, _heap);
  _size = rhs._size;
  return *this;
}

Node* PQueue::preOrderOp(Node* rhsroot, Node* node) {
  if (rhsroot == NULL) {
    return nullptr;
  }

  else {
    //Creating a node new node with patient data from rhs, then replacing null node with it.
    Node* temp = new Node(rhsroot->_patient);
    node = NULL;
    node = temp;
    node->_left = preOrderOp(rhsroot->_left, node->_left);
    node->_right = preOrderOp(rhsroot->_right, node->_right);
  }
  return node;
}

void PQueue::insertPatient(const Patient& input) {  
  //Creating the only node from our patient input
  Node* h2heap = new Node(input);
  
  if(_size == 0) {
    cout << "Adding first node.." << endl;
    _heap = h2heap;
    _size++;
  }
  
  else {
    //Building a new, one-node queue to be merged with our existing one.
    PQueue h2 = PQueue(getPriorityFn());
    h2._size = 1;
    h2._heap = h2heap;
    
    mergeWithQueue(h2);
  }
}


Patient PQueue::getNextPatient() {
  if(_size == 0) 
    throw domain_error("The queue is empty.");
  
  else {
    _size--;
     Patient priorityPatient = _heap->_patient;
     PQueue temp = PQueue(getPriorityFn());

     //Case where left child's heap has more priority than right child's
     //Right child merges into left child while the original heap is deleted (Treated)
     if (priority(_heap->_right->_patient) > priority(_heap->_left->_patient)) {
       temp._heap = _heap->_right;
       delete _heap;
       _heap = _heap->_left;
       mergeWithQueue(temp);
     }

     //Case where right child's heap has more priority than left child's.
     else {
       temp._heap = _heap->_left;
       delete _heap;
       _heap = _heap->_right;
       mergeWithQueue(temp);
     }
       
     return priorityPatient;
  }
}



void PQueue::mergeWithQueue(PQueue& rhs) {
  if(rhs.getPriorityFn() != priority)
    throw domain_error("Cannot merge queues with diffent priority functions.");

  else if(rhs._heap == _heap and rhs._heap->_right == _heap->_right) {
    throw domain_error("Cannot merge identical queues.");
  }
  
  _size = _size + rhs._size;
  _heap = mergeRecursive(rhs._heap, _heap);
  
}

Node* PQueue::mergeRecursive(Node* rhsroot, Node* root) {
  if(root == NULL)
    return rhsroot;
  
  else if(rhsroot == NULL)
    return root;

  else {
    //Swapping the main Heap node for the right side heap node if rhs has priority
    if(priority(rhsroot->_patient) < priority(root->_patient)) {
      swap(rhsroot, root);
    }

    //Swapping left and right child to prevent naive merging
    swap(root->_left, root->_right);
    root->_left = mergeRecursive(rhsroot, root->_left);
    
    return root;
  }
}


void PQueue::clear() {
  _heap = NULL;
  /*
  if(_heap != nullptr)
    clearRecursive(_heap);
  
  else if(_heap == NULL)
    delete _heap;
  */
}

void PQueue::clearRecursive(Node* node) {
  //**Doesn't Work**
  if(node->_left != NULL)
    clearRecursive(node->_left);
  if(node->_right != NULL)
    clearRecursive(node->_right);
  cout << "Here" << endl;
  delete node;
  node = NULL;

  //**Alternative algorithm
  /*
  if (node == NULL) {
    return;
  }
  
    cout << "clearRec(left)" << endl;
    clearRecursive(node->_left);
    cout << "clearRec(right)" << endl;
    clearRecursive(node->_right);
   
    //cout << "deleting.. ";
  // cout << node->_patient.getPatient() << endl;
  
  if(node != _heap) {
    cout << "deleting.. ";
    cout << node->_patient.getPatient() << endl;
    delete node;
  }
  
  else {
    _heap = NULL;
  }
  */
}


int PQueue::numPatients() const {
  return _size;  
}


void PQueue::printPatientQueue() const {
  //Throwing an error to prevent printing empty queues
  if(_size == 0)
    throw domain_error("Empty queue cannot be printed.");

  else {
    printPreOrder(_heap);
  }
}

void PQueue::printPreOrder(Node* node) const {
  if (node == NULL) {
    return;
  }
  //Printing the skew heap using PreOrder Traversal
  else {
    cout << node->_patient << endl;
    dumpInOrder(node->_left);
    dumpInOrder(node->_right);
  }
}

prifn_t PQueue::getPriorityFn() const {
  return priority;
}

void PQueue::setPriorityFn(prifn_t priFn) {
  priority = priFn;
  PQueue* h2 = new PQueue(priFn);
  //Creating a copy h2
  rebuild(_heap, h2);
  clear();
  //Using the rebuild algorithm to re-insert nodes into the main node from the copy
  rebuild(h2->_heap);
}

void PQueue::rebuild(Node* node) {
  if(node == NULL) {
    return;
  }
  else {
    insertPatient(node->_patient);
    rebuild(node->_left);
    rebuild(node->_right);
  }
  return;
}


void PQueue::rebuild(Node* node, PQueue* copy) {
  if(node == NULL) {
    return;
  }
  else {
    copy->insertPatient(node->_patient);
    rebuild(node->_left);
    rebuild(node->_right);
  }
  return;
}


void PQueue::dump() const //Same as print?
{
  //Throwing an error if an empty queue is attempted to be dumped.
  if(_size == 0) {
    throw domain_error("Empty queue cannot be printed.");
  }
  
  else {
     dumpInOrder(_heap);
  }
}

void PQueue::dumpInOrder(Node* root) const {
  if (root == NULL) {
    return;
  }
  
  //Dumping the skew heap using Inorder traversal, Left Process Right
  else {
    dumpInOrder(root->_left);
    cout << root->_patient << endl;
    dumpInOrder(root->_right);
  }
  
}


ostream& operator<<(ostream& sout, const Patient& patient) {
  sout << "Patient: " << patient.getPatient() << ", triage: " << patient.getTriage()
       << ", temperature: " << patient.getTemperature() << ", oxygen: " << patient.getOxygen() << ", RR: "
       << patient.getRR() << ", HR: " << patient.getHR() << ", BP: " << patient.getBP();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
