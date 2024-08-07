// CMSC 341 - Fall 2020 - Project 1
#include "csrlist.h"

CSRList::CSRList(){
  m_head = NULL;  
}

CSRList::CSRList(const CSRList & rhs){
  m_head = NULL;
  CSR* temp = rhs.m_head;
  //  CSR* copy = temp;
  m_head = temp;
  /*  
  while(temp != nullptr) {
    copy = rhs.CSR(temp);
    
    
  }
  */
}

CSRList::~CSRList(){
  clear();
  m_head = NULL;
  m_size = 0;
}

bool CSRList::empty() const{
  if (m_head == NULL)
    return true;
  else
    return false;
}

void CSRList::insertAtHead(const CSR & matrix) {
  CSR * new_node = new CSR(matrix);
  m_size++;
  
  //Inserts the node at the beginning of the list.
  new_node->m_next = m_head;
  m_head = new_node;
}

void CSRList::clear(){
  CSR* curr = m_head;
  while (curr != NULL) {
    m_head = curr;
    curr = curr->m_next;
    delete m_head;
  }
  
} 
 

int CSRList::getAt(int CSRIndex, int row, int col) const{
  int counter = 0;
  CSR* temp = m_head;
  while(counter != CSRIndex and curr != NULL) { 
    curr = curr->m_next;
    counter++;
  }
  
  if (counter == CSRIndex)
    return curr->getAt(row,col);

  return 0;
}

void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}

bool CSRList::operator== (const CSRList & rhs) const{
  CSR* temp = m_head;
  CSR* temp2 = rhs.m_head;
  while (temp != nullptr) {
    if((temp == temp2) == false) {
      return false;
    }
    temp = temp->m_next;
    temp2 = temp2->m_next;
  }
    
    return true;
}

const CSRList& CSRList::operator=(const CSRList & rhs){
    /***********************************
     *      To be implemented
     * ********************************/
    return *this;
}

int CSRList::averageSparseRatio() {
  CSR* temp = m_head;
  double running_total = 0.0;
  
  while(temp != NULL) {
    running_total = running_total + temp->sparseRatio();
    temp = temp->m_next;
  }
  
  return running_total/m_size;
}
