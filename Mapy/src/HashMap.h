#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <list>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
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
private:
    struct HashNode
    {
        value_type element;
        HashNode *prev, *next;

        HashNode(key_type key, mapped_type mapped)
            :element(std::make_pair(key, mapped)), prev(nullptr), next(nullptr)
        {}
        HashNode(key_type key, mapped_type mapped, HashNode previous)
            :HashNode(key, mapped)
        {
            prev = previous;
        }
        ~HashNode()
        {
            if(next)
                delete next;
        }
    };

    HashNode* findNode(key_type key) const
    {
        HashNode* temp = tab[hashFun(key)];
        while(temp)
        {
            if (temp->element.first == key)
                return temp;
            temp = temp->next;
        }
        return temp;
    }



    void deleteList(size_type index)
    {
        HashNode* temp = tab[index];
        HashNode* temp1;
        while(temp)
        {
            temp1 = temp->next;
            //  temp->next = nullptr;
            //  temp->prev = nullptr;
            delete temp;
            temp = temp1;
        }
    }

    void deleteMap()
    {
        for (size_type i; i < HASH_TABLE_MAX_SIZE; i++)
        {
            deleteList(i);
            tab[i] = nullptr;
        }
        counter = 0;
    }

    size_type hashFun(key_type key) const
    {
        return key % HASH_TABLE_MAX_SIZE;
    }

    size_type findFirstNonEmpty() const
    {
        size_type index = 0;
        while(!tab[index] && index != HASH_TABLE_MAX_SIZE)
            index++;
        return index;
    }

public:

    static const  size_type HASH_TABLE_MAX_SIZE = 499;
    HashNode **tab;
    size_type counter;

    HashMap()
    {
        tab = new HashNode* [HASH_TABLE_MAX_SIZE] {};
        for (size_type i = 0; i < HASH_TABLE_MAX_SIZE; i++)
            tab[i] = nullptr;
    }
    ~HashMap()
    {
        if(tab)
        {
            deleteMap();
            delete [] tab;
        }
    }
    HashMap(std::initializer_list<value_type> list) : HashMap()
    {
        for(auto it = list.begin(); it != list.end(); it++)
            operator[](it->first) = it->second;
    }

    HashMap(const HashMap& other) :HashMap()
    {
        counter = other.counter;
        tab = other.tab;
        // for(auto it = list.begin(); it != list.end(); it++)
        //   operator[](it->first) = it->second;
    }

    HashMap(HashMap&& other):HashMap()
    {
        counter = other.counter;
        tab = other.tab;
        other.counter= 0;
        other.tab = nullptr;
    }

    HashMap& operator=(const HashMap& other)
    {
        if(this == &other)
            return *this;

        deleteMap();

        counter = other.counter;
        tab = other.tab;
        return *this;
    }

    HashMap& operator=(HashMap&& other)
    {
        if(this == &other)
            return *this;

        deleteMap();

        counter = other.counter;
        tab = other.tab;
        other.counter = 0;
        other.tab = nullptr;
        return *this;
    }

    bool isEmpty() const
    {
        return (!counter);
    }

    mapped_type& operator[](const key_type& key)
    {
        size_type k = hashFun(key);
        HashNode *node = findNode(key);
        if(!node)
        {
            node = new HashNode(key, mapped_type());
            ++counter;
            if (!tab[k])
                tab[k] = node;
            else
            {
                HashNode * temp = tab[k];
                while(temp->next)
                    temp = temp->next;
                temp->next = node;
                node->prev = temp;
            }
        }
        return node->element.second;
    }

    const mapped_type& valueOf(const key_type& key) const
    {
        HashNode* temp = findNode(key);
        if(!temp)
            throw std::out_of_range("ValueOf");
        return temp->element.second;
    }

    mapped_type& valueOf(const key_type& key)
    {
        HashNode* temp = findNode(key);
        if(!temp)
            throw std::out_of_range("ValueOf");
        return temp->element.second;
    }

    const_iterator find(const key_type& key) const
    {
        return const_iterator(this, findNode(key), hashFun(key));
    }

    iterator find(const key_type& key)
    {
        return iterator(this, findNode(key), hashFun(key));
    }

    void remove(const key_type& key)
    {
        remove(find(key));
    }

    void remove(const const_iterator& it)
    {
        if(it == cend())
            throw std::out_of_range("remove");
        auto temp = it.node;
        if(temp->prev)
            temp->prev->next = temp->next;
        else
            tab[hashFun(temp->element.first)] = temp->next;
        if(temp->next)
            temp->next->prev = temp->prev;
        //temp->prev = temp->next = nullptr;
        counter--;
        delete temp;
    }

    size_type getSize() const
    {
        return counter;
    }

    bool operator==(const HashMap& other) const
    {
        if(counter != other.counter)
            return false;

        for(auto it = other.begin(); it != other.end(); it++)
        {
             if(find(it->first) == end() || (find(it->first))->second != it->second)
                    return false;
        }
        return true;
}

bool operator!=(const HashMap& other) const
    {
        return !(*this == other);
    }

    iterator begin()
    {
        size_type index = findFirstNonEmpty();
        if (index == HASH_TABLE_MAX_SIZE)
            return end();
        return iterator(this, tab[index], index);
    }

    iterator end()
    {
        return iterator(this, nullptr, HASH_TABLE_MAX_SIZE);
    }

    const_iterator cbegin() const
    {
        size_type index = findFirstNonEmpty();
        if (index == HASH_TABLE_MAX_SIZE)
            return cend();
        return const_iterator(this, tab[index], index);
    }

    const_iterator cend() const
    {
        return const_iterator(this, nullptr, HASH_TABLE_MAX_SIZE);
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
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using pointer = const typename HashMap::value_type*;

private:

    const HashMap * container;
    HashNode * node;
    size_type index;

    friend void HashMap<KeyType, ValueType>::remove(const const_iterator&);

public:

    explicit ConstIterator(const HashMap *container = nullptr, HashNode *node = nullptr, size_type index = 0)
        : container(container), node(node), index(index)
    {
        if(node == nullptr && container != nullptr)
            this->index = container->HASH_TABLE_MAX_SIZE;
    }
    ConstIterator(const ConstIterator& other)
        : ConstIterator(other.container, other.node, other.index)
    {}

    ConstIterator& operator++()
    {
        if(!container || !node)
            throw std::out_of_range("++");
        if(node->next)
            node = node->next;
        else
        {
            do //while
                index++;
            while(!(container->tab[index]) && index != container->HASH_TABLE_MAX_SIZE);

            if(index ==  container->HASH_TABLE_MAX_SIZE)
                node = nullptr;
            else
                node = container->tab[index];
        }
        return *this;
    }

    ConstIterator operator++(int)
    {
        auto result = *this;
        operator++();
        return result;
    }

    ConstIterator& operator--()
    {
        if(!container)
            throw std::out_of_range("--");

        else if (!node || node == container->tab[index])
        {
            do //while
                index--;
            while (index > 0 && !(container->tab[index]));

            if (index == 0 && !(container->tab[index]))
                throw std::out_of_range("--");

            node = container->tab[index];
            while(node->next)
                node = node->next;
        }
        else
            node = node->prev;
        return *this;
    }
    ConstIterator operator--(int)
    {
        auto result = *this;
        operator--();
        return result;
    }

    reference operator*() const
    {
        if(!node)
            throw std::out_of_range("");
        return node->element;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {
        return (container == other.container && node == other.node && index == other.index);
    }

    bool operator!=(const ConstIterator& other) const
    {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::value_type*;

    explicit Iterator()
    {}

    Iterator(const HashMap *container = nullptr, HashNode *node = nullptr, size_type index = 0)
        : ConstIterator(container, node, index)
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

#endif /* AISDI_MAPS_HASHMAP_H */
