#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>


/*
		parent
	   /	  \
	lower 	  higher
	value     value
*/
namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

class Node{
	value_type element; 
	Node* left, right, parent;
	int balance;
	
	Node(key_type key, mapped_type value, Node* left = nullptr,
		Node* right = nullptr, Node* parent = nullptr):
		element(key, value), left(left), right(right), parent(parent),
		balance(0){}
};

Node* root;
size_type treeSize;

//rotations
void RR(Node * A)
{
	Node * B = A->right;
	Node * top = A->parent;
	
	A->right = B->left;
	
	if (a->right)
		A->right->parent = A;
	
	B->left = A;
	B->parent = top;
	A->parent = B;
	
	if (top){//if there is a parent we have to 
			//change its childs pointer
		if (top->left == A)
			top->left = B;
		else
			top->right = B;
	}
	else
		root = B;
	
	if (B->balance == -1){
		A->balance = 0;
		B->balance = 0;
	}
	else{
		A->balance = -1;
		B->balance = 1;
	}
}
void LL(Node * A)
{
	Node * B = A->left;
	Node * top = A->parent;
	
	A->left = B->right;
	if (A->left)
			A->left->up = A;
		
	B->right = A;
	B->up = top;
	A->up = B;
	
	if (top){
		if (top->left == A)
			top->left = B;
		else
			top->right = B;
	}
	else 
		root = B;
	
	if (B->balance == 1){
		A->balance = 0;
		B->balance = 0;
	}
	else{
		A->balance = 1;
		B->balance = -1;
	}
}

void LR(Node * A)
{
	Node * B = A->left;
	Node * C = B->right;
	Node * top = A->parent;
	
	B->right = C->left;
	if (B->right)
		B->right->parent = B;
	A->left = C->right;
	if (A->left)
		A->left->parent = A;
	C->right = A;
	C->left = B;
	A->parent = C;
	B->parent = C;
	C->parent = top;
	if (top){
		if (top->left == A)
			top->left = C;
		else
			top->right = C;
	}
	else
		root = C;
	A->balance = (C->balance == 1) ? -1 : 0;
	B->balance = (C->balance == -1) ? 1 : 0;
	C->balance = 0;
}
void RL(Node * A)
{
	Node * B = A->right;
	Node * C = B->left;
	Node * top = A->parent;
	
	B->left = C->right;
	if (B->left)
		A->right->parent = A;
	A->right = C->left;
	if (A->right)
		A->right->parent = A;
	C->left = A;
	C->right = B;
	A->parent = C;
	B->parent  = C;
	C->parent = top;
	if (top){
		if (top->left == A)
			top->left = C;
		else
			top->right = C;
	}
	else
		root = C;
	
	A->balance = (C->balance == -1) ? 1 : 0;
	B->balance = (C->balance == 1) ? -1 : 0;
	C->balance = 0;
	
} 

void clearFromGivenNode(Node * node)
{
	if (!node)
		return;
	clearFromGivenNode(node->left);
	clearFromGivenNode(node->right);
	if (root == node)
		root = nullptr;
	delete node;
	treeSize--;
}
Node* minimum(Node * parent)
{
	if (isEmpty())
		return nullptr;
	while(parent->left)
		parent = parent->left;
	return parent;
}
Node* maximum(Node * parent)
{
	if (isEmpty())
		return nullptr;
	while(parent->right)
		parent = parent->right;
	return parent;
}



  TreeMap():root(nullptr), treeSize(0)
  {}

  TreeMap(std::initializer_list<value_type> list)
  {
    treeSize = 0;
	root = nullptr;
	for (auto &element: list)
		this->operator[](element.first) = element.second;
  }

  TreeMap(const TreeMap& other)
  {
    root = nullptr;
	treeSize = 0;
	for (auto &element: other)
		this->operator[](element.first) = element.second;
  }

  TreeMap(TreeMap&& other)
  {
    root = other.root;
	treeSize = other.treeSize;
	other.treeSize = 0;
	other.root = nullptr;
  }
  
  ~TreeMap()
  {
	clearFromGivenNode(root);
  }
  
  TreeMap& operator=(const TreeMap& other)
  {
    if (this == &other) 
		return *this;
	
	clearFromGivenNode(root);
	root = other.root;
	treeSize = other.treeSize;
	return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if (this == &other)
		return *this;
	
	clearFromGivenNode(root);
	root = other.root;
	treeSize = other.treeSize;
	other.root = nullptr;
	other.treeSize = 0;
	return *this;
  }

  bool isEmpty() const
  {
	return treeSize == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
	Node * temp = root;
	Node * prev = nullptr;
	
	while(temp != nullptr)
	{
		prev = temp;
		if (key == temp->element.first)//node with key already exists 
			return temp->element.second;
		
		if (key > temp->element.first)
			temp = temp->right;
		else
			temp = temp->left;
	}
	Node* newNode = new Node(key, mapped_type{});
	if (!prev) //tree was empty
		root = newNode;
		
	if (key > prev->element.first){
		prev->right = newNode;
		rebalance(prev, -1);
	}
	else{
		prev->left = newNode;
		rebalance(prev, 1);
	}
	
	newNode->parent = prev;
	treeSize++;
	return newNode->element.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
	const_iterator iter = find(key);
	if (iter == cend())
		throw std::out_of_range("key not found");
	return iter.operator*().second;
  }

  mapped_type& valueOf(const key_type& key)
  {
	iterator iter = find(key);
	if (iter == end())
		throw std::out_of_range("key not found");
	return iter.operator*().second;
  }

  const_iterator find(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  iterator find(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  bool operator==(const TreeMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    throw std::runtime_error("TODO");
  }

  iterator end()
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  explicit ConstIterator()
  {}

  ConstIterator(const ConstIterator& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
    throw std::runtime_error("TODO");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_MAP_H */



//funkcje do poprawienia
//kod sie powtarza
//TO DO - nowa funkcja obslugujaca /**/
/*void rotateRight(Node * grandParent)
{
	if(!grandParent)
		throw std::logic_error("rotation impossible");
	auto temp = grandParent->left; //rotating around this node
	grandParent->left = temp->right
	if(!(temp->right))
		temp->right->parent = grandParent;
/*	if(grandParent == root)
		root = temp;
	else{ //if grandparent has a parent
		if(grandParent == grandParent->parent->right)
			grandParent->parent->right = temp;
		else
			grandParent->parent->left = temp;
/*<	}
	temp-right = grandParent;
/**	temp->parent = grandParent->parent;
/**	grandParent->parent = temp;
}/*
void rotateLeft(Node * grandParent)
{
	if(!grandParent)
		throw std::logic_error("rotation impossible");
	auto temp = grandParent->right;//rotating around this node
	grandParent->right = temp->left;
	if(!(temp->left)) 
		temp->right->parent = grandParent;
	if(grandParent == root)
		root = temp;
	else{ //if grandparent has a parent
		if(grandParent == grandParent->parent->left)
			grandParent->parent->left = temp;
		else
			grandParent->parent->right = temp;
	}
	temp->left = grandParent;
	temp->parent = grandParent->parent;
	grandParent;
}

void rightLeftRotate(Node* grandParent)
{
	rotateLeft(grandParent->right);
	rotateRight(grandParent);
}
void leftRightRotate(Node* grandParent)
{
	rotateRight(grandParent->left);
	rotateLeft(grandParent);
}
void rebalance(Node * temp, int difference)
{
	int currBalance = temp->balance + difference;
	
	if(currBalance > 1){
		if(temp->left->balance > 0){ // RR
			temp->balance -= 2;
			temp->left->balance -= 1;
			rotateRight(temp);
		}
		else{// LR
			if(temp->left->right->balance > 0)
				temp->balance = -1;
			else
				temp->balance = 0;
			}
		temp->left->right->balance = 0;
		leftRightRotate(temp);
	}
	else if(currBalance < -1){
		
	}
	else{//if currBalance in {-1, 0, 1}
		//current node is balanced
		//we go higher
		
	}
}*/