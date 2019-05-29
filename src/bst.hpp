#ifndef BST_HPP
#define BST_HPP

#include <vector>

class BST
{
   public:
      BST() : _root(0) {}
      ~BST() { destroy(_root); }
      void insert(const std::string&, const int&);
      void print() const;
      void preOrder() const;
      void postOrder() const;
      void levelOrder();
      void inOrder(std::ofstream&) const;
      void load(std::string*) const;
      void loadIdxVector(std::vector<int>&) const;
      void loadIdxVectorGTE(std::vector<int>&, const std::string&) const;
      void serializeText(std::ofstream&) const;
      void serializeBinary(std::ofstream&) const;
      void deserializeText(std::ifstream&);
      // std::string betasearch(const std::string) const;
      std::vector<int> search(std::string);
      int height() const;
      int size() const;
      int level(const std::string) const;
   protected:
      struct Node;
      Node* _root;
      void destroy(Node*);
      int max(int, int) const;
      void insert(Node*&, const std::string&, const int&);
      void print(Node *) const;
      void preOrder(Node *) const;
      void postOrder(Node *) const;
      void inOrder(std::ofstream&, Node *) const;
      void load(Node*, std::string*, int&) const;
      void loadIdxVector(Node*, std::vector<int>&) const;
      void loadIdxVectorGTE(Node*, std::vector<int>&, const std::string&) const;
      void serializeText(std::ofstream&, Node *) const;
      void serializeBinary(std::ofstream&, Node *) const;
      Node* search(Node *, const std::string) const;
      int h(Node *) const;
      int size(Node*) const;
      int level(Node *, const std::string, int) const;
};

// Node definition
struct BST::Node
{
   std::string _key;
   std::vector<int> _index;
   int _height;
   Node* _left;
   Node* _right;
   Node(const std::string& key, int n) : _key(key), _height(0), _left(0),
      _right(0) { _index.push_back(n); }
};

#endif
