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
    // Helper functions for AVL Tree
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item) {
    // If tree is empty, create new root
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }  
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        } else if (current->getKey() < new_item.first) {
            current = current->getRight();
        } else {
            current->setValue(new_item.second);
            return;
        }
    }
    
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
        parent->updateBalance(-1);
    } else {
        parent->setRight(newNode);
        parent->updateBalance(1);
    }
 
    if (parent->getBalance() == 0) {
        return;
    } else if (parent->getBalance() == 1 || parent->getBalance() == -1) {
        insertFix(parent, newNode);
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

 template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* rightChild = node->getRight();
    
    // Update child pointers
    node->setRight(rightChild->getLeft());
    
    if (rightChild->getLeft() != nullptr) {
        rightChild->getLeft()->setParent(node);
    }
    
    // Update parent pointers
    rightChild->setParent(node->getParent());
    
    if (node->getParent() == nullptr) {
        this->root_ = rightChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(rightChild);
    } else {
        node->getParent()->setRight(rightChild);
    }
    
    // Link node as left child of rightChild
    rightChild->setLeft(node);
    node->setParent(rightChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* leftChild = node->getLeft();
    //Update pointers of tree
   
    node->setLeft(leftChild->getRight());
    
    if (leftChild->getRight() != nullptr) {
        leftChild->getRight()->setParent(node);
    }
    
    leftChild->setParent(node->getParent());
    
    if (node->getParent() == nullptr) {
        this->root_ = leftChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(leftChild);
    } else {
        node->getParent()->setRight(leftChild);
    }
    
    // Link node as right child of leftChild
    leftChild->setRight(node);
    node->setParent(leftChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    if (parent == nullptr || node == nullptr || parent->getParent() == nullptr) {
        return; 
    }
    
    AVLNode<Key, Value>* grandparent = parent->getParent();
    
    if (parent == grandparent->getLeft()) {
        grandparent->updateBalance(-1);
        
        if (grandparent->getBalance() == 0) {
            return; 
        } else if (grandparent->getBalance() == -1) {
            insertFix(grandparent, parent);
        } else if (grandparent->getBalance() == -2) {
            if (node == parent->getLeft()) {
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                // Left-Right case: Double rotation
                rotateLeft(parent);
                rotateRight(grandparent);
                
                if (node->getBalance() == -1) {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else {  
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }
                
                node->setBalance(0);
            }
        }
    } else {
        grandparent->updateBalance(1);
        
        if (grandparent->getBalance() == 0) {
            return;  
        } else if (grandparent->getBalance() == 1) {
            insertFix(grandparent, parent);
        } else if (grandparent->getBalance() == 2) {
            if (node == parent->getRight()) {
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                rotateRight(parent);
                rotateLeft(grandparent);
                
                if (node->getBalance() == 1) {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else {  // node->getBalance() == -1
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                }
                
                node->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // Find node to remove
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == nullptr) {
        return; 
    }
    
    // If node has two children, swap with predecessor
    if (node->getLeft() != nullptr && node->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
    }
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
    //
    int8_t diff = 0;
    if (parent != nullptr) {
        if (node == parent->getLeft()) {
            diff = 1; 
        } else {
            diff = -1;
        }
    }

    if (parent == nullptr) {
        this->root_ = child;  
    } else {
        if (node == parent->getLeft()) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    }
    if (child != nullptr) {
        child->setParent(parent);
    }
   
    delete node;
  //Rebalance
    if (parent != nullptr) {
        removeFix(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff) {
    if (node == nullptr) return;
    
    AVLNode<Key, Value>* parent = node->getParent();
    
    int8_t ndiff = 0;
    if (parent != nullptr) {
        if (node == parent->getLeft()) {
            ndiff = 1;  
        } else {
            ndiff = -1; 
        }
    }
    

    node->updateBalance(diff);
    
    if (node->getBalance() == 2) {
        AVLNode<Key, Value>* rightChild = node->getRight();
        
        if (rightChild->getBalance() == -1) {
            AVLNode<Key, Value>* rlChild = rightChild->getLeft();
            rotateRight(rightChild);
            rotateLeft(node);
            

            if (rlChild->getBalance() == 1) {
                node->setBalance(-1);
                rightChild->setBalance(0);
                rlChild->setBalance(0);
            } else if (rlChild->getBalance() == 0) {
                node->setBalance(0);
                rightChild->setBalance(0);
                rlChild->setBalance(0);
            } else {
                node->setBalance(0);
                rightChild->setBalance(1);
                rlChild->setBalance(0);
            }
            rlChild->setBalance(0);
        } else {
            rotateLeft(node);
            
            if (rightChild->getBalance() == 0) {
                node->setBalance(1);
                rightChild->setBalance(-1);
                return;
            } else {
                node->setBalance(0);
                rightChild->setBalance(0);
            
            }
        }
    } else if (node->getBalance() == -2) {
        AVLNode<Key, Value>* leftChild = node->getLeft();
        
        if (leftChild->getBalance() == 1) {
            AVLNode<Key, Value>* lrChild = leftChild->getRight();
            rotateLeft(leftChild);
            rotateRight(node);
            
     
            if (lrChild->getBalance() == -1) {
                node->setBalance(1);
                leftChild->setBalance(0);
                lrChild->setBalance(0);
            } else if (lrChild->getBalance() == 0) {
                node->setBalance(0);
                leftChild->setBalance(0);
                lrChild->setBalance(0);
            } else { 
                node->setBalance(0);
                leftChild->setBalance(-1);
                lrChild->setBalance(0);
            }
            lrChild->setBalance(0);
        } else {
      
            rotateRight(node);
            
            if (leftChild->getBalance() == 0) {
                node->setBalance(-1);
                leftChild->setBalance(1);
           
                return;
            } else { 
                node->setBalance(0);
                leftChild->setBalance(0);
             
            }
        }
    } else if (node->getBalance() == 1 || node->getBalance() == -1) {

        return;
    }

    if (parent != nullptr) {
        removeFix(parent, ndiff);
    }
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
