//used as reference: https://kukuruku.co/post/avl-trees/


#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H
#include <stdio.h>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <stack>

namespace aisdi
{
    template<typename KeyType, typename ValueType>
    class TreeMap
    {
    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;


        class Node;

        size_t treeSize{0};
        Node* root{nullptr};

       

        void deleteTree()
        {
            deleteFromNode(root);
            root = nullptr;
        }

        void deleteFromNode(Node* node)
        {
            if (node) {
                deleteFromNode(node->left);
                deleteFromNode(node->right);
                --treeSize;
                delete node;
            }
        }

        Node* findMinNode(Node* node)
        {
            Node* currentNode = node;
            while (currentNode->left)
                currentNode = currentNode->left;

            return currentNode;
        }
        Node* findMaxNode(Node* node)
        {
            Node* currentNode = node;
            while(currentNode->right)
                currentNode = currentNode->right;

            return currentNode;
        }

        Node* rightRightRotation(Node* a)
        {
            Node* b = a->right;

            a->right = b->left;
            b->left = a;
            a->heightUpdate();
            b->heightUpdate();

            return b;
        }

        Node* leftLeftRotation(Node* a)
        {
            Node* b = a->left;

            a->left = b->right;
            b->right = a;
            a->heightUpdate();
            b->heightUpdate();

            return b;
        }

        Node* rightLeftRotation(Node* a)
        {
            a->right = leftLeftRotation(a->right);
            return rightRightRotation(a);
        }

        Node* leftRightRotation(Node* a)
        {
            a->left = rightRightRotation(a->left);
            return leftLeftRotation(a);
        }

        Node* insert(Node* node, value_type value)
        {
            if (!node){ 
                ++treeSize;
                return (new Node{value});
            }
            key_type key = value.first;

            if (key < node->getKey()) 
                node->left = insert(node->left, value);
            else
                node->right = insert(node->right, value);

            node->heightUpdate();
            return rebalance(node);
        }

        Node* rebalance(Node* node)
        {
            int balance = node->getBalance();

            if (balance > 1) //left child >
                return (node->left->getBalance() > 0) ? leftLeftRotation(node) : leftRightRotation(node);
            if (balance < -1) //right child >
                return (node->right->getBalance() < 0) ? rightRightRotation(node) : rightLeftRotation(node);

            return node;
        }

        Node* deleteNode(Node* node, key_type key)
        {
            if (!node)
                return node;

            if (key < node->getKey())
                node->left = deleteNode(node->left, key);
            else if (key > node->getKey())
                node->right = deleteNode(node->right, key);
            else {
                if (node->twoChildren()){// 2 children{
                    Node *minNode = findMinNode(node->right);
                    std::swap(node->value, minNode->value);
                     node->right = deleteNode(node->right, minNode->getKey());
                }
                else{// 1 or 0 children
                    Node *tmp = node;
                    node = (node->leftChild()) ? node->left : node->right;
                    delete tmp;
                }
                --treeSize;
            }
            if (!node)
                return nullptr;

            node->heightUpdate();
            return rebalance(node);
        }

    public:

        TreeMap()
        = default;

        TreeMap(std::initializer_list<value_type> list)
        {
            for (auto val : list)
                root = insert(root, val);
        }

        TreeMap(const TreeMap &other)
        {
            for (auto val : other)
                root = insert(root, val);
        }

        TreeMap(TreeMap &&other) : treeSize(other.treeSize), root(other.root)
        {
            other.root = nullptr;
            other.treeSize = 0;
        }

        ~TreeMap()
        {
            deleteFromNode(root);
            root = nullptr;
        }

        TreeMap &operator=(const TreeMap &other)
        {
            if(this != &other){
                deleteTree();
                for (auto val : other)
                    root = insert(root, val);
            }
            return *this;
        }

        TreeMap &operator=(TreeMap &&other)
        {
            if (this != &other){
                deleteTree();
                treeSize = other.treeSize;
                root = other.root;
                other.treeSize = 0;
                other.root = nullptr;
            }
            return *this;
        }

        bool isEmpty() const {return !treeSize;}

        mapped_type &operator[](const key_type &key)
        {
            auto it = find(key);
            if (it == end() || !it.current){
                root = insert(root, {key, mapped_type{} });
                return find(key)->second;
            }
            return it->second;
        }

        const mapped_type &valueOf(const key_type &key) const
        {
            if (isEmpty())
                throw std::out_of_range("No Tree");
            auto it = find(key);
            if (it == end())
                throw std::out_of_range("Key not found");

            return it->second;
        }

        mapped_type &valueOf(const key_type &key)
        {
            if (isEmpty())
                throw std::out_of_range("No Tree");
            auto it = find(key);
            if (it == end())
                throw std::out_of_range("Key not found");
            
            return it->second;
        }

        const_iterator find(const key_type &key) const
        {
            if (!root)
                return cend();

            Node* node = root;
            std::stack<Node*> parents;
            while (node->value->first != key){
                parents.push(node);
                if(key < node->value->first){
                    if(node->left)
                        node = node->left;
                    else
                        return ConstIterator(root, nullptr, parents);
                }
                else if (key > node->value->first){ /*tutaj był błąd, brakowalo "else" */
                    if (node->right)
                        node = node->right;
                    else
                        return ConstIterator(root, nullptr, parents);
                }
            }
            return ConstIterator(root, node, parents);
        }

        iterator find(const key_type &key)
        {
            return Iterator((const_cast<const TreeMap*>(this))->find(key));
        }

        void remove(const key_type &key)
        {
            if(!root)
                throw std::out_of_range("no tree");

            auto it = find(key);
            if(!it.current)
                throw std::out_of_range("there isn't value with that key");

            root = deleteNode(root, key);
        }

        void remove(const const_iterator &it)
        {
            remove(it->first);
        }

        size_type getSize() const
        {
            return treeSize;
        }

        bool operator==(const TreeMap &other) const
        {
            if (treeSize != other.treeSize)
                return false;

            auto endIt = end();
            for (auto val : other){
                auto it = find(val.first);

                if (it == endIt || val != *it)
                    return false;
            }
            return true;
        }

        bool operator!=(const TreeMap &other) const
        {
            return !(*this == other);
        }

        iterator begin()
        {
            return Iterator(cbegin());
        }

        iterator end()
        {
            return Iterator(cend());
        }

        const_iterator cbegin() const
        {
            if (!treeSize)
                return cend();

            std::stack<Node* > parents;
            Node* node = root;
            while (node->left){
                parents.push(node);
                node = node->left;
            }
            return ConstIterator{root, node, parents};
        }

        const_iterator cend() const
        {
            return ConstIterator{root, nullptr};
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }


        class Node
        {
            friend class TreeMap;

        private:
            value_type *value{nullptr};
            Node* left{nullptr};
            Node* right{nullptr};
            int height{1};

            bool twoChildren() const{ 
                return left && right; 
            }

            bool rightChild() const{
                return right; 
            }

            bool leftChild() const{
                return left; 
            }

        public:

            explicit Node(const value_type &value1)
            {
                value = new value_type{value1};
            }

            ~Node()
            {
                delete value;
            }

            const key_type &getKey() const
            {
                return value->first;
            }

            bool operator==(const Node &other)
            {
                return *value == *other.value;
            }

            bool operator!=(const Node &other)
            {
                return !(operator==(other));
            }

            void heightUpdate()
            {
                if (twoChildren())
                    height = 1 + std::max(left->height, right->height);
                else if (leftChild())
                   height = 1 + left->height;
                else if (rightChild())
                    height = 1 + right->height;
                else //doesn't have children
                    height = 1;
            }

            int getBalance()
            {
                if (twoChildren())
                    return (left->height - right->height);
                else if (leftChild())
                    return left->height;
                else if (rightChild())
                    return -1*right->height;
                else
                    return 0;
            }
            Node* max()
            {
                Node *temp = this;
                while (temp->right)
                    temp = temp->right;
                return temp;
            }

            Node* min()
            {
                Node *temp = this;
                while (temp->left)
                    temp = temp->left;
                return temp;
            }
            
            };//node
        
        
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename TreeMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename TreeMap::value_type;
        using pointer = const typename TreeMap::value_type *;
        using Node = typename TreeMap::Node;

        explicit ConstIterator()
        = default;

        friend class TreeMap;

    private:
        Node *root;
        Node *current;
        std::stack<Node *> parents;

        void travelToMin() {
            while (current->left) {
                parents.push(current);
                current = current->left;
            }
        }

        void travelToMax() {
            while (current->right) {
                parents.push(current);
                current = current->right;
            }
        }

    public:
        ConstIterator(Node *root, Node *current, std::stack<Node *> &parents)
                : root(root), current(current), parents(parents) {}

        ConstIterator(Node *root, Node *current) : root(root), current(current) {
            Node *tmp = root;
            if (current)
                while (tmp != current) {
                    parents.push(tmp);
                    tmp = (current->getKey() < tmp->getKey()) ? tmp->left : tmp->right;
                }
            else
                while (tmp != current) {
                    parents.push(tmp);
                    tmp = tmp->right;
                }

        }

        ConstIterator(const ConstIterator &other) : root(other.root), current(other.current), parents(other.parents) {
        }

        ConstIterator &operator++() {
            if (!root)
                throw std::out_of_range("Collection is empty");
            if (!current)
                throw std::out_of_range("out of range incrementing");
            if (current == root->max()) {
                parents.push(current);
                current = nullptr;
            }
            else if (current->rightChild()) { //has right son{
                parents.push(current);
                current = current->right;
                travelToMin();
            }
            else { //no right child, go up
                Node *tmp;
                do {
                    tmp = current;
                    current = parents.top();
                    parents.pop();
                } while (tmp == current->right); //travel parents
            }

            return *this;
        }

        const ConstIterator operator++(int)
        {
            auto it = *this;
            operator++();
            return it;
        }

        ConstIterator &operator--()
        {
            if (!root || current == root->min())
                throw std::out_of_range("--");

            if (!current){
                current = parents.top();
                parents.pop();
            }

           else if (current->rightChild()){
                parents.push(current);
                current = current->left;
                travelToMax();
            }
            else{
                Node* tmp;
                do{
                    tmp = current;
                    current = parents.top();
                    parents.pop();
                } while (tmp == current->left);
            }

            return *this;
        }

        const ConstIterator operator--(int)
        {
            auto it = *this;
            operator--();
            return it;
        }

        reference operator*() const
        {
            if (!root || !current)
                throw std::out_of_range("*");

            return *(current->value);
        }

        pointer operator->() const
        {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const
        {
            return (parents == other.parents && current == other.current);
        }

        bool operator!=(const ConstIterator &other) const
        {
            return !(*this == other);
        }
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
    {
    public:
        using reference = typename TreeMap::reference;
        using pointer = typename TreeMap::value_type *;

        explicit Iterator()
        = default;

        explicit Iterator(const ConstIterator &other)
                : ConstIterator(other)
        {}

        Iterator &operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        const Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        const Iterator operator--(int)
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
