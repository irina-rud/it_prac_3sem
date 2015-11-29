// Aho-Corasick.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Aho.h"
#include <iostream>
#include <fstream>
#include <string>


int main(){
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");
	std::ifstream stop;
	stop.open("stopwords.txt", std::ifstream::in);

	Aho bor;
	while (!stop.eof()) {
		std::string str;
		getline(stop,str);
		bor.add_string(str);
	}

	std::string text = "";
	while (!in.eof()) {
		std::string buf;
		getline(in, buf);
		text += buf;
	}

	std::map<std::string, std::vector<int>> answer = bor.findAllPatterns(text);
	for (std::map<std::string, std::vector < int > >::iterator it = answer.begin(); it != answer.end(); it++)
	{
		for (int i = it->second[0]; i < it->second.size(); ++i) {
			for (int j = 0; j < it->first.size(); ++j) {
				text[i + j] = '*';
			}
		}
	}
	out << text;
	
	return 0;
}

