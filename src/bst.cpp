#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <iomanip>
#include "bst.hpp"

// public BST class methods
void BST::insert(const std::string& x, const int& idx)
{
   insert(_root, x, idx);
}

void BST::print() const
{
   std::cout << "\n in-order traversal of binary search tree" << std::endl;
   print(_root);
   std::cout << std::endl;
}

void BST::preOrder() const
{
   std::cout << "\n pre-order traversal of binary search tree" << std::endl;
   preOrder(_root);
   std::cout << std::endl;
}

void BST::postOrder() const
{
   std::cout << "\n post-order traversal of binary search tree" << std::endl;
   postOrder(_root);
   std::cout << std::endl;
}

// use queue to pop out each level in order
void BST::levelOrder()
{
   std::queue<Node*> levelqueue;

   std::cout << "\n level-order traversal of binary search tree" << std::endl;
   levelqueue.push(_root);
   int l = 1;
   while (!levelqueue.empty()) {
      Node *nodeout =  levelqueue.front();
      levelqueue.pop();
      std::cout << ' ' << nodeout;
      std::cout << ' ' << std::setw(48) << std::left << nodeout->_key;
      std::cout << std::setw(10) << nodeout->_left;
      std::cout << std::setw(10) << nodeout->_right << std::endl;
      if (nodeout->_left) levelqueue.push( nodeout->_left);
      if (nodeout->_right) levelqueue.push( nodeout->_right);
   }
}

void BST::inOrder(std::ofstream& ofstr) const
{
   inOrder(ofstr, _root);
}

void BST::load(std::string* a) const
{
   int i = 0;
   load(_root, a, i);
}

void BST::loadIdxVector(std::vector<int>& v) const
{
   loadIdxVector(_root, v);
}

void BST::loadIdxVectorGTE(std::vector<int>& v, const std::string& target) const
{
   loadIdxVectorGTE(_root, v, target);
}

void BST::serializeText(std::ofstream& ofstr) const
{
   serializeText(ofstr, _root);
}

void BST::serializeBinary(std::ofstream& ofstr) const
{
   serializeBinary(ofstr, _root);
}

void BST::deserializeText(std::ifstream& ifstr)
{
   int recno = 0;
   std::string key;

   while (getline(ifstr, key)) {
      insert(key, ++recno);
      key.clear();
   }
}

std::vector<int> BST::search(std::string x)
{
   Node* found = search(_root, x);
   if (found != NULL)
      return found->_index;
   else {  // return dummy empty vector when nothing found
      std::vector<int> empty;
      return empty;
   }
}

int BST::height() const
{
   // return h(_root);
   if ( !_root ) return -1;
   return _root->_height;
}

int BST::size() const
{
   return size(_root);
}

int BST::level(const std::string x) const
{
   return level(_root, x, 1);
}

// protected BST class methods
// post-order traversal node deletion
void BST::destroy(Node* p)
{
   if (!p) return;
   destroy(p->_left);
   destroy(p->_right);
   delete p;
}

int BST::max(int m, int n) const
{
   if (m > n) return m;
   else return n;
}

void BST::insert(Node*& p, const std::string& x, const int& idx)
{
   if (!p) p = new Node(x, idx);
   else if (x == p->_key) { p->_index.push_back(idx); return; }
   else if (x < p->_key) insert(p->_left, x, idx);
   else insert(p->_right, x, idx);
	p->_height = 1 + max(h(p->_left), h(p->_right));
}

void BST::print(Node *p) const
{
   if (!p) return;
   print( p->_left);
   std::cout << ' ' << p->_key;
   std::cout << std::endl;
   print( p->_right);
}

void BST::preOrder(Node *p) const
{
   if (p) {
      std::cout << ' ' << p->_key << std::endl;
      preOrder( (p)->_left);
      preOrder( (p)->_right);
   }
}

void BST::postOrder(Node *p) const
{
   if (!p) return;
   postOrder( (p)->_left);
   postOrder( (p)->_right);
   std::cout << ' ' << p->_key << std::endl;
}

void BST::inOrder(std::ofstream& ofstr, Node *p) const
{
   if (!p) return;
   inOrder(ofstr, reinterpret_cast<Node *>(p->_left));
   ofstr << p->_key << std::endl;
   inOrder(ofstr, reinterpret_cast<Node *>(p->_right));
}

// sort a using in-order traversal
void BST::load(Node* p, std::string* a, int& i) const
{
   if (!p) return;
   load( p->_left, a, i);
   a[i++] = p->_key;
   load( p->_right, a, i);
}

// create an index to sorted values using an in-order traversal of the tree
void BST::loadIdxVector(Node* p, std::vector<int>& v) const
{
   if (!p) return;
   loadIdxVector( p->_left, v);
   // might be longer than one if there're duplicates
   for (unsigned int i = 0; i < p->_index.size(); i++)
      v.push_back(p->_index[i]);
   loadIdxVector( p->_right, v);
}

// create an index to sorted values using an in-order traversal of the tree portion greater than p
void BST::loadIdxVectorGTE(Node* p, std::vector<int>& v, const std::string& target) const
{
   if (!p) return;
   loadIdxVectorGTE(p->_left, v, target);
   // might be longer than one if there're duplicates
   if (p->_key >= target)  for (unsigned int i = 0; i < p->_index.size(); i++) v.push_back(p->_index[i]);
   loadIdxVectorGTE(p->_right, v, target);
}

// serialize tree to text file using pre-order traversal
void BST::serializeText(std::ofstream& ofstr, Node *p) const
{
   if (p) {
      ofstr << p->_key << std::endl;
      serializeText(ofstr,  (p)->_left);
      serializeText(ofstr,  (p)->_right);
   }
}

// serialize tree to binary file using pre-order traversal
void BST::serializeBinary(std::ofstream& ofstr, Node *p) const
{
   if (p) {
      int pos = p->_key.find_last_of(",");
      static std::istringstream ssidx( p->_key.substr(pos + 1) );
      static int idx;
      ssidx >> idx;
      ofstr.write(reinterpret_cast<const char*>(&idx), sizeof(int));
      serializeBinary(ofstr, reinterpret_cast<Node*>(p->_left));
      serializeBinary(ofstr, reinterpret_cast<Node*>(p->_right));
   }
}

BST::Node* BST::search(Node *p, const std::string x) const
{
   if (p == 0)
      return p;
   else if (x == p->_key)
      return p;
   else if (x < p->_key)
      return search(p->_left, x);
   else
      return search(p->_right, x);
}

int BST::h(Node *p) const
{
   if (!p) return -1;
   return p->_height;
}

int BST::size(Node* p) const
{
   if ( !p ) return 0;
   return 1 + size(p->_left) + size(p->_right);
}

int BST::level(Node *p, const std::string x, int n) const
{
   if (p == 0) return 0;
   if (x == p->_key) return n;
   int downlevel = level( (p)->_left, x, n + 1);
   if (downlevel != 0) return downlevel;
   downlevel = level( (p)->_right, x, n + 1);
   return downlevel;
}
