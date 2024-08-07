// CMSC 341 - Fall 2020 - Project 1
#ifndef _CSRLIST_H_
#define _CSRLIST_H_
#include "csr.h"
class CSRList{
    public:
    CSRList();
    ~CSRList();
    CSRList(const CSRList & rhs);
    void insertAtHead(const CSR & matrix);
    const CSRList& operator=(const CSRList & rhs);
    bool operator==(const CSRList & rhs) const;
    int getAt(int CSRIndex, int row, int col) const;
    int averageSparseRatio();
    void clear();
    bool empty() const;
    void dump();//prints data for all nodes
  
    private:
    CSR* m_head;//the pointer to the head of the list
    int m_size;//the current number of nodes in the list
};
#endif
