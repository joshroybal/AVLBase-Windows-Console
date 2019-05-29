#include <string>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "avl.hpp"

// public AVL class methods
void AVL::insert(const std::string& key, int idx_no)
{
   _root = insert(_root, key, idx_no);
}

// private AVL class methods
int AVL::balance_factor(const Node* node) const
{
   if ( !node ) return 0;
   return h(node->_left) - h(node->_right);
}

// AVL insertion
AVL::Node* AVL::insert(Node* p, const std::string& key, int idx_no)
{
   if ( !p ) return new Node(key, idx_no);

   if (key < p->_key)
      p->_left = insert(p->_left, key, idx_no);
   else if (key > p->_key)
      p->_right = insert(p->_right, key, idx_no);
   else {
      p->_index.push_back(idx_no);
      return p;
   }

   p->_height = 1 + max(h(p->_left), h(p->_right));
   int bal = balance_factor(p);
   if (bal >= -1 && bal <= 1) return p;	// tests show some speed gain
   int left_bal = balance_factor(p->_left);
   int right_bal = balance_factor(p->_right);

   // Left Left Case
   if (bal > 1 && left_bal > 0)
      return rotate_right(p);

   // Right Right Case
   if (bal < -1 && right_bal < 0)
      return rotate_left(p);

   // Left Right Case
   if (bal > 1 && left_bal < 0) {
      p->_left = rotate_left(p->_left);
      return rotate_right(p);
   }

   // Right Left Case
   if (bal < -1 && right_bal > 0) {
      p->_right = rotate_right(p->_right);
      return rotate_left(p);
   }

   return p;
}

AVL::Node* AVL::rotate_left(Node* p)
{
   Node* q = p->_right;
   Node* localtemp = q->_left;

   q->_left = p;
   p->_right = localtemp;

   p->_height = max(h(p->_left), h(p->_right)) + 1;
   q->_height = max(h(q->_left), h(q->_right)) + 1;

   return q;
}

AVL::Node* AVL::rotate_right(Node* p)
{
   Node* q = p->_left;
   Node* localtemp = q->_right;

   q->_right = p;
   p->_left = localtemp;

   p->_height = max(h(p->_left), h(p->_right)) + 1;
   q->_height = max(h(q->_left), h(q->_right)) + 1;

   return q;
}
