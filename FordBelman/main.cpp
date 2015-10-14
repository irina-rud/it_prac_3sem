#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <istream>
#include <fstream>

#define INF 30000
#define MAXV 100

//#define DEBUG_MODE

using namespace std;


int main()
{
    ifstream in("test.txt");
    streambuf * cinbuf = cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    int VertexesCount;
    cin >> VertexesCount;

    int LincsCount;
    cin >> LincsCount;

    int lincs[MAXV][MAXV];

    for (int i = 0; i < VertexesCount; ++i){
         for (int j = 0; j < VertexesCount; ++j){
            lincs[i][j] = INF;
         }
    }

    int first;
    int second;
    int weight;

    int count = 0;
    while (count < LincsCount){
        cin >> first >> second >> weight;
        if (lincs[first - 1][second - 1] == INF){
            lincs[first - 1 ][second - 1] = weight;
        }
        else{
            lincs[first - 1 ][second - 1] += weight;
        }

        ++count;
    }
#ifdef DEBUG_MODE
    for (int i = 0; i < VertexesCount; ++i){
        for (int j = 0; j< VertexesCount; ++j){
            cout << lincs[i][j] << " ";
        }
        cout << endl;
    }
#endif // DEBUG_MODE
    int fordBelman[MAXV][MAXV];

    for (int i = 0; i < VertexesCount; ++i){
        fordBelman[0][i] = 0;
        for (int j = 1; j < VertexesCount; ++j){
            fordBelman[j][i] = INF;
        }
    }
#ifdef DEBUG_MODE
    cout << endl<< "ford belman begins"<< endl;
    for (int i = 0; i < VertexesCount; ++i){
        for (int j = 0; j< VertexesCount; ++j){
            cout << fordBelman[i][j] << " ";
        }
        cout << endl;
    }
cout<< endl;

#endif // DEBUG_MODE

    for (int i = 1; i < VertexesCount; ++i){
        for (int j = 0; j < VertexesCount; ++j){
            for (int k = 0; k < VertexesCount; ++k){
                if (lincs[j][k] != INF){
                    if (fordBelman[k][i+1] == INF && fordBelman[j][i] != INF){
                        fordBelman[k][i+1] = fordBelman[j][i] + lincs[j][k];
                    }
                    else if (fordBelman[k][i+1] == INF && fordBelman[k][i]  == INF){
                        fordBelman[k][i+1] = INF;
                    }
                    else if (fordBelman[k][i+1] != INF && fordBelman[j][i]  != INF ){
                        fordBelman[k][i+1] = min(fordBelman[k][i+1], fordBelman[j][i] + lincs[j][k] );
                    }
                }
            }
        }
#ifdef DEBUG_MODE
    cout << "ford belman running"<< endl;
    for (int i = 0; i < VertexesCount; ++i){
        for (int j = 0; j< VertexesCount; ++j){
            cout << fordBelman[i][j] << " ";
        }
        cout << endl;
    }
#endif // DEBUG_MODE

    }

#ifdef DEBUG_MODE

    cout<< endl << "ford belman continues"<< endl;
    for (int i = 0; i < VertexesCount; ++i){
        for (int j = 0; j< VertexesCount; ++j){
            cout << fordBelman[i][j] << " ";
        }
        cout << endl;
    }
#endif // DEBUG_MODE

    for (int  i = 0; i < VertexesCount; ++i ){
        int min = INF;
        for (int k = 0 ; k < VertexesCount; ++k){
            if (fordBelman[i][k] < min){
                min = fordBelman[i][k];
            }
        }
        cout << min<< " ";
    }

    return 0;
}
