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
	size_t degree;

	BasicTree() {}
	BasicTree(TVertexID key) {
		data = key;
		child = NULL;
		right = NULL;
		degree = 0;
	}
	BasicTree(TVertexID key, size_t currentDegree) {
		data = key;
		child = NULL;
		right = NULL;
		degree = currentDegree;
	}
	~BasicTree() {}

	/*ostream& operator<<(ostream& ostr, CBasicTree& t) {
		
		if (t == NULL)
		{
			return;
		}
		for (int i = 0; i < 2 * t->degree; ++i)
			ostr << " ";
		ostr << t->data << " " << degree << "	";
		if (t->right != NULL)  ostr << t->right;
		ostr << endl;
		if (t->child != NULL) ostr << t->child;
		
	}*/
};

template <class TVertexID>
class CMeldableHeap {
public:
	CMeldableHeap() {}

	virtual void Insert(int key) = 0;
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
		tree1->child = tree2;
		++(tree1->degree);
		tree2->~BasicTree();
		return tree1;
	}
public:
	BinomialHeap() {}
	BinomialHeap(TVertexID key) {
		this->vectorHeapData.push_back(new BasicTree<TVertexID>(key));
	}
	void Insert(TVertexID key) {
		BinomialHeap heapDef(key);
		meld(&heapDef);
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

	~BinomialHeap() {}
};



#endif