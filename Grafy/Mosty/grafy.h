#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <queue>
#define SIZE 10

class Graph{
private:
const static int size = SIZE;
vector <int> *adjList;
vector <pair<int,int>> allEdges;
void init()
{
    EdgesOfVertex =  new vector <int> [size];
}

public:
Graph()
{
    init();
}

void add_element(const int & first, const int & second)
{
    for (int i = 0; i < allEdges.size(); i++)
        if (std::make_pair(first,second) == allEdges[i] || std::make_pair(second,first) == allEdges[i])
            return;

    allEdges.push_back(make_pair(first,second));
    EdgesOfVertex[first].push_back(second);
    EdgesOfVertex[second].push_back(first);
}

bool CheckIfVisited(int tab[], int sizeOfTab, int vertex)
{
    for (int i = 0; i < sizeOfTab; i++)
        if (vertex == tab[i])
            return true;

    return false;
}

int getSize()
{
    int counter = 0;
    for (int i = 0; i < size; i++)
        if (!EdgesOfVertex[i].empty())
            counter++;

    return counter;
}
int FirstNotEmptyVertex(const int &remove1, const int &remove2)
{
    for (int i = 0; i < size; i++)
        if (!EdgesOfVertex[i].empty() && i != remove1 && i != remove2)
            return i;
    
    return -1;
}

bool isConnected(const int & remove1, const int & remove2)
{
    int VisitedTab[size];
    VisitedTab[0] = remove1;
    VisitedTab[1] = remove2;
    int currentSize = 2;
    queue <int> VertexToVisit;
    VertexToVisit.push(FirstNotEmptyVertex(remove1,remove2));  //dodajemy pierwszy element z pierwszego niepustego wektora do kolejki.

    while(!VertexToVisit.empty())   //dopoki kolejka nie jest pusta
    {
        int currentVertex = VertexToVisit.front();
        if (!CheckIfVisited(VisitedTab, currentSize, currentVertex)) //jezeli jeszcze nie byl odwiedzony to
        {
            VisitedTab[currentSize++] = currentVertex;  //dodaj go do odwiedzonych
            for (auto it = EdgesOfVertex[currentVertex].begin(); it != EdgesOfVertex[currentVertex].end(); it++) //spisujemy jego sasiadow do kolejki
                VertexToVisit.push(*it);
        }
        VertexToVisit.pop();    
    }
    return currentSize == getSize();
}
void printAllEdges()
{
    for (auto it = allEdges.begin(); it != allEdges.end(); it++)
        cout<<"["<<it->first<<","<<it->second<<"]"<<endl;
}

void printBridges()
{
    cout<<"Bridges in given graph:\n";
    for (auto it = allEdges.begin(); it != allEdges.end(); it++)
        if (!isConnected(it->first,it->second))
            std::cout<<"["<<it->first<<","<<it->second<<"]\n";    
}

};
