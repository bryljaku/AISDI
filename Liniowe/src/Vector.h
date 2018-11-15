#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{
int default_vector_size = 8;

template <typename Type>
class Vector
{
private:
    Type *array;
    int size, reserved_size;

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

    Vector()
    {
        _array = nullptr;
        _size = 0;
        _max_size = 0;
    }
    Vector(std::initializer_list<Type> l)
    {
        //overflow exception needed?
        _size = l.size();
        _max_size = 2 * _size;
        _array = new Type[_max_size];
        for (int i = 0; i<_size; i++)
        {
            _array[i] = l[i];
        }
    }

    Vector(const Vector& other)
    {
        _size = other._size;
        _array = other._array;
        _max_size = other._max_size;
    }

    Vector(Vector&& other)
    {
        _size = other._size;
        _array = other._array;
        _max_size = other._max_size;
        other._size = 0;
        other._array = nullptr;
        other._max_size = 0;
    }

    ~Vector()
    {}

    Vector& operator=(const Vector& other)
    {
        if(this == &other) return *this;
        erase(begin(),end());
        _size = other._size;
        _array = other._array;
        _max_size = other._max_size;
        return *this
    }

    Vector& operator=(Vector&& other)
    {
        if(this == &other) return *this;
        erase(begin(),end());
        _size = other._size;
        _array = other._array;
        _max_size = other._max_size;
        other._size = 0;
        other._array = nullptr;
        other._max_size = 0;
        return *this;
    }

    bool isEmpty() const
    {
        if(_size == 0) return true;
        return false;
    }

    size_type getSize() const
    {
        return _size;
    }

    void append(const Type& item)
    {
        if(_size == _max_size) resize(_max_size);




        _array[_size] = item;
        _size++;
    }
    void resize()
    {
        if(_max_size == 0)
        {
            if(_array != nullptr) throw std::out_of_range("Something is wrong");
            _max_size = default_vector_size;
            Type *new_array = new Type[_max_size];
            _array = new_array;
        }
        else
        {
            if(_array == nullptr) throw std::out_of_range("Impossibe, vector pointer NULL and size > 0");
            Type *new_array = new Type[2 * _max_size];
            for(iterator it = begin(); it != end(); it++)
            {
                new_array[it.position] = *it.current;

                /*
                position in Iterator is a counter of elements we skipped
                current is a pointer to element on which iterator stands

                what must be done in Iterator class XD
                */
            }
            _max_size = 2 * _max_size;
        }
    }

    void prepend(const Type& item)
    {
        insert(begin(), item);
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if(_size >= _max_size)
            resize();
        ConstIterator it = insertPosition;
        for(ConstIterator it2 = end(); it2 != it; it2--)
        {
            _array[it2.position] = _array[it2.position - 1];
        }
        _array[it2.position] = item;
        _size++;
    }

    Type popFirst()
    {
        throw std::runtime_error("TODO");
    }

    Type popLast()
    {
        throw std::runtime_error("TODO");
    }

    void erase(const const_iterator& possition)
    {
        (void)possition;
        throw std::runtime_error("TODO");
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        (void)firstIncluded;
        (void)lastExcluded;
        throw std::runtime_error("TODO");
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

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

    explicit ConstIterator()
    {}

    reference operator*() const
    {
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

    ConstIterator operator+(difference_type d) const
    {
        (void)d;
        throw std::runtime_error("TODO");
    }

    ConstIterator operator-(difference_type d) const
    {
        (void)d;
        throw std::runtime_error("TODO");
    }

    bool operator==(const ConstIterator& other) const
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    bool operator!=(const ConstIterator& other) const
    {
        (void)other;
        throw std::runtime_error("TODO");
    }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
