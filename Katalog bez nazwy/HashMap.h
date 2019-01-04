#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

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
        value_type data;
        HashNode *prev, *next;

        HashNode(key_type key, mapped_type mapped)
            : data(std::make_pair(key, mapped)), prev(nullptr), next(nullptr)
        {}

        HashNode(key_type key, mapped_type mapped, HashNode *prev) : HashNode(key, mapped)
        {
            this->prev = prev;
        }

        ~HashNode()
        {
            if(next)
                delete next;
        }
    };

    const size_type TABLE_SIZE = 100;
    HashNode **table;
    size_type count;

public:

  HashMap() : count(0)
  {
    table = new HashNode* [TABLE_SIZE]{};
    for(size_type i = 0; i< TABLE_SIZE; ++i)
        table[i] = nullptr;
  }

  ~HashMap()
  {
      if(table)
      {
          deleteMap();
        delete [] table;
      }
  }

  HashMap(std::initializer_list<value_type> list) : HashMap()
  {
    for(auto it = list.begin(); it != list.end(); ++it)
        operator[](it->first) = it->second;
  }

  HashMap(const HashMap& other) : HashMap()
  {
    for(auto it = other.begin(); it != other.end(); ++it)
        operator[](it->first) = it->second;
  }

  HashMap(HashMap&& other) : HashMap()
  {
    count = other.count;
    table = other.table;
    other.count = 0;
    other.table = nullptr;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(this == &other)
        return *this;

    deleteMap();

    for(auto it = other.begin(); it != other.end(); ++it)
        operator[](it->first) = it->second;

    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    if(this == &other)
        return *this;

    deleteMap();

    count = other.count;
    table = other.table;
    other.count = 0;
    other.table = nullptr;

    return *this;
  }

  bool isEmpty() const
  {
    return !(count);
  }

  mapped_type& operator[](const key_type& key)
  {
    size_type k = hashFunction(key);
    HashNode *node = findNode(key);
    if(!node)
    {
        node = new HashNode(key, mapped_type());
        ++count;

        if(table[k] == nullptr)
            table[k] = node;
        else
        {
            HashNode *temp = table[k];
            while(temp->next)
                temp = temp->next;
            temp->next = node;
            node->prev = temp;
        }
    }

    return node->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    HashNode *temp = findNode(key);
    if(!temp)
        throw std::out_of_range("valueOFC");
    return temp->data.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    HashNode *temp = findNode(key);
    if(!temp)
        throw std::out_of_range("valueOf");
    return temp->data.second;
  }

  const_iterator find(const key_type& key) const
  {
    return const_iterator(this, findNode(key), hashFunction(key));
  }

  iterator find(const key_type& key)
  {
    return iterator(this, findNode(key), hashFunction(key));
  }

  void remove(const key_type& key)
  {
    remove(find(key));
  }

  void remove(const const_iterator& it)
  {
    if(it == end())
        throw std::out_of_range("remove");

    auto temp = it.node;
    if(temp->prev)
        temp->prev->next = temp->next;
    else //pierwszy element
        table[hashFunction(temp->data.first)] = temp->next;

    if(temp->next)
        temp->next->prev = temp->prev;

    temp->prev = temp->next = nullptr;
    --count;
    delete temp;
  }

  size_type getSize() const
  {
    return count;
  }

  bool operator==(const HashMap& other) const
  {
    if(count != other.count)
        return false;

    for(auto it = other.begin(); it != other.end(); ++it)
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
    size_type index = firstNotEmptyElement();

    if(index == TABLE_SIZE)
        return end();

    return iterator(this, table[index], index);
  }

  iterator end()
  {
    return iterator(this, nullptr, TABLE_SIZE);
  }

  const_iterator cbegin() const
  {
      size_type index = firstNotEmptyElement();

      if(index == TABLE_SIZE)
        return cend();

      return const_iterator(this, table[index], index);
  }

  const_iterator cend() const
  {
    return const_iterator(this, nullptr, TABLE_SIZE);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

private:

    size_type firstNotEmptyElement() const
    {
        size_type index = 0;

        while(table[index] == nullptr && index != TABLE_SIZE)
            ++index;

        return index;
    }

    size_type hashFunction(const key_type& key) const
    {
        return key % TABLE_SIZE;
    }

    HashNode* findNode(const key_type& key) const
    {
        HashNode* temp = table[hashFunction(key)];

        while(temp)
        {
            if(temp->data.first == key)
                return temp;
            temp = temp->next;
        }

        return temp;
    }

    void deleteMap()
    {
      for(size_type index = 0; index < TABLE_SIZE; ++index)
      {
          deleteList(index);
          table[index] = nullptr;
      }

      count = 0;
    }

    void deleteList(size_type index)
    {
        HashNode* temp = table[index];
        HashNode* temp2;

        while(temp)
        {
            temp2 = temp->next;
            temp->next = nullptr;
            temp->prev = nullptr;
            delete temp;
            temp = temp2;
        }
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
    const HashMap *map;
    HashNode *node;
    size_type index;
    friend void HashMap<KeyType, ValueType>::remove(const const_iterator&);

public:

  explicit ConstIterator(const HashMap *map = nullptr, HashNode *node = nullptr, size_type index = 0)
    : map(map), node(node), index(index)
  {
    if(node == nullptr && map != nullptr)
        this->index = map->TABLE_SIZE;
  }

  ConstIterator(const ConstIterator& other) : ConstIterator(other.map, other.node, other.index)
  {}

  ConstIterator& operator++()
  {
    if(map == nullptr || node == nullptr)
        throw std::out_of_range("operator++");

    if(node->next)
        node = node->next;
    else
    {
        ++index;
        while(map->table[index] == nullptr && index != map->TABLE_SIZE)
            ++index;

        if(index == map->TABLE_SIZE)
            node = nullptr;
        else
            node = map->table[index];
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
      if(!map)
        throw std::out_of_range("operator--");
      else if(node == nullptr || node == map->table[index]) //end
      {
          --index;
          while(index > 0 && map->table[index] == nullptr)
            --index;

          if(index == 0 && map->table[index] == nullptr)
            throw std::out_of_range("operator--");

          node = map->table[index];

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
        throw std::out_of_range("operator*");
    return node->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (map == other.map && node == other.node && index == other.index);
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

  explicit Iterator(const HashMap *map = nullptr, HashNode *node = nullptr, size_type index = 0)
    : ConstIterator(map, node, index)
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

#endif /* AISDI_MAPS_HASHMAP_H */
