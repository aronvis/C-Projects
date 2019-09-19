#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <stack>

/**
* A templated class for a Node in a search tree. 
*/
template <typename Key, typename Value>
class Node
{
public:
	Node(const Key& key, const Value& value, Node<Key, Value>* parent);
	virtual ~Node();

	const std::pair<Key, Value>& getItem() const;
	std::pair<Key, Value>& getItem();
	const Key& getKey() const;
	const Value& getValue() const;
	Key& getKey();
	Value& getValue();

	virtual Node<Key, Value>* getParent() const;
	virtual Node<Key, Value>* getLeft() const;
	virtual Node<Key, Value>* getRight() const;

	void setParent(Node<Key, Value>* parent);
	void setLeft(Node<Key, Value>* left);
	void setRight(Node<Key, Value>* right);
	void setValue(const Value &value);

protected:
	std::pair<Key, Value> mItem;
	Node<Key, Value>* mParent;
	Node<Key, Value>* mLeft;
	Node<Key, Value>* mRight;
};

/*
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
	: mItem(key, value)
	, mParent(parent)
	, mLeft(NULL)
	, mRight(NULL)
{

}

template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<Key, Value>& Node<Key, Value>::getItem() const
{
	return mItem;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& Node<Key, Value>::getItem()
{
	return mItem;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
	return mItem.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
	return mItem.second;
}

/**
* A non-const getter for the key.
*/
template<typename Key, typename Value>
Key& Node<Key, Value>::getKey()
{
	return mItem.first;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
	return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
	return mParent;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
	return mLeft;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
	return mRight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
	mParent = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
	mLeft = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
	mRight = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
	mItem.second = value;
}

/*
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
	public:
		BinarySearchTree(); //TODO
		virtual ~BinarySearchTree(); //TODO
  		virtual void insert(const std::pair<Key, Value>& keyValuePair); //TODO
        virtual void remove(const Key& key); //TODO
  		void clear(); //TODO
  		void print() const;
  		bool isBalanced() const; //TODO

	public:
		/**
		* An internal iterator class for traversing the contents of the BST.
		*/
		class iterator
		{
			public:
				iterator(Node<Key,Value>* ptr);
				iterator();

				std::pair<Key,Value>& operator*() const;
				std::pair<Key,Value>* operator->() const;

				bool operator==(const iterator& rhs) const;
				bool operator!=(const iterator& rhs) const;
				iterator& operator=(const iterator& rhs);

				iterator& operator++();

			protected:
				Node<Key, Value>* mCurrent;

				friend class BinarySearchTree<Key, Value>;
		};

	public:
		iterator begin() const;
		iterator end() const;
		iterator find(const Key& key) const;

	protected:
		Node<Key, Value>* internalFind(const Key& key) const; //TODO
		Node<Key, Value>* getSmallestNode() const; //TODO
		void printRoot (Node<Key, Value>* root) const;

	protected:
		Node<Key, Value>* mRoot;

	public:
		void print() {this->printRoot(this->mRoot);}
	private:
		// Helper function for isBalenced()
		int height(Node<Key,Value>* node) const;
};

/*
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
	: mCurrent(ptr)
{

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
	: mCurrent(NULL)
{

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const
{
	return mCurrent->getItem();
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const
{
	return &(mCurrent->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key, Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
	this->mCurrent = rhs.mCurrent;
	return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
	if(mCurrent->getRight() != NULL)
	{
		mCurrent = mCurrent->getRight();
		while(mCurrent->getLeft() != NULL)
		{
			mCurrent = mCurrent->getLeft();
		}
	}
	else if(mCurrent->getRight() == NULL)
	{
		Node<Key, Value>* parent = mCurrent->getParent();
		while(parent != NULL && mCurrent == parent->getRight())
		{
			mCurrent = parent;
			parent = parent->getParent();
		}
		mCurrent = parent;
	}
	return *this;
}

/*
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/*
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
	mRoot = nullptr;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
	clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& key) const
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/*
* An insert method to insert into a Binary Search Tree. The tree will not remain balanced when
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	Node<Key,Value> * newNode;
	if(mRoot == nullptr)
	{
		newNode = new Node<Key,Value>(keyValuePair.first,keyValuePair.second, nullptr);
		mRoot = newNode;
	}
	else
	{
		Node<Key, Value> * tempLoc = mRoot;
		Node<Key, Value> * tempParent;
		while(tempLoc != nullptr)
		{
			tempParent = tempLoc;
			if(keyValuePair.first <= tempLoc->getKey())
				tempLoc = tempLoc->getLeft();
			else
				tempLoc = tempLoc->getRight();
		}
		newNode = new Node<Key,Value>(keyValuePair.first,keyValuePair.second,tempParent);
		if(keyValuePair.first <= tempParent->getKey())
			tempParent->setLeft(newNode);	
		else
			tempParent->setRight(newNode);
	}
}

/**
* An remove method to remove a specific key from a Binary Search Tree.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
	Node<Key,Value>* delNode = internalFind(key);
	Node<Key,Value>* replacementNode;
	if(delNode != nullptr)
	{
		// Selects the movement of another node if needed
		if((delNode->getLeft() == nullptr) && (delNode->getRight() == nullptr))
			replacementNode = nullptr;
		else if((delNode->getLeft() == nullptr) && (delNode->getRight() != nullptr))
		{
			replacementNode = delNode->getRight();
			replacementNode->setParent(delNode->getParent());
		}
		else
		{
			replacementNode = delNode->getLeft();
			while(replacementNode->getRight() != nullptr)
			{
				replacementNode = replacementNode->getRight();
			}
			if(delNode->getRight() != nullptr)
			{
				replacementNode->setRight(delNode->getRight());
				delNode->getRight()->setParent(replacementNode);
			}
			if(replacementNode->getParent() != delNode)
			{
				replacementNode->getParent()->setRight(replacementNode->getLeft()); 
				replacementNode->setLeft(delNode->getLeft());
				delNode->getLeft()->setParent(replacementNode);
			}
			replacementNode->setParent(delNode->getParent());
		}
		// Makes sure the parent connection is correctly set up
		if(delNode != mRoot)
		{
			if(delNode->getParent()->getLeft() == delNode)
				delNode->getParent()->setLeft(replacementNode);
			else
				delNode->getParent()->setRight(replacementNode);
		}
		else
			mRoot = replacementNode;
		delete delNode;
	}
}

/**
* A method to remove all contents of the tree and reset the values in the tree
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	if(mRoot != nullptr)
	{
		Node<Key,Value>* val;
		std::stack<Node<Key,Value>*> stack1;
		std::stack<Node<Key,Value>*> stack2;
		stack1.push(mRoot);
		while(!stack1.empty())
		{
			val = stack1.top();
			stack1.pop();
			stack2.push(val);
			if(val->getLeft() != nullptr)
				stack1.push(val->getLeft());
			if(val->getRight() != nullptr)
				stack1.push(val->getRight());
		}
		while(!stack2.empty())
		{
			val = stack2.top();
			stack2.pop();
			if(val != mRoot)
			{
				if(val->getParent()->getLeft() == val)
					val->getParent()->setLeft(nullptr);
				else
					val->getParent()->setRight(nullptr);
			}
			delete val;
		}
		mRoot = nullptr;
	}
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
	Node<Key, Value> * temp = mRoot;
	while(temp->getLeft() != nullptr)
		temp = temp->getLeft();
	return (temp);
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
	Node<Key, Value> * temp = mRoot;
	while(temp != nullptr)
	{
		if(key == temp->getKey())
			return temp;
		else if(key < temp->getKey())
			temp = temp->getLeft();
		else 
			temp = temp->getRight();
	}
	return nullptr;
}

// Helper function for isBalenced()
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::height(Node<Key,Value> *node) const
{
	if(node == nullptr)
		return 0;
	else
	{
		int leftHeight = height(node->getLeft());
		int rightHeight = height(node->getRight());
		if((leftHeight == -1) || (rightHeight == -1) || (abs(leftHeight - rightHeight) > 1))
			return -1;
		int maxVal;
		if(leftHeight > rightHeight)
			maxVal = leftHeight;
		else
			maxVal = rightHeight;
		return 1 + maxVal;
	}
}


/**
 * Return true iff the BST is an AVL Tree.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
	if(mRoot == nullptr)
		return true;
	return (height(mRoot) != -1);
}

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif
