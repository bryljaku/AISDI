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
    size_type DEFAULTVECTORSIZE = 20;
    size_type reservedSize;
    size_type vectorSize;
    value_type *arr ;

    void reallocate()
    {
        if(reservedSize == 0)
            reservedSize = 20;

        reservedSize *= 2;
        Type *temp = new Type[reservedSize];
        size_type i;
        for (i=0; i != vectorSize; i++)
            temp[i] = arr[i];
        
        delete[] arr;
        arr = temp;
    }

public:
    Vector()
    {
        vectorSize = 0;
        reservedSize = 0;
        arr = nullptr;
    }


    ~Vector()
    {
        delete[] arr ;
    }


    Vector(std::initializer_list<Type> l)
    {
        vectorSize = l.size();
        reservedSize = vectorSize;
        arr = new value_type[reservedSize];
        std::copy(l.begin(), l.end(), arr);
    }

    Vector(const Vector& other)
    {
        vectorSize = other.vectorSize;
        reservedSize = other.reservedSize;
        arr = new value_type[reservedSize];
        std::copy(other.begin(), other.end(), arr);
    }



    Vector(Vector&& other)
    {
        arr = other.arr;
        vectorSize = other.vectorSize;
        reservedSize = other.reservedSize;
        other.arr = nullptr;
        other.vectorSize = 0;
        other.reservedSize = 0;
    }

    Vector& operator=(const Vector& other)
    {
        if (this != &other)
        {
            delete[] arr;
            vectorSize = other.vectorSize;
            reservedSize = other.reservedSize;
            arr = new Type[reservedSize];
            std::copy(other.begin(), other.end(), arr);
        }
        return *this;
    }

    Vector& operator=(Vector&& other)
    {
        if(this != &other)
        {
            delete [] arr;
            arr = other.arr;
            vectorSize = other.vectorSize;
            reservedSize = other.reservedSize;
            other.arr = nullptr;
            other.vectorSize = 0;
            other.reservedSize = 0;
        }
        return *this;
    }


    bool isEmpty() const
    {
        return (vectorSize == 0);
    }

    size_type getSize() const
    {
        return vectorSize;
    }

    void append(const Type& item)
    {
        if (vectorSize == reservedSize)
            reallocate();
        
        arr[vectorSize] = item;
        vectorSize++;
    }

    void prepend(const Type& item)
    {
        if (vectorSize == reservedSize)
            reallocate();
        
        for(size_type i = vectorSize; i != 0; --i)
            arr[i] = arr[i - 1];
        
        arr[0] = item;
        vectorSize++;
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if (insertPosition == cbegin())
        {
            prepend(item);
            return;
        }
        if (insertPosition == cend())
        {
            append(item);
            return;
        }

        size_type insertIndex = insertPosition.givePointer() - arr;
        if (vectorSize == reservedSize)
            reallocate();

        size_type i = vectorSize;

        while (i != insertIndex)
        {
            arr[i] = arr[i - 1];
            i--;
        }
        arr[i] = item;
        vectorSize++;
    }

    Type popFirst()
    {
        if (isEmpty())
        {
            throw std::out_of_range("");
        }
        value_type x = *begin();
        size_type i;
        for (i = 0; i != (vectorSize-1); i++)
        {
            arr[i] = arr[i + 1];
        }
        vectorSize--;
        return x;
    }

    Type popLast()
    {
        if (isEmpty())
            throw std::out_of_range("");
        
        value_type x;
        x = arr[vectorSize-1];
        --vectorSize;
        return x;
    }

    void erase(const const_iterator& possition)
    {
        if (isEmpty())
            throw std::out_of_range("");
        
        if (possition==end())
            throw std::out_of_range("");
        
        const_iterator j = begin();
        size_type i = 0;

        while (j != possition)
        {
            i++;
            j++;
        }
        j++;

        while(j != end())
        {
            arr[i] = arr[i + 1];
            i++;
            j++;
        }
        vectorSize--;

    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        if (&firstIncluded == &lastExcluded)
            return;
     
        const_iterator it = begin();
        size_type newIndex = 0;
        size_type counter = 0;
        while (it != firstIncluded)
        {
            newIndex++;
            it++;
        }
        while (it != lastExcluded)
        {
            it++;
            counter++;

        }
        while (it != end())
        {
            arr[newIndex] = arr[newIndex + counter];
            it++;
            newIndex++;
        }
        vectorSize -= counter;

    }

    iterator begin()
    {
        iterator i = Iterator(arr, this);
        return i;
    }

    iterator end()
    {
        iterator i = Iterator(arr+vectorSize, this);
        return i;
    }


    const_iterator cbegin() const
    {
        const_iterator i = ConstIterator(arr, this);
        return i;
    }

    const_iterator cend() const
    {
        const_iterator i = ConstIterator(arr + vectorSize, this);
        return i;
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
    using this_pointer = const Vector<Type>*;


    explicit ConstIterator()
    {
        ptr = nullptr;
        vectorPointer = nullptr;
    }

    ConstIterator(pointer other, this_pointer other2)
    {
        ptr = other;
        vectorPointer = other2;
    }

    pointer givePointer() const
    {
        return ptr;
    }

    reference operator*() const
    {
        if(*this == vectorPointer->end())
            throw std::out_of_range("");
        
        return *ptr;
    }

    value_type giveValue() const
    {
        return *ptr;
    }

    ConstIterator& operator++()
    {
        if(*this==vectorPointer->end())
            throw std::out_of_range("");
        
        ptr++;
        return *this;
    }

    ConstIterator operator++(int)
    {
        if(*this==vectorPointer->end())
            throw std::out_of_range("");
        
        ConstIterator i= *this;
        ptr++;
        return i;
    }

    ConstIterator& operator--()
    {
        if(*this==vectorPointer->begin())
            throw std::out_of_range("");
        
        ptr--;
        return *this;
    }

    ConstIterator operator--(int)
    {
        if(*this==vectorPointer->begin())
            throw std::out_of_range("");
        
        auto i = *this;
        ptr--;
        return i;
    }

    ConstIterator operator+(difference_type d) const
    {
        ConstIterator itt = *this;
        itt.ptr += d;
        return itt;
    }

    ConstIterator operator-(difference_type d) const
    {
        ConstIterator itt = *this;
        itt.ptr -= d;
        return itt;
    }

    ConstIterator& operator=(const ConstIterator&other)
    {
        ptr = other.ptr;
        return *this;
    }

    bool operator==(const ConstIterator& other) const
    {
        return (ptr== other.ptr);
    }

    bool operator>=(const ConstIterator& other) const
    {
        return (ptr >= other.ptr);
    }
    bool operator<(const ConstIterator& other) const
    {
        return (ptr < other.ptr);
    }
    bool operator!=(const ConstIterator& other) const
    {
        return (ptr != other.ptr);
    }

private:
    pointer ptr;
    this_pointer vectorPointer;

};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;
    using this_pointer = Vector<Type>*;

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

    Iterator (pointer other, this_pointer other2)
        : ConstIterator(other, other2)
    {}

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
