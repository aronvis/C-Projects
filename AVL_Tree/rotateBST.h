#include "bst.h"

template <typename Key, typename Value>
class rotateBST: public BinarySearchTree<Key,Value>
{
public:
	bool sameKeys(const rotateBST& t2) const;
	void transform(rotateBST& t2) const;

protected:
	void leftRotate(Node<Key, Value>* r);
	void rightRotate(Node<Key, Value>* r);
};

template <typename Key, typename Value>
void rotateBST<Key, Value>::leftRotate(Node<Key, Value>* r)
{
	if((r != nullptr) && (r->getRight() != nullptr))
	{
		Node<Key, Value>* temp = r->getRight();
		r->setRight(temp->getLeft());
		if(temp->getLeft() != nullptr)
			temp->getLeft()->setParent(r);
		temp->setParent(r->getParent());
		r->setParent(temp);
		temp->setLeft(r);
		if(r == this->mRoot)
			this->mRoot = temp;
		else
		{
			if(temp->getParent()->getRight() == r)
				temp->getParent()->setRight(temp);
			else
				temp->getParent()->setLeft(temp);
		}
	}
}

template <typename Key, typename Value>
void rotateBST<Key, Value>::rightRotate(Node<Key, Value>* r)
{
	if((r != nullptr) && (r->getLeft() != nullptr))
	{
		Node<Key, Value>* temp = r->getLeft();
		r->setLeft(temp->getRight());
		if(temp->getRight() != nullptr)
			temp->getRight()->setParent(r);
		temp->setParent(r->getParent());
		r->setParent(temp);
		temp->setRight(r);
		if(r == this->mRoot)
			this->mRoot = temp;
		else
		{
			if(temp->getParent()->getLeft() == r)
				temp->getParent()->setLeft(temp);
			else
				temp->getParent()->setRight(temp);
		}
	}
}

template <typename Key, typename Value>
bool rotateBST<Key, Value>::sameKeys(const rotateBST& t2) const
{
	std::vector<Key> val1;
	std::vector<Key> val2;
	typename BinarySearchTree<Key, Value>::iterator it;
	for(it = this->begin(); it != this->end(); ++it)
	{
		val1.push_back(it->first);
	}
	for(it = t2.begin(); it != t2.end(); ++it)
	{
		val2.push_back(it->first);
	}
	return (val1 == val2);
}

template <typename Key, typename Value>
void rotateBST<Key, Value>::transform(rotateBST& t2) const
{
	if((sameKeys(t2) == true) && (t2.mRoot != nullptr) && (this->mRoot != nullptr))
	{
		Node<Key, Value>* node = t2.mRoot;
		int dist = 0;
		while((node->getLeft() != nullptr) || (node->getRight() != nullptr))
		{
			if(node->getLeft() != nullptr)
			{
				t2.rightRotate(node);
				node = t2.mRoot;
				for(int i=0; i<dist; i++)
					node = node->getRight();
			}
			else
			{
				node = node->getRight();
				dist++;
			}
		}
		node = t2.mRoot;
		Node<Key, Value>* node1 = this->mRoot;
		while((node != node1) && (node->getRight() != nullptr))
		{
			t2.leftRotate(t2.mRoot);
			node = t2.mRoot;
			node1 = this->mRoot;
		}
	}
}
