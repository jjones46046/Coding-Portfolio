/**************************************
 ** File: WordCloud.cpp
 ** Project: CMSC 202 Project 4, Spring 2020
 ** Author: Jared Jones
 ** Date: 5/7/20
 ** Section: 11
 ** Email: jjones20@umbc.edu
 **
 ** This file contains the functions for WordCloud.h
 ** This function creates a dynamically allocated word cloud,
 ** removes all common words from the cloud based on a list called "EXCLUDE_LIST,
 ** and exports the cloud to a text file.
 **
 *************************************/
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <fstream>
#include "WordCloud.h"
using namespace std;


WordCloud::WordCloud() {
  m_fileName = "default.txt";
  m_cloud = new LL<string>();
}

WordCloud::WordCloud(string essay) {
  m_fileName = essay;
  m_cloud = new LL<string>();
}

WordCloud::~WordCloud() {
  //Runs destructor, then deletes m_cloud
  //  m_cloud->~LL();
  delete m_cloud;
}

void WordCloud::LoadFile() {
  fstream essay;
  string word;

  essay.open(m_fileName.c_str());
  //Loading words in until the end
  while (essay >> word) {
    RemovePunct(word);
    m_cloud->Insert(word);
  }

  essay.close();
}

void WordCloud::RemovePunct(string& word) {
  char quote = 39;
  char punct[26] =  {'!','"','#','$','%','&','(',')','*','+',',','-','.','/',';',':','?','[',']','`','~','{','}','|','_',quote};

  
  for (int i = 0;i < (int)sizeof(punct); i++) {
    //Checking the first letter of word to be punctuation
    if (word[0] == punct[i]) {
      word.erase(0,1);
    }
    //Checking the last letter of word to be punctuation
    if (word[word.size() - 1] == punct[i]) {
      word.erase(word.size() - 1,1);
    }
  }
}

void WordCloud::RemoveCommon() {
  //Iterating through EXCLUDE_LIST
  for (int i = 0; i < (int)EXCLUDE_LIST.size(); i++) {
    if (m_cloud->Find(EXCLUDE_LIST[i]) != nullptr) {
      //Removes a word if it was found within Exclude list
      m_cloud->RemoveAt(EXCLUDE_LIST[i]);
      cout << "Removed: "<< EXCLUDE_LIST[i] << endl;
    }
    else {
      continue;
    }
  }
}

void WordCloud::RemoveSingles() {
  string answer;
  int size = m_cloud->GetSize();

  cout << "Would you like to remove all words with a frequency of 1?" << endl;
  cin >> answer;

  if (answer == "Y" or answer == "yes" or answer == "y" or answer == "YES") {
    for (int i = 0; i < size;i++) {
      pair<string,int> data = (*m_cloud)[i];
      cout << data.first << endl;
      if (data.second == 1) {
	m_cloud->RemoveAt(data.first);
	cout << "Removed: " << data.first << endl;
      }
    }
  }
}

void WordCloud::Export() {
  ofstream expor;
  string fileName;
  //Asking for user input
  cout << "What would you like to name the export file?(end with a .txt)" << endl;
  cin >> fileName;

  expor.open(fileName);
  expor << *m_cloud;
  expor.close();
  cout << "Output to exportFile: " << fileName << endl;
  cout << m_cloud->GetSize() << " unqiue words exported." << endl;
}

void WordCloud::Start() {
  LoadFile();
  RemoveCommon();
  //RemoveSingles();      //Couldn't get this to work
  Export();
}
