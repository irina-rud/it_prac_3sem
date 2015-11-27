#ifndef CMYGRAPH_INCLUDED
#define CMYGRAPH_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <stack>
#include <fstream>

#define DEBUG_MODE

using namespace std;

class IteratorAtEnd{};


template <class TVertexID, class TWeight = int>  class CMyGraph  {

public:
    class CVertexIterator {
        friend class CMyGraph;
    protected:
        CMyGraph* master;
        set <TVertexID> seen;
        TVertexID v;
        stack <TVertexID> dfs;
        int init;// 1- valid, 0- opposite;
        int ifEnd; //1-end, 0- not reached;
    public:
        CVertexIterator(CMyGraph& m): master(&m), init(1), ifEnd(0){
            v = *master->vertexes.begin();
            master->initVertexesIterators.push_back(this);
        }

        CVertexIterator(CMyGraph* m): master(m), init(1), ifEnd(0){
            v = *master->vertexes.begin();
            #ifdef DEBUG_MODE
                cout << v;
            #endif // DEBUG_MODE
            master->initVertexesIterators.push_back(this);
        }

        CVertexIterator(const CVertexIterator& from): master(from.master), v(from.v), seen(from.seen), dfs(from.dfs), init(from.init), ifEnd(from.ifEnd){
            if (init== 1){
                master->initVertexesIterators.push_back(this);
            }
        }

        CVertexIterator(CVertexIterator&& from): master(from.master), v(from.v), seen(from.seen), dfs(from.dfs), init(from.init), ifEnd(from.ifEnd){
            if (init== 1){
                master->initVertexesIterators.push_back(this);
            }
            from->init = 0;
            master->initVertexesIterators.erase(&from);
        }

        CVertexIterator& operator++(int){
            if (ifEnd == 1) {
				throw IteratorAtEnd();
			}
			seen.insert(v);
			do {
				for (auto i = master->links.find(v); i->first == v; i++){
					if (seen.find(i->second.first) != seen.end()) {
						continue;
					}
					else {
						dfs.push(v);
						v = i->second.first;
						return *this;
					}
				}
				if (!dfs.empty()) {
					v = dfs.top();
					dfs.pop();
				}
				else {
					ifEnd = 1;
					return *this;
				}
			} while (!dfs.empty() ||
			v == *master->vertexes.begin());
			return *this;
        }

        //CVertexIterator& operator--(int); // I don't think we needin dfs iterators

        const TVertexID& operator*()const{
            if (ifEnd == 1){
                throw IteratorAtEnd();
            }
            return v;
        }


        CVertexIterator& operator = (const CVertexIterator& from){
                master = from.master;
                ifEnd = from.ifEnd;
                init = from.init;
                dfs = from.dfs;
                seen = from.seen;
                v = from.v;
        }

        bool operator == (const CVertexIterator& from){
                if (master == from.master && ifEnd == from.ifEnd && init == from.init && dfs == from.dfs && seen == from.seen && v == from.v){
                    return true;
                }
                else return false;
        }

        bool operator != (const CVertexIterator& from){
                return !(*this == from);
        }

        CVertexIterator& operator = (CVertexIterator&& from){
                auto it = master.initVertexesIterators.find(from);
                master.initVertexesIterators.erase(from);

                master = from.master;
                ifEnd = from.ifEnd;
                init = from.init;
                dfs = from.dfs;
                seen = from.seen;
                v = from.v;

                from.init = 0;
        }

        bool IsValid()const{
            if (init == 1) return true;
            else return false;
        }

    };


    /*class CEdgeIterator {
    public:
        CEdgeIterator();
        CEdgeIterator(const CEdgeIterator&);
        CEdgeIterator(CEdgeIterator&&);

        CEdgeIterator& operator++(int);
        CEdgeIterator& operator--(int);
        bool operator == (const CEdgeIterator) const;
        CEdgeIterator& operator = (const CEdgeIterator&);
        CEdgeIterator& operator = (CEdgeIterator&&);

        const TVertexID& GetSourceVertex() const;
        const TVertexID& GetTargetVertex() const;
        const TWeight& GetWeight();

        bool IsValid() const;
    };*/ //useless type of iterators

public:
    multimap <TVertexID, pair <TVertexID, TWeight>> links;
    set <TVertexID> vertexes;
    vector <CVertexIterator*> initVertexesIterators;
    //list <CEdgeIterator&> initEdgeIterators;


public:
	bool isVertex(TVertexID what) {
		return (vertexes.find(what) != vertexes.end());
	}
    void AddEdge(TVertexID from, TVertexID to, TWeight weight){
        this->AddVertex(from);
        this->AddVertex(to);
        links.insert(make_pair(from, make_pair(to, weight)));
        for (auto i : initVertexesIterators){
            i->init = 0;
        }
        initVertexesIterators.clear();
    }

	void ChangeWeight(TVertexID from, TVertexID to, TWeight diff) {
		bool isfound = false;
		for (auto it = links.find(from); it != links.end() && it->first == from; ++it) {
			if (it->second.first == from) {
				isfound = true;
				break;
			}
		}
		if (!isfound) {
			AddEdge(from, to, diff);
		}
		else {
			TWeight weigh = (it->second.second += diff);
			if (weigh == 0) {
				links.erase(it);
			}
			if (weigh < 0) {
				ChangeWeight(to, from, -1 * weigh);
				links.erase(it);
			}
		}

	}

	TWeight whatWeight(TVertexID from, TVertexID to) {
		bool isfound = false;
		for (auto it = links.find(from); it != links.end() && it->first == from; ++it) {
			if (it->second.first == from) {
				isfound = true;
				break;
			}
		}
		if (!isfound) {
			return 0;
		}
		else {
			return it->second.second;
		}
	}

    void AddVertex(TVertexID what){
        if (vertexes.find(what) == vertexes.end()){
            vertexes.insert(what);
            #ifdef DEBUG_MODE
                cout <<"insertion" <<what<<endl;
            #endif
        }
        for (auto i : initVertexesIterators){
            (*i).init = 0;
        }
        initVertexesIterators.clear();
    }

    CVertexIterator& Begin() const{
        CVertexIterator* it= new CVertexIterator(const_cast<CMyGraph*>(this));
        return *it;
    }

    CVertexIterator& End()const{
        CVertexIterator*  it = new CVertexIterator(const_cast<CMyGraph*>(this));
        do {
            it++;
        } while (it->ifEnd != 1);
        return *it;
    }

	vector<TVertexID> hasPath(TVertexID from, TVertexID to) {
		vector<TVertexID> result;
		result.push_back(from);
		if (from == to) {
			return result;
		}
		for (auto i = links.find(from); i != links.end() &&i->first == from ; ++i) {
			vector<TvertexID> oneOf = hasPath(i->second.first, to);
			if (oneOf.end() - 1 == to) {
				for (auto i : oneOf) {
					result.push_back(i);
				}
				return result;
			}
			else {
				oneOf.clear();
			}
		}

	}

    CVertexIterator FindVertex(const TVertexID& what) const{
        for (auto i= this->Begin(); i == this->End(); ++i){
            if (*i == what) return i;
        }
        return this->End();
    }
    void DeleteVertex(CVertexIterator& what){
        if (what.init == 0) return;
        vertexes.erase(vertexes.find(*what));
        links.erase(links.equal_range(*what), links.upper_bound(*what));
        for (auto i : initVertexesIterators ){
            i->init = 0;
        }
        initVertexesIterators.clear();
    }
//template <class TVertex, class TWeig>
    friend istream& operator>>(istream& istr, CMyGraph<string, string>& g);
//template <class TVertex, class TWeig>
    friend ostream& operator<< (ostream& ostr, const CMyGraph<string, string>& g);
//    void DeleteEdge(CEdgeIterator&);

//    CEdgeIterator OutgoingEdgesBegin(const CVertexIterator&) const;
//    CEdgeIterator OutgoingEdgesEnd(const CVertexIterator&) const;

//    void Transpon();
};

template<typename TVertexID, typename TWeight = int>
class CNetwork: public CMyGraph<TVertexID, TWeight> {
public:
	multimap <TVertexID, pair <TVertexID, TWeight>> flow;
	TWeight maxFlow;
	CNetwork() {
		setZeroFlow();
		maxFlow = 0;
	}
	void setZeroFlow(){
		for (multimap<TVertexID, pair <TVertexID, TWeight>>::iterator i = links.begin(); i != links.end(); ++ i) {
			flow.insert(i->first,make_pair( i->second.first, 0));
		}
	}
	
	TWeight getMaxFlow(TVertexID S/*start*/, TVertexID G/*goal*/) {
		maxFlow = 0;
		if (S == G) {
			return 0;
		}
		countMaxFlow(S, G);
		return maxFlow;
	}

	void countMaxFlow(TVertexID S/*start*/, TVertexID G/*goal*/) {
		while (1) {
			vector<TVertexID> path = hasPath(S, G);
			if (path.empty()) {
				break;
			}
			TWeight max = whatWeight(path.begin(), path.begin() + 1);
			for (auto i = path.begin() + 1; i != path.end() - 1; ++i) {
				max = std::fmax(max, whatWeight(i, i + 1));
			}
			maxFlow += max;
			for (auto i = path.begin(); i != path.end() - 1; ++i) {
				ChangeWeight(i, i + 1, (-1)*max);
				ChangeWeight(i + 1, i, max);
			}
		}
	}



};

istream& operator>>(istream& istr, CMyGraph<string, string> & g) {
    g.vertexes.clear();
    g.links.clear();
    string tmp;

    while (istr.good()) {
        getline(istr, tmp);
        if ('(' == tmp[0]) {
            string from = tmp.substr(1, tmp.find(',') - 1);
            string to = tmp.substr(tmp.find(',') + 1, tmp.find(')') - tmp.find(',') - 1);
            string value = tmp.substr(tmp.find("=>") + 2);
            g.AddEdge(from, to, value);
        }
        else {
            string vertex = tmp.substr(0, tmp.find(' '));
            g.AddVertex(vertex);
        }
    };
    return istr;
}

//template <class TVertexID, class TWeight = int>
ostream& operator<< (ostream& ostr, const CMyGraph<string,string> & g) {
    for (auto l : g.links) {
        ostr << '(' << l.first << ","<< l.second.first << ")=>" << l.second.second << endl;
    }
    for (auto l : g.vertexes) {
        ostr << l<< endl;
    }
    return ostr << endl;
}

#endif // CMYGRAPH_INCLUDED
