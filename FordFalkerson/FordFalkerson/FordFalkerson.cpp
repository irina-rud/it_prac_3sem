// FordFalkerson.cpp
// by RIV
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include "CMyGraph.h"

#define KUBE_FACES 6

class WrongSizeExeption : std::exception{
public:
	WrongSizeExeption() {
		fprintf(stderr, "Wrong size of kube");
	}

};

class CKube {
public:
	char letters[KUBE_FACES];
	string str;
	CKube(std::string input) {
		if (input.size() != KUBE_FACES) {
			throw WrongSizeExeption();
		}
		for (int i = 0; i < KUBE_FACES; ++i) {
			letters[i] = input[i];
			str = input;
		}
	}
};



int main()
{
	CNetwork<std::string, int> graph;
	graph.AddVertex("Start");
	graph.AddVertex("Goal");
	std::string word;
	cin >> word;
	for (unsigned int i = 0; i < word.size(); ++i) {
		graph.AddEdge(word[i] + "2", "Goal", 1);
	}
	std::string newKubeString;
	int count = 0;
	try {
		newKubeString.clear();
		cin >> newKubeString;
		CKube kubic(newKubeString);
		for (int i = 0; i < KUBE_FACES; ++i) {
			graph.AddEdge("Start", std::to_string(i), 1);
			if (graph.isVertex(kubic.letters[i] + "2")) {
				graph.AddEdge(std::to_string(i), kubic.letters[i] + "2", 1);
			}
		}
		count++;
	}
	catch (WrongSizeExeption) {
		fprintf(stdout, "NO");
	}
	for (int i = 0; i < word.size(); ++i) {
		graph.AddEdge(std::to_string(word[i]), "Goal", 1);
	}

	if (graph.getMaxFlow("Start", "Goal") == word.size()) {
		fprintf(stdout, "Yes");
	}
	else {
		fprintf(stdout, "NO");
	}
    return 0;
}




