//Title: proj5.cpp
//Author: Jeremy Dixon
//Date: 4/20/2020
//Description: This is main for project 5.
#include "WordCloud.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

//Inside the parameters for main, we can pass arguments
//argc is the count of arguments
//argv are the parameters themselves
int main(int argc, char* argv[]) {
  cout << "Welcome to UMBC Word Clouds" << endl;
  while (argc < 2) { //Checks to make sure the user passes a file to load
    cout << "You are missing a data file." << endl;
    cout << "Expected usage ./proj5 proj5_test1.txt" << endl;
    cout << "The input file should have some words" << endl;
    return 0;
  }
  WordCloud myCloud(argv[1]); //Creates a new word cloud by passing the filename
  myCloud.Start(); //Starts the wordCloud
  return 0;
}
