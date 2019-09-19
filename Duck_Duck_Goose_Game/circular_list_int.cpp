#include "circular_list_int.h"

// standard no-argument constructor
CircularListInt::CircularListInt()
{}

// Destructor. Should delete all data allocated by the list. 
CircularListInt::~CircularListInt()
{
	while(!empty())
   	{
   		remove(0);
   	}
   	
}

// Gets item at an index.
// If an index is passed that is larger then the number of items in the list, it should "wrap around" back to the first element.
// If there are no elements in the list, returns 0.
int CircularListInt::get(size_t index) const
{
	if(count == 0)
		return 0;
	else
		return (findItem(index)->value);
}

// get length of list.
size_t CircularListInt::size() const
{
	return count;
}


// returns true iff the list is empty.
bool CircularListInt::empty() const
{
	if(count == 0)
		return true;
	else 
		return false;
}

// Inserts (a copy of) the given item at the end of list.
void CircularListInt::push_back(int value)
{
	Item * node = new Item(value);
	if(count== 0)
	{
		head = node;
		head->prev = head;
		head->next = head;
	}
	else
	{
		node->prev = head->prev;
		node->next = head;
		head->prev = node;
		(node->prev)->next = node;
	}
	count++;
}

// Sets the item at the given index to have the given value.
// If an index is passed that is >= the number of items in the list, it should "wrap around" back to the first element.
void CircularListInt::set(size_t index, int value)
{
	if(count > 0)
	{
		Item * node = findItem(index);
		node->value = value;
	}
}

// Removes the item at the given index from the list.
// List elements after the removed element are pulled forward, so their indicies decrease by one.
// If an index is passed that is larger then the number of items in the list, it should "wrap around" back to the first element.
void CircularListInt::remove(size_t index)
{
	if(count > 0)
	{
		Item * node = findItem(index);
		(node->prev)->next = node->next;
		(node->next)->prev = node->prev;
		if(index % count == 0)
		{
			head = node->next;
		}
		delete node; 
		count--;
	}
}
