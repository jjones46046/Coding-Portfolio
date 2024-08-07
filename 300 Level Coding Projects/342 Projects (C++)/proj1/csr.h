// CMSC 341 - Fall 2020 - Project 1
#ifndef _CSR_H_
#define _CSR_H_
#include <iostream>
#include <stdexcept>
using std::runtime_error;
using namespace std;


class CSR{
    public:
    CSR();
    ~CSR();
    CSR(const CSR & rhs);
    void compress(int m, int n, int array[], int arraySize);
    int getAt(int row, int  col) const;
    int sparseRatio();
    bool operator==(const CSR & rhs) const;
    void clear();
    bool empty() const;
    void dump();//prints data for the node
  
    private:
    int* m_values;//array to store non-zero values
    int* m_col_index;//array to store column indices
    int* m_row_index;//array to store row indices 
    int m_nonzeros;//number of non-zero values
    int m_arraySize;//size of the total array
    int m_m;//number of rows
    int m_n;//number of columns 
    CSR* m_next;//pointer to the next CSR object in linked list
    friend class CSRList;//CSRList class has access to this class
};
#endif
