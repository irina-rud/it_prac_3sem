#pragma once
#ifndef FIBHEAP_INCLUDED
#define FIBHEAP_INCLUDED

#include <vector>
#include<list>

using std::vector;
using std::pair;



class CTreeException : std::exception {
public:
	std::string Message() {}
};

class BadCastEXC : public CTreeException {
public:
	std::string Message() {
		return "ERROR: Meldable heap has uncorrect type";
	}
};

class DifferentSizeMeldedTrees : public CTreeException {
public:
	std::string Message() {
		return "ERROR: in MeldSimilar: Meldable trees have a different degree";
	}
};

class EmptyTree : public CTreeException {
public:
	std::string Message() {
		return "ERROR: in ExtactMin: Tree is empty";
	}
};


template <class TVertexID>
class CExtendedBasicTree {
public:
	TVertexID data;
	typename std::list<CExtendedBasicTree<TVertexID>*>::iterator listit = NULL;
	std::list<CExtendedBasicTree<TVertexID>*> child = NULL;
	CExtendedBasicTree<TVertexID>* parent = NULL;
	bool mark = false;
	size_t degree;

	CExtendedBasicTree() {}
	CExtendedBasicTree(TVertexID key) {
		data = key;
		child = NULL;
		right = NULL;
		left = NULL;
		parent = NULL;
		mark = false;
		degree = 0;
	}
	CExtendedBasicTree(TVertexID key, size_t currentDegree) {
		data = key;
		child = NULL;
		right = NULL;
		left = NULL;
		parent = NULL;
		mark = false;
		degree = currentDegree;
	}
	~CExtendedBasicTree() {}
};

template <TVertexID>
class CMeldableHeap {
public:
	CMeldableHeap() {}

	virtual void Insert(int key) = 0;
	virtual int ExtractMin() = 0;
	virtual void meld(iMeldableHeap* addedHeap) = 0;

	virtual BasicTree* meldSimilar(BasicTree* tree1, BasicTree* tree2) = 0;
	virtual int getMin() = 0;

	virtual ~iMeldableHeap() {}
};


template <class TVertexID>
class CFibonacciTree :CMeldableHeap<TVertexID> {
public:
	int size = 0;
	CExtendedBasicTree* min = NULL;
	list<CExtendedBasicTree<TVertexID>*> roots;
	CFibonacciTree() {}
	CFibonacciTree(TVErtexID key) {
		min = new CExtendedBasicTree(key);
	}
	void insert(TVertexID key) {
		this->meld(new CFibonacciTree(key));
	}
	TVertexID getMin() {
		if (min == NULL) {
			throw EmptyTree();
		}
		return min->child;
	}
	void push(TVertexID key)
	{
		CExtendedBasicTree<TVertexID>*  node = new CExtendedBasicTree<TVertexID>(key);
		roots.push_back(node);
		auto p = roots.end();
		p--;
		node->listit = p;
		if (min == NULL || value < min->data))
			min = *p;
	}
	
	bool Empty() const
	{
		return roots.empty();
	}

	TVertexID ExtractValue()
	{
		TVertexID val = min->data;
		Erase(min);
		return val;
	}

	void Erase(CExtendedBasicTree<TVertexID> * node)
	{
		for (typename std::list<CExtendedBasicTree<TVertexID>*>::iterator it = node->child.begin(); it != node->child.end(); ++it)
		{
			roots.push_back(*it);
			auto p = roots.end();
			p--;
			(**it).listit = p;
			(**it).parent = NULL;

		}
		if (node->parent == NULL)
		{
			roots.erase(node->listit);
		}
		else
		{
			node->parent->children.erase(node->listit);
			ChildGo(node->parent);
		}
		delete node;
		update();
	}
	void ChildGo(CExtendedBasicTree<TVertexID>* t)
	{
		if (t == NULL)
			return;
		if (!t->mark)
			t->mark = true;
		else
		{
			t->mark = false;
			t->parent->children.erase(t->listit);
			roots.push_back(t);
			auto p = roots.end();
			p--;
			t->listit = p;
			ChildGo(t->parent);
			t->parent = NULL;
		}
	}

	void update()
	{
		int maxn = 0;
		for (auto it = roots.begin(); it != roots.end(); it++)
			if (maxn < (int)((*it)->child.size()))
				maxn = (int)((*it)->child.size());
		std::vector<std::list<CExtendedBasicTree<TVertexID>*> > v(maxn + 1);
		for (typename std::list<CExtendedBasicTree<TVertexID>*>::iterator it = roots.begin(); it != roots.end(); ++it)
			v[(**it).children.size()].push_back(*it);
		roots.clear();
		hmin = NULL;
		for (int i = 0; i < (int)v.size(); ++i)
		{
			if (v[i].size() > 1 && i + 1 == (int)v.size())
				v.push_back(std::list<Node*>());
			while (v[i].size() > 1)
			{
				if (Cmp()(v[i].back()->value, v[i].front()->value))
					std::swap(v[i].back(), v[i].front());
				v[i].back()->parent = v[i].front();
				v[i].front()->children.push_back(v[i].back());
				auto p = v[i].front()->children.end();
				p--;
				v[i].back()->listit = p;
				v[i].pop_back();
				v[i + 1].push_back(v[i].front());
				v[i].pop_front();
			}
			if (v[i].size() == 1)
			{
				roots.push_back(v[i].front());
				auto p = roots.end();
				p--;
				v[i].front()->listit = p;
				v[i].front()->parent = NULL;
				if (min == NULL || v[i].front()->value < min->data)
					min = *p;
			}
		}
	}

	void merge(CFibonacciTree<TVertexID>& anotherHeap)
	{
		roots.merge(anotherHeap.roots);
		if (anotherHeap.min == NULL)
			return;
		if (min == NULL)
		{
			min = anotherHeap.min;
			return;
		}
		if (anotherHeap.min->value < min->value))
			min = anotherHeap.min;
	}

};



#endif //FIBHEAP_INCLUDED