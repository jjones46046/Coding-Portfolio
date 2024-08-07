#include "wordTree.h"

WordTree::~WordTree(){
  clearTree(_root);
  _root = nullptr;
}

void WordTree::updateHeight(Node *aNode) {  
}

void WordTree::clearTree(Node* aNode){
  if(aNode != aNode->_left) {
    clearTree(aNode->_left);
    clearTree(aNode->_right);
    delete aNode;
  }
}

void WordTree::inOrder(Node* aNode, std::ostream& ostr){
    /***********************************
     *      Do not modify
     * ********************************/
    if (aNode != nullptr){
        ostr << "[";
        inOrder(aNode->_left, ostr);
        ostr << aNode->_value << ":" << aNode->_count << ":" << aNode->_height;
        inOrder(aNode->_right, ostr);
        ostr << "]";
    }
}

Node* WordTree::find(Node* aNode, const string& element){
  if(aNode == NULL) 
    return NULL;
  
  if(aNode->_value != element) {
    find(aNode->_left, element);
    find(aNode->_right, element);
  }

  else {
    return aNode;
  }
}
Node* WordTree::find(const string& element){
  if (_root->_value == element)
    return _root;
  else {
    return find(_root, element);
}

Node* WordTree::rightRotation(Node* aNode){
  Node* temp = aNode->_left; //Storing the value of the parent node in its new position.
  aNode->_left = temp->right;
  temp->right = aNode;
  
  updateHeight(aNode);
    return temp;
}

Node* WordTree::leftRotation(Node* aNode){
  Node* temp = aNode->_right; //Storing the value of the parent node in its new position.
  aNode->_right = temp->left;
  temp->_left = aNode;

  updateHeight(aNode);
  return temp;
}

int WordTree::checkBalance(Node* aNode){
  int rchildheight;
  int lchildheight;

  if(aNode->_left == NULL)
    lchildheight = -1;
  else
    lchildheight = aNode->_left->_height;

  if (aNode->_right == NULL)
    rchildheight = -1;
  else
    rchildheight = aNode->_right->_height;
  
  int value = lchildheight - rchildheight;
  
  return value;
}

Node* WordTree::reBalance(Node* aNode){
    /***********************************
     *      To be implemented
     * Note: to avoid warnings, a
     * fictional return value is added,
     * your implementaion should return
     * a correct value.
     * ********************************/
    return nullptr;
}

void WordTree::insert(const string& element){
  if(_node == NULL) {
    Node* temp = new Node(element);
    temp->_height = 0;
    _root = temp;
    _count = 0;
    }
  
  else {
    insert(element, _root);
  }
}
Node* WordTree::insert(const string& element, Node*& aNode){
  if (aNode->_left == NULL) {
    aNode->_left = new Node(element);
  }
  else if( aNode->_right == NULL) {
    aNode->_right = new Node(element);
  }
  
  else if (element < aNode->_value) {
    insert(element, aNode->_left); //Running insert to find an empty slot on the left side.
    
    if(aNode->_left->_height - aNode->_right->_height == 2) {
      reBalance(aNode);
    }
  }
  
  else if(element > aNode->_value) {
    insert(element, aNode->_right);
    
    if(aNode->_right->_height - aNode->_left->_height == 2) {
      rebalance(aNode);
    }
  }
    return nullptr;
}

void WordTree::dump(std::ostream& ostr){
    /***********************************
     *      Do not modify
     * ********************************/
    inOrder(_root, ostr);
}

int WordTree::searchCount(string word){
    /***********************************
     *      To be implemented
     * Note: to avoid warnings, a
     * fictional return value is added,
     * your implementaion should return
     * a correct value.
     * ********************************/
    return 0;
}

int WordTree::searchCountHelp(Node* aNode, string word, int counter){
    /***********************************
     *      To be implemented
     * Note: to avoid warnings, a
     * fictional return value is added,
     * your implementaion should return
     * a correct value.
     * ********************************/
    return 0;
}

int WordTree::getRootHeight(){
    return _root->_height;
}

int WordTree::getNodeHeight(string word){
    /***********************************
     *      To be implemented
     * Note: to avoid warnings, a
     * fictional return value is added,
     * your implementaion should return
     * a correct value.
     * ********************************/
    return 0;
}

int WordTree::getNodeHeightHelp(Node* aNode, string word){
  if(_root->_value == word) {
    return 0;
  }

  else {
  }
    return 0;
}
