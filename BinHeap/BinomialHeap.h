#pragma once
#ifndef BINOMIALHEAP_INCLUDED
#define BINOMIALHEAP_INCLUDED

#include <vector>

using std::vector;
using std::pair;

class CBasicTree;

class CBinomialHeap;

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
class BasicTree {
public:
	TVertexID data;
	BasicTree* right = NULL;
	BasicTree* child = NULL;
	BasicTree* parent = NULL;
	size_t degree;

	BasicTree() {}
	BasicTree(TVertexID key) {
		data = key;
		child = NULL;
		right = NULL;
		parent = NULL;
		degree = 0;
	}
	BasicTree(TVertexID key, size_t currentDegree) {
		data = key;
		child = NULL;
		right = NULL;
		parent = NULL;
		degree = currentDegree;
	}
	~BasicTree() {
		if (child!= NULL) delete child;
		if (right!= NULL) delete right;
	}
};

template <class TVertexID>
class CMeldableHeap {
public:
	CMeldableHeap() {}

	virtual int ExtractMin() = 0;
	virtual void meld(CMeldableHeap* addedHeap) = 0;

	virtual BasicTree<TVertexID>* meldSimilar(BasicTree<TVertexID>* tree1, BasicTree<TVertexID>* tree2) = 0;
	virtual int getMin() = 0;

	virtual ~CMeldableHeap() {}
};



template <class TVertexID>
class BinomialHeap : public CMeldableHeap<TVertexID> {
private:
	std::vector <BasicTree<TVertexID>*> vectorHeapData;

	BasicTree<TVertexID>* meldSimilar(BasicTree<TVertexID>* tree1, BasicTree<TVertexID>* tree2) {
		if (tree1 == NULL) return tree2;
		if (tree2 == NULL) return tree1;
		if (tree1->degree != tree2->degree)
		{
			throw DifferentSizeMeldedTrees();
		}
		if (tree1->data > tree2->data)
		{
			std::swap(tree1, tree2);
		}
		tree2->right = tree1->child;
		tree1->child->parent = tree2;
		tree1->child = tree2;
		tree2->parent = tree1;
		++(tree1->degree);
		return tree1;
	}
public:
	class CIterator{
	BasicTree<TVertexID>* it;
		CIterator(BasicTree<TVertexID>* ref) {
			it = ref;
		}
		BasicTree<TVertexID> operator*()const {
			return it->data;
		}
	};

	BinomialHeap() {}
	BinomialHeap(TVertexID key) {
		this->vectorHeapData.push_back(new BasicTree<TVertexID>(key));
	}
	CIterator Insert(TVertexID key) {
		BinomialHeap heapDef(key);
		CIterator it = new CIterator(heapDef.vectorHeapData[0]);
		meld(&heapDef);
		return it;
	}

	void DecreaseKey(CIterator what, TVertexID newKey) {
		while (true) {
			if (what.it.parent == NULL) {
				return;
			}
			if (*what < what.it->parent.data) {
				BasicTree<TVertexID> bufferCh = what.it->child;
				BasicTree<TVertexID> bufferR = what.it->right;
				what.it->parent = what.it->parent->parent;
				what.it->child = what.it->parent.child;
				what.it->right = what.it->parent.right;
				what.it->parent->parent = what.it;
				what.it->parent.child = bufferCh;
				what.it->parent.right = bufferR;
			}
			else {
				return;
			}
		}
	}

	int ExtractMin() {
		if (this->vectorHeapData.size() == 0) {
			throw EmptyTree();
		}
		TVertexID minKey = this->vectorHeapData[0]->data;
		vector <BasicTree<TVertexID>*> ::iterator it = vectorHeapData.begin();
		int minTreeNumb = 0;
		while (it != vectorHeapData.end())
		{
			if ((*it)->data < minKey)
			{
				minKey = (*it)->data;
				minTreeNumb = it - vectorHeapData.begin();
			}
			++it;
		}
		BinomialHeap<TVertexID> addedHeapWithoutExtractedMin;
		addedHeapWithoutExtractedMin.vectorHeapData.clear();
		BasicTree<TVertexID>* minTreeIt = vectorHeapData[minTreeNumb];
		if (minTreeIt->child != NULL)
		{
			minTreeIt = minTreeIt->child;
			while (minTreeIt != NULL)
			{
				BasicTree<TVertexID>* buffer = new BasicTree<TVertexID>;
				buffer = minTreeIt->right;
				minTreeIt->right = NULL;
				buffer->parent = minTreeIt->parent;
				minTreeIt->parent = NULL;
				addedHeapWithoutExtractedMin.vectorHeapData.push_back(minTreeIt);
				minTreeIt = buffer;
			}
			std::reverse(addedHeapWithoutExtractedMin.vectorHeapData.begin(), addedHeapWithoutExtractedMin.vectorHeapData.end());
		}
		vectorHeapData.erase(vectorHeapData.begin() + minTreeNumb);
		meld(&addedHeapWithoutExtractedMin);

		return minKey;
	}

	void meld(CMeldableHeap<TVertexID>* addedHeapExc)
	{
		if (addedHeapExc == NULL)
		{
			return;
		}
		BinomialHeap <TVertexID>* addedHeap = dynamic_cast <BinomialHeap<TVertexID>*> (addedHeapExc);
		if (addedHeap == NULL)
		{
			throw BadCastEXC();
		}

		vector <BasicTree<TVertexID>*> bufferHeapData = this->vectorHeapData;
		this->vectorHeapData.clear();
		vector <BasicTree<TVertexID>*>::iterator firstHeapIt = bufferHeapData.begin();
		vector <BasicTree<TVertexID>*>::iterator secondHeapIt = addedHeap->vectorHeapData.begin();
		BasicTree<TVertexID>* currentTreeRadical = NULL;
		bool frangeCondition;
		while (!(firstHeapIt == bufferHeapData.end() && secondHeapIt == addedHeap->vectorHeapData.end()))
		{
			frangeCondition = false;
			if (firstHeapIt == bufferHeapData.end())
			{
				frangeCondition = true;
			}

			if (frangeCondition == false && (secondHeapIt == addedHeap->vectorHeapData.end() || (*firstHeapIt)->degree < (*secondHeapIt)->degree))
			{
				if (currentTreeRadical == NULL || currentTreeRadical->degree < (*firstHeapIt)->degree)
				{
					if (currentTreeRadical != NULL)
					{
						this->vectorHeapData.push_back(currentTreeRadical);
						currentTreeRadical = NULL;
					}
					this->vectorHeapData.push_back(*firstHeapIt);
				}
				else
				{
					currentTreeRadical = meldSimilar(currentTreeRadical, *firstHeapIt);
				}
				++firstHeapIt;
				continue;
			}
			if (firstHeapIt == bufferHeapData.end() || (*firstHeapIt)->degree != (*secondHeapIt)->degree)
			{
				if (currentTreeRadical == NULL || currentTreeRadical->degree < (*secondHeapIt)->degree)
				{
					if (currentTreeRadical != NULL)
					{
						this->vectorHeapData.push_back(currentTreeRadical);
						currentTreeRadical = NULL;
					}
					this->vectorHeapData.push_back(*secondHeapIt);
				}
				else
				{
					currentTreeRadical = meldSimilar(currentTreeRadical, *secondHeapIt);
				}
				++secondHeapIt;
				continue;
			}
			if (currentTreeRadical != NULL)
			{
				this->vectorHeapData.push_back(currentTreeRadical);
			}
			currentTreeRadical = meldSimilar(*firstHeapIt, *secondHeapIt);
			++firstHeapIt;
			++secondHeapIt;
		}
		if (currentTreeRadical != NULL)
		{
			this->vectorHeapData.push_back(currentTreeRadical);
		}
	}

	int getMin() {
		if (this->vectorHeapData.size() == 0) {
			throw EmptyTree();
		}
		TVertexID minKey = (this->vectorHeapData[0])->data;
		vector <BasicTree<TVertexID>*> ::iterator it = vectorHeapData.begin();
		int minTreeNumb = 0;
		while (it != vectorHeapData.end())
		{
			if ((*it)->data < minKey)
			{
				minKey = (*it)->data;
				minTreeNumb = it - vectorHeapData.begin();
			}
			++it;
		}
	}

	~BinomialHeap() {
		for (auto i : vectorHeapData) {
			delete i;
		}
	}

	
};



#endif