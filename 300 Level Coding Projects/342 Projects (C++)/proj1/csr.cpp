// CMSC 341 - Fall 2020 - Project 1
#include "csr.h"
#include <stdexcept>

CSR::CSR(){
  m_values = nullptr;
  m_col_index = nullptr;
  m_row_index = nullptr;
  m_nonzeros = 0;
}

CSR::~CSR(){
  m_next = NULL;
  clear();
}

CSR::CSR(const CSR & rhs){
  m_values = new int[rhs.m_nonzeros];   
  m_col_index = new int[rhs.m_nonzeros];
  m_row_index = new int[rhs.m_m + 1];
  
  for (int i = 0; i<rhs.m_nonzeros;i++) {
    m_values[i] = rhs.m_values[i];
    m_col_index[i] = rhs.m_col_index[i];
  }

  for (int i = 0; i < rhs.m_m+1;i++) {
    m_row_index[i] = rhs.m_row_index[i];
  }

  m_nonzeros = rhs.m_nonzeros;
  m_m = rhs.m_m;
  m_n = rhs.m_n;
  m_next = rhs.m_next;
}

void CSR::dump(){
    cout << endl;
    if (!empty()){
      cout << "V = ";
        for (int i=0;i<m_nonzeros;i++)
	  cout << m_values[i] << " ";
        cout << endl;

	cout << "COL_INDEX = ";
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
	
	cout << "ROW_INDEX = ";
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

void CSR::clear(){
  for (int i = 0; i < m_nonzeros;i++) {
    m_values[i] = 0;
    m_col_index[i] = 0;
  }
  
  for (int i = 0; i < m_m; i++) {
    m_row_index[i] = 0;
  }

  m_nonzeros = 0;
  m_m = 0;
  m_n = 0;
  delete[] m_values;
  delete[] m_col_index;
  delete[] m_row_index;
}

bool CSR::empty() const{
  if (m_values[0] == 0) //Checking the first index of m_values, which must be nonzero otherwise the CSR is empty. 
    return true;
  else
    return false;
}

void CSR::compress(int m, int n, int array[], int arraySize){
  //Note: m = rows, n = columns
  m_m = m;
  m_n = n;
  m_arraySize = arraySize;
  int counter = 0;  //Indicates the column number of each index. Resets after each new row.
  int numnonzeros = 0;
  int rownum = 1;
  
  for (int i = 0;i < arraySize;i++) {
    if (array[i] == 0)
      numnonzeros++;
  }
  
  m_values = new int[numnonzeros];   //Initializes the values array to the size of nonzeros
  m_col_index = new int[numnonzeros]; 
  m_row_index = new int[m_m + 1];  //Initializes the row index to the number of rows
  m_row_index[0] = 0; //Setting the first row of row index to zero.
  
  for (int i = 0;i < arraySize; i++) {
    if (array[i] != 0) { //Adds nonzeros to the values array.
      m_values[m_nonzeros] = array[i];
      m_col_index[m_nonzeros] = counter;
      m_nonzeros++;
    }
      
    if (counter == m_n or i == arraySize - 1) {  //Indicates a new row must be started.
      m_row_index[rownum] = m_nonzeros;  //Adds the running total of nonzeros to row_index.

      rownum++;
      counter = 0; //Reset counter with new row
    }
    
    counter++;
  }
  
  cout << endl;
  
}

int CSR::getAt(int row, int  col) const{
  if (row > m_m or col > m_n or col < 0 or row < 1) {
    throw std::out_of_range ("Invalid selection");
  }
  
    else {
  int row_start = m_row_index[row - 1];
  int row_end = m_row_index[row];
  
  for (int i = row_start;i < row_end;i++) {
    if (m_col_index[i] == col)
      return m_values[i];
  }
    }
} 

bool CSR::operator==(const CSR & rhs) const{
  bool value = true;
  
  for (int i = 0; i<rhs.m_nonzeros;i++) {
    if(m_values[i] != rhs.m_values[i])
      return false;
    if(m_col_index[i] != rhs.m_col_index[i])
      return false;
  }

  for (int i = 0; i < m_m+1;i++) {
    if (m_row_index[i] != rhs.m_row_index[i])
      return false;
  }
  
  return value;
}

int CSR::sparseRatio(){
  double zeros = m_arraySize - m_nonzeros;
  double deci_ratio = zeros/m_arraySize;
  
  return deci_ratio*100;
}
