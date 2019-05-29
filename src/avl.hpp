#ifndef AVL_HPP
#define AVL_HPP

#include "bst.hpp"

class AVL : public BST
{  public:
      void insert(const std::string&, int);
   protected:
      int balance_factor(const Node*) const;
      Node* insert(Node*, const std::string&, int);
      Node* rotate_right(Node*);
      Node* rotate_left(Node*);
};

#endif
