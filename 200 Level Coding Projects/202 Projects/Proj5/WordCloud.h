#ifndef WORDCLOUD_H
#define WORDCLOUD_H

#include "LL.cpp"
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

//Constants (You may add additional)

//Common words to be removed from output
const vector<string> EXCLUDE_LIST{"a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", "can", "can't", "cannot", "com", "could", "couldn't", "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down", "during", "each", "else", "ever", "few", "for", "from", "further", "get", "had", "hadn't", "has", "hasn't", "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's", "http", "i", "i'd", "i'll", "i'm", "i've", "I", "I'd", "I'll", "I'm", "I've", "if", "in", "into", "is", "isn't", "it", "it's", "its", "itself", "just", "k", "let's", "like", "me", "more", "most", "mustn't", "my", "myself", "no", "nor", "not", "of", "off", "on", "once", "only", "or", "other", "ought", "our", "ours", "ourselves", "out", "over", "own", "r", "same", "shall", "shan't", "she", "she'd", "she'll", "she's", "should", "shouldn't", "so", "some", "such", "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then", "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've", "this", "those", "through", "to", "too", "under", "until", "up", "very", "was", "wasn't", "we", "we'd", "we'll", "we're", "we've", "were", "weren't", "what", "what's", "when", "when's", "where", "where's", "which", "while", "who", "who's", "whom", "why", "why's", "with", "won't", "would", "wouldn't", "www", "you", "you'd", "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"};

class WordCloud{
 public:
  // Name: WordCloud (Default Constructor)
  // Desc: Used to pass an essay (in input file) and generates word list and frequency
  //       Dynamically allocates m_cloud. m_fileName would be hardcoded.
  // Preconditions: Input file has correctly formatted input
  // Postconditions: Returns list of items and their frequency (stored in LL)
  WordCloud();
  // Name: WordCloud (Overloaded Constructor) - Passed filename for input
  // Desc: Used to pass an essay (in input file) and generates word list and frequency
  //       Dynamically allocates m_cloud
  // Preconditions: Input file has correctly formatted input
  // Postconditions: Returns list of items and their frequency (stored in LL)
  WordCloud(string);
  // Name: ~WordCloud (Destructor)
  // Desc: Deallocates m_cloud
  // Preconditions: m_cloud has been dynamically allocated
  // Postconditions: m_cloud is empty
  ~WordCloud();
  // Name: LoadFile()
  // Desc: Loads a file (everything as a string although could be anything stringlike)
  //       Each input file is an unknown length
  // Preconditions: m_fileName is populated with good data
  // Postconditions: m_cloud is populated
  void LoadFile();
  // Name: RemovePunct (passed a string
  // Desc: When passed a string, removes all non letter punctation on the beginning
  //       or end of the string.
  //       These are all ok: "don't" "inner-circle" but "candy," is bad
  //       Is called during LoadFile
  // Preconditions: Strings to be evaluated
  // Postconditions: Punctuation is removed from string
  void RemovePunct(string&);
  // Name: RemoveCommon
  // Desc: Iterates through m_cloud and removes all common words after they are read
  // Preconditions: m_cloud is populated and EXCLUDE_LIST is populated
  // Postconditions: m_cloud will have no words from EXCLUDE_LIST
  void RemoveCommon();
  // Name: RemoveSingles
  // Desc: Asks the user if they would like to remove words with a frequency of 1
  //       Should use the overloaded [] NOTE: Don't forget m_cloud is a pointer!
  // Preconditions: m_cloud is populated
  // Postconditions: m_cloud will have no words with a frequency of 1
  void RemoveSingles();
  // Name: Export
  // Desc: Iterates through m_cloud and exports to file "data:frequency"
  // Preconditions: m_cloud is populated
  // Postconditions: New export file is created with "data:frequency" one on each line
  void Export();
  // Name: Start
  // Desc: Calls LoadFile, RemoveCommon, Diplay, and Export 
  // Preconditions: None (filename is available)
  // Postconditions: None
  void Start();
 private: 
  string m_fileName; //Name of the input file
  LL<string>* m_cloud; //LL to store all data
};

#endif
