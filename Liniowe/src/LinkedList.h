#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    class Node
    {
    public:
        size_type value;
        Node * prev;
        Node * next;
        Node()
        {
            value = 0;
            prev = nullptr;
            next = nullptr;
        }
        Node(const Type &item)
        {
            prev = nullptr;
            next = nullptr;
            value = item;
        }
    };

    Node *head;
    Node *tail;
    size_type _size;


    LinkedList()
    {
        this->_size = 0;
        this->head = nullptr;
        this->tail= nullptr;
    }

    LinkedList(std::initializer_list<Type> l)
    {
        for(auto it = l.begin(); it!=l.end(); it++)
        {
            this->append(*it);
        }
    }
    void swapList(LinkedList<Type> &a, LinkedList<Type> &b)
    {
        std::swap(a.head,b.head);
        std::swap(a._size,b._size);
        std::swap(a.tail,b.tail);
    }
    LinkedList(const LinkedList& other):LinkedList()
    {
        for(auto it = other.begin; it != other.end(); ++it)
        {
            this->append(*it);
        }
    }

    LinkedList(LinkedList&& other):LinkedList()
    {
        swapList(*this, other);
    }

    ~LinkedList()
    {
        if(this->root != nullptr)
        {
            this->erase(this->begin(), this->end());
        }
    }

    LinkedList& operator=(const LinkedList& other)
    {
        swapList(*this, other);
        return *this;
    }

    LinkedList& operator=(LinkedList&& other)
    {

    }

    bool isEmpty() const
    {
        return ((int) this->_size == 0);
    }

    size_type getSize() const
    {
        return this->_size;
    }

    void append(const Type& item)
    {
        Node *newNode = new Node(item);
        newNode->prev = this->tail;
        newNode->next = nullptr;
        if(this->head == nullptr)
        {
            this->head = newNode;
            this->tail = newNode;
            this->_size++;
            return;
        }
        this->tail->next = newNode;
        this->tail = newNode;
        this->_size++;
        return;
    }

    void prepend(const Type& item)
    {
        Node *newNode;
        newNode = new Node(item);
        newNode->prev = nullptr;
        newNode->next = this->head;
        this->head->prev = newNode;
        this->head = newNode;
        if(this->head == nullptr)
        {
            this->tail = newNode;
        }
        this->_size++;
        return;
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        Node *newNode = new Node(item);
        Node *insertPosition = insertPosition.node;

        if(insertPosition.node == nullptr)
        {
            this->append(item);
        }
        else if(insertPosition.node == head)
        {
            this->prepend(item);
        }
        else
        {
            newNode->next = insertPosition;
            newNode->prev = insertPosition->prev;
            insertPosition->prev->next = newNode;
            insertPosition->prev = newNode;
            ++_size;
        }

    }

    Type popFirst()
    {
        if(this->head == nullptr) throw std::out_of_range("");

        Node *popped = this->head;
        Type returnVal = popped->value;

        this->head = this->head->next;

        if(this->head != nullptr)
        {
            this->head->prev = nullptr;
        }
        delete popped;
        this->_size--;

        return returnVal;

    }

    Type popLast()
    {
        if(this->head == nullptr) throw std::out_of_range("");
        else if(this->head == this->last)
        {
            return this->popFirst();
        }
        Type returnVal = this->last->value;
        Node *temp = this->last->prev;
        temp->next = nullptr;
        delete this->tail;
        this->tail = temp;
        this->_size--;
        return returnVal;
    }

    void erase(const const_iterator& possition)
    {
        if(possition.node == nullptr) throw std::out_of_range("");
        if(possition.node == this->head)
        {
            popFirst();
            return;
        }
        else if(possition.node == this->tail)
        {
            popLast();
            return;
        }
        else
        {
            possition.node->prev->next = possition.node->next;
            possition.node->next->prev = possition.node->prev;
            delete possition.node;
            --this->_size;
            return;
        }
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        Node *temp = firstIncluded.node;
        Node *tempPrev = firstIncluded.node->prev;
        Node *tempNext;

        while(temp != lastExcluded.node)
        {
            tempNext = temp->next;
            delete temp;
            temp = tempNext;
            --(this->_size);
        }

        if(temp != nullptr)
        {
            temp-prev = tempPrev;
        }
        else
        {
            this->tail = nullptr;
        }
        if(tempPrev != nullptr)
        {
            tempPrev->next = temp;
        }
        else
        {
            this->head = temp;
        }
    }

    iterator begin()
    {
        if((int)this->_size != 0) return end();
        else return Iterator(ConstIterator(this, this->head));
    }

    iterator end()
    {
        return Iterator(ConstIterator(this, nullptr))
    }

    const_iterator cbegin() const
    {
        return ConstIterator(this, this->head);
    }

    const_iterator cend() const
    {
        return ConstIterator(this, nullptr);
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

template <typename Type>
class LinkedList<Type>::ConstIterator
{
    const LinkedList *lista;
    Node *node

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    explicit ConstIterator()
    {
        lista = nullptr;
        node = nullptr;
    }

    reference operator*() const
    {
        return this->node->value;
    }

    ConstIterator& operator++()
    {
        if(this->node == nullptr || this->node->next == nullptr) throw std::out_of_range("");
        this->node = this-node->next;
        return *this
    }

    ConstIterator operator++(int)
    {
        ConstIterator it(this->lista, this->node);
        it->operator++();
        return it;
    }

    ConstIterator& operator--()
    {
        if (this->node == nullptr || this->node->prev == nullptr) throw std::out_of_range("");
        this->node = this->node->prev;
        return *this;
    }

    ConstIterator operator--(int)
    {
        ConstIterator it(this->lista, this->node);
        it->operator--();
        return it;
    }

    ConstIterator operator+(difference_type d) const
    {
        Node *temp = this->node;
        for (int i = 0; i < d; ++i)
        {
            if (temp->next == nullptr) throw std::out_of_range("");
            temp = temp->next;
        }
        ConstIterator it(this->list, temp);
        return it;
    }

    ConstIterator operator-(difference_type d) const
    {
        Node *temp = this->node;
        for (int i = 0; i < -d; ++i)
        {
            if (temp->prev == nullptr) throw std::out_of_range("");
            temp = temp->prev;
        }
        ConstIterator it(this->list, temp);
        return it;
    }
}

bool operator==(const ConstIterator& other) const
{
    return (this->list == other.list && this->node == other.node);
}

bool operator!=(const ConstIterator& other) const
{
    return !this->operator==(other);
}
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

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

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
