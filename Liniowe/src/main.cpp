#include <cstddef>
#include <cstdlib>
#include <string>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include "Vector.h"
#include "LinkedList.h"
#define PRINTTIME x=clock()-time;file<<x<<";"
using namespace aisdi;

    std::fstream file;
<<<<<<< HEAD

void prependTest(Vector<int> *vec, LinkedList<int> *llist)
{
    double x;
=======
/*
LinkedList<int> * createLinkedList(size_t numberOfElements)
{
    *linkedList = new LinkedList;
    return linkedList
}
Vector<int> * createVector(size_t numberOfElements)
{
    *vector = new Vector;
    for( size_t i = 0; i < numberOfElements; i++)
        vector->append((const int) i);
    return vector;
}*/

void prependTest(Vector<int> *vec, LinkedList<int> *llist)
{
    double x;    
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
    file<<vec->getSize()<<";";
    clock_t time  = clock();
    vec->prepend(10);
    PRINTTIME;
    time = clock();
    llist->prepend(10);
    PRINTTIME;
}

void appendTest(Vector<int> *vec, LinkedList<int> *llist)
<<<<<<< HEAD
{
    double x;
    clock_t time = clock();
    vec->append(15);
    PRINTTIME;
    time = clock();
    llist->append(15);
    PRINTTIME;
}
void eraseTest(Vector<int> *vec, LinkedList<int> *llist)
{
=======
{
    double x;
    clock_t time = clock();
    vec->append(15);
    PRINTTIME;
    time = clock();
    llist->append(15);
    PRINTTIME;
}
void eraseTest(Vector<int> *vec, LinkedList<int> *llist)
{
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
double x;
    auto vecIter = vec->cbegin();
    clock_t time = clock();
    vec->erase(vecIter);
    PRINTTIME;

    auto listIter = llist->cbegin();
    time = clock();
    llist->erase(listIter);
    PRINTTIME;
}


int main()
{
file.open("test.txt", std::fstream::out);
LinkedList<int> *linkedList = new LinkedList<int>;
Vector<int> *vect = new Vector<int>;
file<<"items;vectorPrependTime;linkedListPrependTime;VectorAppendTime;LinkedListAppendTime;VectorEraseTime;LinkedListEraseTime;\n";
<<<<<<< HEAD
    for(size_t amount=1; amount<=20 ; amount++)
    {
		for(int i = 1; i < 10000; i++)
		{
			vect->prepend(50);
			linkedList->prepend(50);
		}
=======
    for(size_t amount=1; amount<=100000 ; amount++)
    {
>>>>>>> 9de7cd5d044195608c7191af6b84f5125f66b4e2
        prependTest(vect, linkedList);
        appendTest(vect, linkedList);
        eraseTest(vect, linkedList);
        file<<"\n";
    }
    file.close();
    delete linkedList;
    delete vect;
    return 0;
}
