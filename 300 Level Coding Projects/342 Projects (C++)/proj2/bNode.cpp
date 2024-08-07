
#include "bNode.h"
BNode::BNode(){}
BNode::BNode(string key, string author, string text){
            _key = key;
            _author = author;
            _left = nullptr;
            _right = nullptr;
            _flag = false;
            _parent = nullptr;
            createTextTree(text);
        }
BNode::~BNode(){
  _tree.~WordTree();
  _key = " ";
  _author = " ";
  _left = nullptr;
  _right = nullptr;
  _parent = nullptr;
}

void BNode::createTextTree(string text){
    istringstream iss(text);
    vector<string> tokens;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
    for (std::vector<string>::iterator it = tokens.begin() ; it != tokens.end(); ++it){
        _tree.insert(*it);
    }
}

int BNode::findFrequency(string word){
  int freq = 0;

  if(_tree.find(word) == nullptr)
    return 0;

  else {
    freq = _tree.find(word)->getFrequency();
      return freq;
  }
}

int BNode::searchCount(string word){
  return _tree.searchCount(word);
}

int BNode::getTextTreeHeight(){
  return _tree.getRootHeight();
}

int BNode::getNodeHeight(string word){
  return _tree.getNodeHeight(word);
}
