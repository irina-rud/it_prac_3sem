#pragma once
#include <vector>
#include <iostream>
#include <exception>
using namespace std; 

class SectionTree{
	vector<int> treeData;

	SectionTree(vector<int>& values) {
		treeData.reserve(values.size() * 4);
		for (int i = 0; i < values.size(); ++i) {
			treeData.push_back(INT_MAX);
		}
		build(values, 0, 0, values.size() - 1);
	}

	void build(vector<int>& values, int v, int tl, int tr) {
		if (tl == tr)
			treeData[v] = values[tl];
		else {
			int tm = (tl + tr) / 2;
			build(values, v * 2, tl, tm);
			build(values, v * 2 + 1, tm + 1, tr);
			treeData[v] = fmin(treeData[v * 2], treeData[v * 2 + 1]);
		}
	}

	int min(int v, int tl, int tr, int l, int r) {
		if (l > r)
			throw exception();
		if (l == tl && r == tr)
			return treeData[v];
		int tm = (tl + tr) / 2;
		return fmin(min(v * 2, tl, tm, l, min(r, tm)), min(v * 2 + 1, tm + 1, tr, fmax(l, tm + 1), r));
	}

	void update(int v, int tl, int tr, int pos, int new_val) {
		if (tl == tr)
			treeData[v] = new_val;
		else {
			int tm = (tl + tr) / 2;
			if (pos <= tm)
				update(v * 2, tl, tm, pos, new_val);
			else
				update(v * 2 + 1, tm + 1, tr, pos, new_val);
			treeData[v] = t[v * 2] + t[v * 2 + 1];
		}
	}

};