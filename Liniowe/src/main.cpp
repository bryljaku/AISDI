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

void prependTest(Vector<int> *vec, LinkedList<int> *llist)
{
    double x;
    file<<vec->getSize()<<";";
    clock_t time  = clock();
    vec->prepend(10);
    PRINTTIME;
    time = clock();
    llist->prepend(10);
    PRINTTIME;
}

void appendTest(Vector<int> *vec, LinkedList<int> *llist)
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
    for(size_t amount=1; amount<=20 ; amount++)
    {
		for(int i = 1; i < 10000; i++)
		{
			vect->prepend(50);
			linkedList->prepend(50);
		}
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
