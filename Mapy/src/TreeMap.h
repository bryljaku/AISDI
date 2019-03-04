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

    class Node
    {
    public:
        value_type element;
        Node  *left, *right, *parent;
        int balance;

        Node(key_type key, mapped_type value, Node* left = nullptr,
             Node* right = nullptr, Node* parent = nullptr):
            element(key, value), left(left), right(right), parent(parent),
            balance(0) {}
    };

    Node* root;
    size_type treeSize;

    void rebalance(Node *n)
    {
        if (!n)
            return;
        setBalance(n);

        if (n->balance == -2)
        {
            if (height(n->left->left) >= height(n->left->right))
                n = rotateRight(n);
            else
                n = rotateLeftThenRight(n);
        }
        else if (n->balance == 2)
        {
            if (height(n->right->right) >= height(n->right->left))
                n = rotateLeft(n);
            else
                n = rotateRightThenLeft(n);
        }

        if (n->parent)
            rebalance(n->parent);
        else
            root = n;
    }

    Node* rotateLeft(Node *a)
    {
        Node *b = a->right;
        b->parent = a->parent;
        a->right = b->left;

        if (a->right != NULL)
            a->right->parent = a;

        b->left = a;
        a->parent = b;

        if (b->parent != NULL)
        {
            if (b->parent->right == a)
                b->parent->right = b;
            else
                b->parent->left = b;
        }

        setBalance(a);
        setBalance(b);
        return b;
    }

    Node* rotateRight(Node *a)
    {
        Node *b = a->left;
        b->parent = a->parent;
        a->left = b->right;

        if (a->left != NULL)
            a->left->parent = a;

        b->right = a;
        a->parent = b;

        if (b->parent != NULL)
        {
            if (b->parent->right == a)
                b->parent->right = b;
            else
                b->parent->left = b;
        }
        setBalance(a);
        setBalance(b);
        return b;

    }

    Node* rotateLeftThenRight(Node *n)
    {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }

    Node* rotateRightThenLeft(Node *n)
    {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }

    int height(Node *n)
    {
        if (!n)
            return -1;
        return 1 + std::max(height(n->left), height(n->right));
    }

    void setBalance(Node *n)
    {
        n->balance = height(n->right) - height(n->left);
    }

    Node* treeSuccessor(Node *node) const //find first node with higher key
    {
        if(!node)
            return node;
        if(node->right)
            return minimum(node->right);

        Node *B = node->parent;
        while(B && node == B->right)
        {
            node = B;
            B = B->parent;
        }
        return B;
    }
    Node* treePredecessor(Node *node) const //find first node with lower key
    {
        if(!node)
            return node;

        if(!node->left)
            return maximum(node->left);

        Node *B = node->parent;

        while(!B && node == B->left)
        {
            node = B;
            B = B->parent;
        }
        return B;
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
    Node* minimum(Node * parent) const
    {
        if (isEmpty())
            return nullptr;
        while(parent->left)
            parent = parent->left;
        return parent;
    }
    Node* maximum(Node * parent) const
    {
        if (isEmpty())
            return nullptr;
        while(parent->right)
            parent = parent->right;
        return parent;
    }

    Node* findNode(Node *node, key_type key ) const
    {
        while(node && key != node->element.first)
            node = (node->element.first < key) ? node->left : node->right;
        return node;
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
        Node* newNode = new Node(key, mapped_type {});
        if (!prev) //tree was empty
            root = newNode;

        if (key > prev->element.first)
        {
            prev->right = newNode;
            rebalance(prev);
        }
        else
        {
            prev->left = newNode;
            rebalance(prev);
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
        ConstIterator it;
        it.tree = this;
        it.node = findNode(root, key);
        return it;
    }

    iterator find(const key_type& key)
    {
        Iterator it;
        it.tree = this;
        it.node = findNode(root, key);
        return it;
    }
    void remove(const key_type& key)
    {
        remove(find(key));
    }

    void remove(const const_iterator& it)
    {
        if (it == end())
            throw std::out_of_range("Element does not exist");

        Node* toRemove = it.node;
        Node* parentOfToRemove = toRemove->parent;
        if (!parentOfToRemove)
            root = nullptr;
        else if (!(toRemove->left) && !(toRemove->right))//delete leaf
        {
            if (parentOfToRemove->left == toRemove)
                parentOfToRemove->left = nullptr;
            else
                parentOfToRemove->right = nullptr;
            rebalance(parentOfToRemove);
        }
        else if (!(toRemove->left) && toRemove->right)//right child
        {
            if (!parentOfToRemove)
                root = toRemove->right;
            else if (parentOfToRemove->left == toRemove)
                parentOfToRemove->left = toRemove->right;
            else
                parentOfToRemove->right = toRemove->right;
            toRemove->right->parent = parentOfToRemove;
            rebalance(parentOfToRemove);
        }
        else if (toRemove->left && !(toRemove->right))//left child
        {
            if (!parentOfToRemove)
                root = toRemove->left;
            else if (parentOfToRemove->left == toRemove)
                parentOfToRemove->left = toRemove->left;
            else
                parentOfToRemove->right = toRemove->left;
            toRemove->left->parent = parentOfToRemove;
            rebalance(parentOfToRemove);
        }
        else //two children
        {
            Node* newParentLeft = minimum(toRemove->right);

            if (!parentOfToRemove)
                root = toRemove->right;
            else if (parentOfToRemove->left == toRemove)
                parentOfToRemove->left = toRemove->right;
            else
                parentOfToRemove->right = toRemove->right;

            newParentLeft->left = toRemove->left;
            toRemove->left->parent = newParentLeft;
            rebalance(newParentLeft);

        }
        treeSize--;
        delete toRemove;
    }

    size_type getSize() const
    {
        return treeSize;
    }

    bool operator==(const TreeMap& other) const
    {
        if(treeSize != other.treeSize)
            return false;
        const_iterator iterator1 = begin();
        const_iterator iterator2 = other.begin();

        while (iterator1 != end() )
        {

            if((*iterator1).first != (*iterator2).first || (*iterator1).second != (*iterator2).second)
                return false;
            iterator1++;
            iterator2++;
        }
        return true;
    }

    bool operator!=(const TreeMap& other) const
    {
        return !(*this == other);
    }
    iterator begin()
    {
        Iterator it;
        it.tree = this;
        it.node = minimum(root);

        return it;
    }

    iterator end()
    {
        Iterator it;
        it.tree = this;
        it.node = minimum(root);
        return it;
    }

    const_iterator cbegin() const
    {
        ConstIterator it;
        it.tree = this;
        it.node = minimum(root);

        return it;
    }

    const_iterator cend() const
    {
        ConstIterator it;
        it.tree = this;
        it.node = nullptr;

        return it;
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

    Node *node;
    const TreeMap *tree;

    explicit ConstIterator()
    {}


    ConstIterator(const ConstIterator& other): node(other.node), tree(other.tree) {}

    ConstIterator& operator++()//preincrementation
    {
        if(!node)
            throw std::out_of_range("operator++ nullptr");
        node = tree->treeSuccessor(node);
        return *this;
    }

    ConstIterator operator++(int)//postincrementation
    {
        if(!node)
            throw std::out_of_range("operator++ nullptr");
        ConstIterator temp;
        temp.node = node;
        temp.tree = tree;
        node = tree->treeSuccessor(node);
        return temp;
    }

    ConstIterator& operator--()
    {
        if(*this == tree->begin())
            throw std::out_of_range("operator--");
        if(!node)
        {
            node = tree->maximum(tree->root);
            return *this;
        }
        node = tree->treePredecessor(node);
        return *this;
    }

    ConstIterator operator--(int)
    {
        if(*this == tree->begin())
            throw std::out_of_range("Tree operator-- POST");

        ConstIterator temp;
        temp.node = node;
        temp.tree = tree;

        if(node == nullptr)
        {
            node = tree->maximum(tree->root);
            return temp;
        }
        node = tree->treePredecessor(node);
        return temp;
    }

    reference operator*() const
    {
        if(!node)
            throw std::out_of_range("wrong reference");
        return node->element;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {
        if(tree != other.tree)
            return false;
        if(!node || !(other.node))
        {
            if(!node && !(other.node))
                return true;
            else
                return false;
        }
        else if (node->element == other.node->element)
            return true;
        else
            return false;
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
