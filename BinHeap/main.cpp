#include <iostream>
#include "BinomialHeap.h""
using namespace std;

int main()
{
	BinomialHeap<int> heap(5);
	cout << heap.ExtractMin();
	try {
		heap.ExtractMin();
	}
	catch (EmptyTree) {
		cout << "!:)";
	}
	return 0;
}
