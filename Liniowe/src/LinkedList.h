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

    private:
        class Node;
        Node* head;
        Node* tail;
        size_type listSize;

    public:
        void initializeListSentinels()
        {
            head = new Node;
            tail = new Node;
            head->prev = nullptr;
            head->next = tail;
            tail->prev = head;
            tail->next = nullptr;
            listSize = 0;
        }

        LinkedList()
        {
            initializeListSentinels();
        }

        LinkedList(std::initializer_list<Type> l)
        {
            initializeListSentinels();
            auto i = l.begin();
            while(i != l.end())
            {
                append(*i);
                i++;
            }
        }

        LinkedList(const LinkedList& other)
        {
            initializeListSentinels();
            const_iterator i;
            for(i = other.begin(); i!=other.end(); i++)
                append(*i);
        }

        LinkedList(LinkedList&& other)
        {
            listSize = other.listSize;
            head = other.head;
            tail = other.tail;
            other.listSize = 0;
            other.head = nullptr;
            other.tail = nullptr;
        }

        ~LinkedList()
        {
            while(head != nullptr)
            {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            tail = nullptr;
            listSize = 0;
        }


        LinkedList& operator=(const LinkedList& other)
        {

            if(this == &other)
                return *this;

            while(head != nullptr)
            {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            tail = nullptr;
            listSize = 0;

            initializeListSentinels();
            const_iterator i;
            for(i = other.begin(); i != other.end(); i++)
            {
                append(*i);
            }
            return *this;
        }

        LinkedList& operator=(LinkedList&& other)
        {

            if(this == &other)
                return *this;
            Node* position = head;
            while(position != nullptr)
            {
                Node* temp = position;
                position = position->next;
                delete temp;
            }
            listSize = other.listSize;
            head = other.head;
            tail = other.tail;
            other.listSize = 0;
            other.head = other.tail = nullptr;
            return *this;
        }

        bool isEmpty() const
        {
            return listSize == 0;
        }

        size_type getSize() const
        {
            return listSize;
        }

        void append(const Type& item)
        {
            Node* newNode = new Node;
            newNode->value = item;
            newNode->prev = tail->prev;
            newNode->next = tail;
            tail->prev->next = newNode; //when we append to an empty list tail->prev = head
            tail->prev = newNode;       // <- so it works
            listSize++;
        }

        void prepend(const Type& item)
        {
            Node* newNode = new Node;
            newNode->value = item;
            newNode->prev = head;
            newNode->next = head->next;
            head->next->prev = newNode;
            head->next = newNode;
            listSize++;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if(insertPosition == begin())
                prepend(item);

            else if(insertPosition == end())
                append(item);

            else
            {
            Node* newNode = new Node;
            newNode->value = item;
            newNode->prev = (insertPosition-1).current;
            newNode->next = insertPosition.current;
            (insertPosition-1).current->next = newNode;
            listSize++;
            }
        }
        Type popFirst()
        {
            if (isEmpty())
                throw std::out_of_range ("empty");

            Node* toRemove = head->next;
            value_type result = toRemove->value;
            head->next = toRemove->next;
            toRemove->next->prev = head;
            listSize--;
            delete toRemove;
            return result;
        }

        Type popLast()
        {
            if (isEmpty())
                throw std::out_of_range ("empty");

            value_type result = tail->prev->value;
            Node* toRemove = tail->prev;
            tail->prev = toRemove->prev;
            toRemove->prev->next = tail;
            listSize--;
            delete toRemove;
            return result;
        }

        void erase(const const_iterator& possition)
        {

            if (isEmpty())
                throw std::out_of_range("Nothing to be deleted");
            if (possition == end())
                throw std::out_of_range("Can't erase end position");

            possition.current->prev->next = possition.current->next;
            possition.current->next->prev = possition.current->prev;

            delete possition.current;
            listSize--;
        }

        void erase(const const_iterator& headIncluded, const const_iterator& tailExcluded)
        {
            headIncluded.current->prev->next = tailExcluded.current;
            tailExcluded.current->prev = headIncluded.current->prev;

            size_t counter = 0;
            auto it = headIncluded;
            Node* temp;
            while(it != tailExcluded)
            {
                temp = it.current->next;
                delete it.current;
                it.current = temp;
                counter++;
            }
            listSize -= counter;
        }

        iterator begin()
        {
            return Iterator(head->next);
        }

        iterator end()
        {
            return Iterator(tail);
        }

        const_iterator cbegin() const
        {
            return ConstIterator(head->next);
        }

        const_iterator cend() const
        {
            return ConstIterator(tail);
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
    class LinkedList<Type>::Node
    {
    public:
        Type value;
        Node *prev;
        Node *next;

        Node() {}
        ~Node() {}
    };
    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    private:
        Node* current;
    public:
        friend class LinkedList<Type>;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

    //    ConstIterator()= default;

        explicit ConstIterator(Node* nodePointer = nullptr)
        {
            current = nodePointer;
        }


        reference operator*() const
        {
            if(current->next == nullptr || current->prev == nullptr)
                throw std::out_of_range("OUT OF RANGE");
            else
                return current->value;
        }

        ConstIterator& operator++()
        {
            if(current->next == nullptr)
                throw std::out_of_range("OUT OF RANGE");
            else
                current = current->next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            if (current->next == nullptr)
                throw std::out_of_range("OUT OF RANGE");
            ConstIterator result = ConstIterator(current);
            current = current->next;
            return result;
        }

        ConstIterator& operator--()
        {
            if(current->prev->prev== nullptr)
                throw std::out_of_range("decrementing from beginning");
            current = current->prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            if(current->prev->prev== nullptr)
                throw std::out_of_range("decrementing from beginning");
            ConstIterator result = ConstIterator(current);
            current = current->prev;
            return result;
        }

        ConstIterator operator+(difference_type d) const
        {
            Node* temp = current;
            while(d > 0)
            {
                if(temp->next == nullptr)
                    throw std::out_of_range("OUT OF RANGE");
                temp = temp->next;
                d--;
            }
            return ConstIterator(temp);
        }

        ConstIterator operator-(difference_type d) const
        {
            Node* temp = current;
            while(d > 0)
            {
                if(temp->prev == nullptr)
                    throw std::out_of_range("OUT OF RANGE");
                temp = temp->prev;
                d--;
            }
            return ConstIterator(temp);
        }


        bool operator==(const ConstIterator& other) const
        {
            return current == other.current;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return current != other.current;
        }


    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(Node* node = nullptr) :
                ConstIterator(node)
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator(pointer other)
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
