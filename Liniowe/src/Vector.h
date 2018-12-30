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
<<<<<<< HEAD
        this -> _array = nullptr;
        this -> _size = 0;
        this -> _max_size = 0;
=======
        vectorSize = 0;
        reservedSize = 0;
        arr = nullptr;
    }


    ~Vector()
    {
        delete[] arr ;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }


    Vector(std::initializer_list<Type> l)
    {
<<<<<<< HEAD
        //overflow exception needed?
        this -> _size = l.size();
        this -> _max_size = 2 * this -> _size;
        this -> _array = new Type[this -> _max_size];
        for (int i = 0; i < this -> _size; i++)
        {
            this -> _array[i] = l[i];
        }
=======
        vectorSize = l.size();
        reservedSize = vectorSize;
        arr = new value_type[reservedSize];
        std::copy(l.begin(), l.end(), arr);
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    Vector(const Vector& other)
    {
<<<<<<< HEAD
        this -> _size = other._size;
        this -> _array = other._array;
        this -> _max_size = other._max_size;
=======
        vectorSize = other.vectorSize;
        reservedSize = other.reservedSize;
        arr = new value_type[reservedSize];
        std::copy(other.begin(), other.end(), arr);
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }



    Vector(Vector&& other)
    {
<<<<<<< HEAD
        this -> _size = other._size;
        this -> _array = other._array;
        this -> _max_size = other._max_size;
        other._size = 0;
        other._array = nullptr;
        other._max_size = 0;
=======
        arr = other.arr;
        vectorSize = other.vectorSize;
        reservedSize = other.reservedSize;
        other.arr = nullptr;
        other.vectorSize = 0;
        other.reservedSize = 0;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    Vector& operator=(const Vector& other)
    {
<<<<<<< HEAD
        if(this == &other) return *this;
        erase(begin(),end());
        this -> _size = other._size;
        this -> _array = other._array;
        this -> _max_size = other._max_size;
=======
        if (this != &other)
        {
            delete[] arr;
            vectorSize = other.vectorSize;
            reservedSize = other.reservedSize;
            arr = new Type[reservedSize];
            std::copy(other.begin(), other.end(), arr);
        }
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
        return *this;
    }

    Vector& operator=(Vector&& other)
    {
<<<<<<< HEAD
        if(this == &other) return *this;
        erase(begin(),end());
        this-> _size = other._size;
        this -> _array = other._array;
        this -> _max_size = other._max_size;
        other._size = 0;
        other._array = nullptr;
        other._max_size = 0;
=======
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
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
        return *this;
    }


    bool isEmpty() const
    {
<<<<<<< HEAD
        if(this -> _size == 0) return true;
        return false;
=======
        return (vectorSize == 0);
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    size_type getSize() const
    {
<<<<<<< HEAD
        return this -> _size;
=======
        return vectorSize;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    void append(const Type& item)
    {
<<<<<<< HEAD
        if(this -> _size == this -> _max_size) resize(this -> _max_size);




        this -> _array[this -> _size] = item;
        this -> _size++;
=======
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
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
<<<<<<< HEAD
        if(this -> _max_size == 0)
        {
            if(this -> _array != nullptr) throw std::out_of_range("Something is wrong");
            this -> _max_size = default_vector_size;
            Type *new_array = new Type[this -> _max_size];
            this -> _array = new_array;
=======
        if (insertPosition == cbegin())
        {
            prepend(item);
            return;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
        }
        if (insertPosition == cend())
        {
<<<<<<< HEAD
            if(this -> _array == nullptr) throw std::out_of_range("Impossibe, vector pointer NULL and size > 0");
            Type *new_array = new Type[2 * this -> _max_size];
            for(iterator it = begin(); it != end(); it++)
            {
                new_array[it.position] = *it.current;

                /*
                position in Iterator is a counter of elements we skipped
                current is a pointer to element on which iterator stands

                what must be done in Iterator class XD
                */
            }
            this -> _max_size = 2 * this -> _max_size;
=======
            append(item);
            return;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
        }

        size_type insertIndex = insertPosition.givePointer() - arr;
        if (vectorSize == reservedSize)
            reallocate();

<<<<<<< HEAD
    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if(this -> _size >= this -> _max_size)
            resize();
        ConstIterator it = insertPosition;
        for(ConstIterator it2 = end(); it2 != it; it2--)
        {
            this -> _array[it2.position] = this -> _array[it2.position - 1];
        }
        this -> _array[it2.position] = item;
        this -> _size++;
=======
        size_type i = vectorSize;

        while (i != insertIndex)
        {
            arr[i] = arr[i - 1];
            i--;
        }
        arr[i] = item;
        vectorSize++;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    Type popFirst()
    {
<<<<<<< HEAD
        Type value = this -> _array[0];
        erase(begin());
        return value;
=======
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
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    }

    Type popLast()
    {
<<<<<<< HEAD
        Type value = this -> _array[this -> _size -1];
        erase(end());
        return value;
=======
        if (isEmpty())
            throw std::out_of_range("");
        
        value_type x;
        x = arr[vectorSize-1];
        --vectorSize;
        return x;
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
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
