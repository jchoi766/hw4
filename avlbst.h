#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		AVLNode<Key, Value>* insertHelper(
			AVLNode<Key, Value>* root, 
			const std::pair<const Key, Value>& new_item, 
			AVLNode<Key, Value>* parent, 
			AVLNode<Key, Value>*& added
			);
		void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
		void rotateRight(AVLNode<Key, Value>* node);
		void rotateLeft(AVLNode<Key, Value>* node);
		void removeFix(AVLNode<Key, Value>* n, int diff);
		int getHeight(AVLNode<Key, Value>* root);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
	// TODO
	AVLNode<Key, Value>* root = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
	//AVLNode<Key, Value>* added = insertHelper(root, new_item, NULL);
	AVLNode<Key, Value>* added = NULL;
	BinarySearchTree<Key, Value>::root_ = insertHelper(root, new_item, NULL, added);

	//if tree was empty, update root 
	/*if (BinarySearchTree<Key, Value>::root_ == NULL) {
		BinarySearchTree<Key, Value>::root_ = added;
	}*/

	AVLNode<Key, Value>* p = added->getParent();
	//std::cout << added->getKey()<< std::endl;

	// update parent's balance after adding 
	
	if (p != NULL && added != root) {
		if (std::abs(p->getBalance()) == 1) {
			p->setBalance(0);
		} else { // parent's balance was = 0 
			if (p->getLeft() == added) { // added to left 
				p->setBalance(-1);
			} else { // added to right 
				p->setBalance(1);
			}
			insertFix(p, added);
		}
		//std::cout << "p: " << p->getKey() << " after balancing is " << static_cast<int>(p->getBalance()) << std::endl;

	}
	
	
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(
	AVLNode<Key, Value>* root, 
	const std::pair<const Key, Value>& new_item, 
	AVLNode<Key, Value>* parent, 
	AVLNode<Key, Value>*& newNode
	) 
{
	if (root == NULL) {
		AVLNode<Key, Value>* added = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
		//insertFix(parent, added);
		newNode = added;
		return added;
	}

	if (root->getKey() == new_item.first) {
		// replace 
		root->setValue(new_item.second);
		newNode = root;
		return root;
	}

	//recurse down 
	if (new_item.first < root->getKey()) {
		// go to left 
		root->setLeft(insertHelper(root->getLeft(), new_item, root, newNode));
		root->getLeft()->setParent(root);
	} else {
		// go to right 
		root->setRight(insertHelper(root->getRight(), new_item, root, newNode));
		root->getRight()->setParent(root);
	}
	return root;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) 
{ // n is the newly added node, p is the parent of n. 
	if (p == NULL || p->getParent() == NULL) return;

	AVLNode<Key, Value>* g = p->getParent();
	//check if p is left child of g 
	int isLeft = (g->getLeft() == p) ? 1 : -1;
	g->setBalance(g->getBalance() + -1*isLeft);
	//std::cout << "g Balance : " << static_cast<int>(g->getBalance()) << std::endl;
	if (g->getBalance() == 0) return; // case 1 
	else if (g->getBalance() == -1 * isLeft) {
		insertFix(g, p); // case 2
	} else { // case 3
		bool doubleCall = false;
		if (isLeft == 1) {
			if (g->getBalance() == -2 ) { 
				if (g->getLeft() == p && p->getLeft() == n) {
					// zig zig 
					rotateRight(p);
					p->setBalance(0);
					g->setBalance(0);
				} else {
					// zig zag 
					doubleCall = true;
					rotateLeft(n);
					rotateRight(n);
				}
			}
		} else { // isRight  
			if (g->getBalance() == 2) { 
				if (g->getRight() == p && p->getRight() == n) {
					// zag zag 
					rotateLeft(p);
					p->setBalance(0);
					g->setBalance(0);
				} else {
					// zag zig 
					doubleCall = true;
					rotateRight(n);
					rotateLeft(n);
				}
			}
		}

		if (doubleCall) {
			//redo balance factors: 
			if (n->getBalance() == -1 * isLeft) { // 3a 
				p->setBalance(0);
				g->setBalance(1 * isLeft);
				n->setBalance(0);
			} else if (n->getBalance() == 0) { // 3b 
				p->setBalance(0);
				g->setBalance(0);
				n->setBalance(0);
			} else { // 3c 
				p->setBalance(-1 * isLeft);
				g->setBalance(0);
				n->setBalance(0);
			}
		}
	}

}
template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* p) {
	//std::cout <<"rotate right" <<std::endl;
	AVLNode<Key, Value>* g = p->getParent();
	if (g == NULL) return;

	AVLNode<Key, Value>* rChild = p->getRight();
	AVLNode<Key, Value>* gg = g->getParent();
	
	// rotating 
	p->setRight(g);
	g->setLeft(rChild);	
	if (rChild != NULL){
		rChild->setParent(g);
	}

	g->setParent(p);	
	p->setParent(gg);

	if (gg == NULL) { // g is root 
		BinarySearchTree<Key, Value>::root_ = p;
	} else if (gg->getLeft() == g) {
		gg->setLeft(p);
	} else if (gg->getRight() == g) {
		gg->setRight(p);
	} 
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* p) {
	//std::cout <<"rotate left" <<std::endl;

	AVLNode<Key, Value>* g = p->getParent();
	if (g == NULL) return;

	AVLNode<Key, Value>* lChild = p->getLeft();
	AVLNode<Key, Value>* gg = g->getParent();
	
	// rotating 
	p->setLeft(g);
	g->setRight(lChild);	
	if (lChild != NULL){
		lChild->setParent(g);
	}

	// set parents 
	g->setParent(p);	
	p->setParent(gg);

	if (gg == NULL) { // g is root 
		BinarySearchTree<Key, Value>::root_ = p;
	} else if (gg->getLeft() == g) {
		gg->setLeft(p);
	} else if (gg->getRight() == g) {
		gg->setRight(p);
	} 

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
	AVLNode<Key, Value>* toDelete = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
	if (toDelete != NULL) {
		
		//comment this chunck out 
		
		if (toDelete->getRight() != NULL && toDelete->getLeft() != NULL) {
			// 2 children exist. swap toDelete w its predecessor 
			AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(toDelete)); //find predecessor 
			AVLTree<Key, Value>::nodeSwap(toDelete, pred);
		}
		

		AVLNode<Key, Value>* p = toDelete->getParent(); 
		int diff;
		if (p != NULL) {
			// toDelete is left child 
			if (p->getLeft() == toDelete) {
				diff = 1;
			} else {
				diff = -1;
			}
		} 
		else {
			//head is being deleted, determine if balances are disrupted 
			if (toDelete->getLeft() != NULL) {
				diff = -1;
			} else if (toDelete->getRight() != NULL) {
				diff = 1;
			}
		} 
		BinarySearchTree<Key, Value>::remove(key);
		removeFix(p, diff);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
	if (n == NULL) return;
	AVLNode<Key, Value>* p = n->getParent();
	int nDiff = 0;
	if (p != NULL) {
		if (p->getLeft() == n) nDiff = 1;
		else nDiff = -1;
	}
	if ( (n->getBalance() + diff) == 1 * diff){
		// case 2 
		n->setBalance(-1 * diff);
		return;
	} else if ( (n->getBalance() + diff) == 0) {
		// case 3 
		n->setBalance(0);
		removeFix(p, nDiff);
	} else if ((n->getBalance() + diff) == 2 * diff) {
		// case 1 
		if (diff == -1) { // n is left child
			AVLNode<Key, Value>* c = n->getLeft();
			// case 1a  - zig zig 
			if (c->getBalance() == -1) {
				rotateRight(c);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, nDiff);
			}
			// case 1b  - zig zig 
			else if (c->getBalance() == 0) {
				rotateRight(c);
				n->setBalance(-1);
				c->setBalance(1);
				return;
			}
			// case 1c - zig zag 
			else if (c->getBalance() == 1) {
				AVLNode<Key, Value>* g = c->getRight();
				rotateLeft(g);
				rotateRight(c);
				
				if (g->getBalance() == 1){
					n->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				} else if (g->getBalance() == 0) {
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				} else if (g->getBalance() == -1) {
					n->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p, nDiff);
			}
		} else if (diff == 1) { // n is right child 
			AVLNode<Key, Value>* c = n->getRight();
			// case 1a  - zag zag 
			if (c->getBalance() == 1) {
				rotateLeft(c);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, nDiff);
			} 
			// case 1b  - zag zag  
			else if (c->getBalance() == 0) {
				rotateLeft(c);
				n->setBalance(1);
				c->setBalance(-1);
				return;
			}
			// case 1c - zag zig  
			else if (c->getBalance() == -1) {
				AVLNode<Key, Value>* g = c->getLeft();
				rotateRight(g); 
				rotateLeft(c);

				if (g->getBalance() == -1){
					n->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				} else if (g->getBalance() == 0) {
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				} else if (g->getBalance() == 1) {
					n->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p, nDiff);
			}
		}
	}
}

template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* root) {
	if (root == NULL) return -1;

	int lHeight = getHeight(root->getLeft());
	int rHeight = getHeight(root->getRight());

	return std::max(lHeight, rHeight) + 1;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
