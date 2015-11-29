// Circules.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <istream>
#include <vector>

using namespace std;

int main(){
	char c;
	vector <int> input;
	while (!cin.eof()) {
		cin >> c;
		input.push_back( int(c) - int(char(0)));
	}

	vector <long> hash;
	hash.reserve(input.size());
	hash.push_back(input[0]);
	int result = -1;
	for (int i = 1; i < input.size(); ++i) {
		hash.push_back((hash[i - 1] + long(pow(5, i)*input[i])) % LONG_MAX);
	}
	for (int i = input.size() / 3 - 1 ; i < input.size() / 2; ++i) {
		long pieceHash = (hash[2 * i - 1] - hash[i]) / pow(5, i);
		if (pieceHash == hash[i - 1]) {
			bool equal = true;
			for (int j = 0; j < i; ++j) {
				if (input[j] != input[i + j]) {
					equal = false;
					break;
				}
			}
			if (equal) {
				result = i;
				break;
			}
		}
	}
	if (result == -1) {
		cout << ":(";
		return 0;
	}
	for (int i = 0; i < 2 * (result); ++i) {
		cout << input[i]; 
	}
	cout << "..";
	return 0;
}
