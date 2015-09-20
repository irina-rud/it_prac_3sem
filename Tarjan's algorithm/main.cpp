#include "CMyGraph.h"
#include <iostream>

using namespace std;


int main()
{
    CMyGraph <string, string> my_graph;
    ifstream test_file;
    test_file.open("/home/riv/it_prac_3sem/Graph/test2.txt", ifstream::in);
    test_file >> my_graph;
    cout << my_graph << "end" << endl;

    CMyGraph <string,string> transponed = *my_graph.Transpon();
    vector <string> topologicalResult;
    for (auto i = transponed.Begin();(i != transponed.End());i++){
        topologicalResult.insert(topologicalResult.begin(),*i);
    }

    vector <string> result;

    for (auto i = my_graph.Begin(*(topologicalResult.begin()));i.isEnd() == false ; i = i.next(topologicalResult)){
        cout << *i << " ";
    }

    cout << "Tarjan"<< endl;
    my_graph.Tarjan();


    return 0;
}
