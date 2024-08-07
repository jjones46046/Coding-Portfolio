#include "bookTree.h"
BookTree::BookTree(){
  _root = nullptr;
}

BookTree::~BookTree(){
  clear(_root);
}

BNode* BookTree::makeSplay(BNode* root, string key){
  if (root == nullptr or root->_key == key) { //The book were searching for is the root.
    return root;
  }
  
  if (root->_key > key) {  //Key could be on the left side if true.

    if(root->_left == NULL)
      return root;

    if(root->_left->_key > key) { //Zig-Zig
      root->_left->_left = makeSplay(root->_left->_left, key);
      root = rightRotate(root);
    }

    else if(root->_right->_key < key) { //Zig-Zag
      root->_left->_right = makeSplay(root->_left->_right, key);
      if(root->_left->_right != NULL)
	root->_left = leftRotate(root->_left);
    }
  } //End of search through left side.

  else if(root->_key < key) { //Key could be on the right side if true
    if(root->_right == NULL)
      return root;

    if(root->_right->_key > key) { //Zag Zig
      root->_right->_left = makeSplay(root->_right->_left, key);
      if(root->_right->_left != NULL)
	root->_right = rightRotate(root->_right);
    }

    else if(root->_right->_key < key) { //Zag-Zag
      root->_right->_right = makeSplay(root->_right->_right, key);
      root = leftRotate(root);
    }

    
    return (root->_right == NULL)? root: leftRotate(root);     
  }
}     
 
BNode* BookTree::rightRotate(BNode* x){
  BNode* lft = x->_left;
  x->_left = lft->_right;
  lft->_right = x;

  return lft;
}
BNode* BookTree::leftRotate(BNode* x){
  BNode* rt = x->_right;
  x->_right = rt->_left;
  rt->_left = x;
  
  return rt;
}

bool BookTree::insert(string key, string author, string text){
  string sample = "sample";
  if (makeSplay(_root, key) != NULL) {
    cout << "This book is already stored within the tree." << endl;
    return false;
  }

  else {
    BNode* newnode = new BNode(key, author, text);
    if(_root == NULL) {
      newnode->_left = newnode->_right = NULL;
      _root = newnode;
    }

    else {
      
    }

  }
    return true;
}

void BookTree::clear(BNode* root){
  if(root != root->_left) {
    clear(root->_left);
    clear(root->_right);
    delete root;
  }
}

void BookTree::inorderHelp(BNode* root, bool verbose) const{
     /***********************************
     *      Do not modify
     * ********************************/
    if (root != nullptr){
        if (verbose){
            cout << "(";
            inorderHelp( root->_left, verbose );
            cout << root->_key << ":" << root->_author;
			root->_tree.dump();
            inorderHelp( root->_right, verbose );
            cout << ")";
        }
        else{
            inorderHelp( root->_left, verbose );
            cout << root->_key;
            inorderHelp( root->_right, verbose );
        }
    }
}

void BookTree::dump(bool verbose) const{
     /***********************************
     *      Do not modify
     * ********************************/
    inorderHelp(_root, verbose);
}

int BookTree::findFrequency(string title, string word) {
  //Returns the number of occurrences of the word for a book
  BNode* temp = makeSplay(_root, title);

  return temp->findFrequency(word);
}

void BookTree::dumpTitle(string title){
  BNode* temp = makeSplay(_root, title);

  temp->_tree.dump();
}

int BookTree::searchCount(string title, string word){
  //Gives us the number of operations that occur when looking for a word in the
  //Avl tree of the given book title. Not the number of operations taken to get to
  //the book title node. Returns 0 if the title isn't found.
  if (_root->_key == title) {
    //Given node has been found at root.
    return _root->_tree.searchCount(word);
  }

  else if (makeSplay(_root, title) == NULL) 
    return 0;

  else {
    BNode* temp = makeSplay(_root, title);
    return temp->_tree.searchCount(word);
  }
  
}

int BookTree::getTextTreeHeight(string title){
  BNode* temp = makeSplay(_root, title);
  
  return temp->getTextTreeHeight();
}
int BookTree::getWordHeight(string title, string word){
  BNode* temp = makeSplay(_root, title);

  return temp->_tree.getNodeHeight(word);
}

string BookTree::getRootKey(){
     /***********************************
     *      Do not modify
     * ********************************/
	return _root->_key;
}

void BookTree::loadData(string dataFile){
     /***********************************
     *      This function is implemented
     *      to help you. You should't need
     *      to modify it.
     * ********************************/
	//read the entire file at once
	std::ifstream t(dataFile);
	std::stringstream buffer;
	buffer << t.rdbuf();

	//time to parse the information
	string titleDelimiter = "<==>";
	string itemsDeleimiter = "<=>";
	string s = buffer.str();
	size_t pos = 0;
	string token;
	while ((pos = s.find(titleDelimiter)) != string::npos) {
		token = s.substr(0, pos);//token is complete data for one title
		//get the title
		int tempPos = token.find(itemsDeleimiter);
		string title = token.substr(0,tempPos);
		//convert title to lower case
		std::transform (title.begin(), title.end(), title.begin(), ::tolower);
		token.erase(0, tempPos + itemsDeleimiter.length()+1);
		//get the author
		tempPos = token.find(itemsDeleimiter);
		string author = token.substr(0,tempPos);
		//convert author to lower case
		std::transform (author.begin(), author.end(), author.begin(), ::tolower);
		token.erase(0, tempPos + itemsDeleimiter.length()+1);
		//get the text
		string text = token.substr(0,token.length() - 1);//remove last newline
		//clean up the text, remove all non-alphanumeric characters
          for( std::string::iterator iter = text.begin() ; iter != text.end() ; ){
               if( !std::isalnum(*iter) && *iter != ' ') 
                    iter = text.erase(iter) ;
               else 
                    ++iter ; // not erased, increment iterator
          }
		//convert text to lower case
		std::transform (text.begin(), text.end(), text.begin(), ::tolower);
		
		insert(title,author,text);

		//move to info for the next title
		s.erase(0, pos + titleDelimiter.length()+1);
	}
}
