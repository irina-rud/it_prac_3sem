#include "CMyGraph.h"
#include <iostream>

using namespace std;


int main()
{
    CMyGraph <string, string> my_graph;
    ifstream test_file;
    test_file.open("test.txt", ifstream::in);
    test_file >> my_graph;
    cout << my_graph;

    for (auto i = my_graph.Begin();
     (i != my_graph.End());
     i++){
        cout << *i;
    }


    return 0;
}
