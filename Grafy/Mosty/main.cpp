#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "grafy.h"
#include <fstream>

int main()
{
    int x,y;
    ifstream file;
    file.open("data.txt");
    if (!file.good())
        cout<<"Error while entering file"<<endl;
    
    Graph graph;

    while(!file.eof())
    {
        file>>x>>y;
        graph.add_element(x,y);   
    }
    file.close();

    graph.printAllEdges();
    cout<<"\n\n\n";
    
    graph.printAllRozlegleMosty();

    return 0;
}
