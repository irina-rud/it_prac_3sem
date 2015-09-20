#ifndef CMYGRAPH_INCLUDED
#define CMYGRAPH_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <stack>
#include <fstream>



using namespace std;

class IteratorAtEnd{};


template <class TVertexID, class TWeight = int>  class CMyGraph  {

public:
    class CVertexIterator {
        friend class CMyGraph;
    protected:
        CMyGraph* master;
        set <TVertexID> seen;
        set <TVertexID> notseeen;
        TVertexID v;
        stack <TVertexID> dfs;
        set <TVertexID> indfs;
        int init;// 1- valid, 0- opposite;
        int ifEnd; //1-end, 0- not reached;
    public:
        CVertexIterator(CMyGraph& m): master(&m), init(1), ifEnd(0){
            v = *master->vertexes.begin();
            master->initVertexesIterators.push_back(this);
            notseeen = (master->vertexes);
        }

        CVertexIterator(CMyGraph* m): master(m), init(1), ifEnd(0){
            v = *master->vertexes.begin();
            #ifdef DEBUG_MODE
                cout << "mimimi"<<v<<"mimimi"<< endl;
            #endif // DEBUG_MODE
            master->initVertexesIterators.push_back(this);
            notseeen = (master->vertexes);
        }

        CVertexIterator(const CVertexIterator& from):indfs(from.indfs), master(from.master), v(from.v), seen(from.seen), notseeen(from.notseeen), dfs(from.dfs), init(from.init), ifEnd(from.ifEnd){
            if (init== 1){
                master->initVertexesIterators.push_back(this);
            }
        }

        CVertexIterator(CVertexIterator&& from):indfs(from.indfs), master(from.master), v(from.v), seen(from.seen), notseeen(from.notseeen), dfs(from.dfs), init(from.init), ifEnd(from.ifEnd){
            if (init== 1){
                master->initVertexesIterators.push_back(this);
            }
            from->init = 0;
            master->initVertexesIterators.erase(&from);
        }

        CVertexIterator(CMyGraph* m, TVertexID vert): master(m), init(1), ifEnd(0){
            if (master->vertexes.find(vert) != master->vertexes.end()){
                v = vert;
            }
            else v = *master->vertexes.begin();
            #ifdef DEBUG_MODE
                cout << "mimimi"<<v<<"mimimi"<< endl;
            #endif // DEBUG_MODE
            master->initVertexesIterators.push_back(this);
            notseeen = (master->vertexes);
        }

        CVertexIterator& next(vector<TVertexID>& order){
            if (ifEnd == 1) {
				throw IteratorAtEnd();
			}
			seen.insert(v);
			notseeen.erase(v);
			do {
                if (master->links.find(v) != master->links.end()){
                    for (auto i = master->links.find(v); i != master->links.end() && i->first == v ; i++){
                        if (seen.find(i->second.first) != seen.end()) {
                            continue;
                        }
                        else {

                            if (indfs.find(i->second.first) == indfs.end()){
                                dfs.push(i->second.first);
                            }
                            indfs.insert(i->second.first);
                        }
                    }
				}
				if (!dfs.empty()) {
					v = dfs.top();
					dfs.pop();
					return *this;
				}
				else if (seen.size() == master->vertexes.size()){
					ifEnd = 1;
					return *this;
				}
				else{
                    cout << " end of tree ";
                    for (auto i: order){
                        if (notseeen.find(i) != notseeen.end()){
                            v = i;
                            break;
                        }
                    }
                    return *this;
				}
			} while (!dfs.empty() ||
			v == *master->vertexes.begin());
            return *this;

        }

        CVertexIterator& operator++(int){
            #ifdef DEBUG_MODE
                cout << "I was here"<< endl;
            #endif
            if (ifEnd == 1) {
				throw IteratorAtEnd();
			}
			seen.insert(v);
			notseeen.erase(v);
            #ifdef DEBUG_MODE
                cout << "inserted: "<< v << endl;
            #endif
			do {
                if (master->links.find(v) != master->links.end()){
                    for (auto i = master->links.find(v); i != master->links.end() && i->first == v ; i++){
                        if (seen.find(i->second.first) != seen.end()) {
                            #ifdef DEBUG_MODE
                                cout << "1st"<< endl;
                            #endif
                            continue;
                        }
                        else {

                            if (indfs.find(i->second.first) == indfs.end()){
                                dfs.push(i->second.first);
                                #ifdef DEBUG_MODE
                                        cout << "2st, pushed:"<<  i->second.first <<endl;
                                #endif
                            }
                            indfs.insert(i->second.first);
                        }
                    }
				}
				if (!dfs.empty()) {
                    #ifdef DEBUG_MODE
                        cout << "not empty " << dfs.top()<< endl;
                    #endif
					v = dfs.top();
					dfs.pop();


                    #ifdef DEBUG_MODE
                        cout << "not seen:";
                        for (auto i:notseeen){
                            cout <<i << " ";
                        }
                        cout << endl;
                    #endif // DEBUG_MODE

					return *this;
				}
				else if (seen.size() == master->vertexes.size()){
                    #ifdef DEBUG_MODE
                        cout << "end"<< endl;
                    #endif
					ifEnd = 1;

                    #ifdef DEBUG_MODE
                        cout << "not seen:";
                        for (auto i:notseeen){
                            cout <<i << " ";
                        }
                        cout << endl;
                    #endif // DEBUG_MODE

					return *this;
				}
				else{
                    v = *notseeen.begin();

                    #ifdef DEBUG_MODE
                        cout << "not seen:";
                        for (auto i:notseeen){
                            cout <<i << " ";
                        }
                        cout << endl;
                    #endif // DEBUG_MODE

                    return *this;
				}
			} while (!dfs.empty() ||
			v == *master->vertexes.begin());

			#ifdef DEBUG_MODE
                cout << "not seen:";
                for (auto i:notseeen){
                    cout <<i << " ";
                }
                cout << endl;
            #endif // DEBUG_MODE
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
                return *this;
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
        bool isEnd()const{
            if (ifEnd == 1) return true;
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

private:
    multimap <TVertexID, pair <TVertexID, TWeight>> links;
    set <TVertexID> vertexes;
    vector <CVertexIterator*> initVertexesIterators;
    //list <CEdgeIterator&> initEdgeIterators;


public:
    void AddEdge(TVertexID from, TVertexID to, TWeight weight){
        this->AddVertex(from);
        this->AddVertex(to);
        links.insert(make_pair(from, make_pair(to, weight)));
        for (auto i : initVertexesIterators){
            i->init = 0;
        }
        initVertexesIterators.clear();
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

    CVertexIterator& Begin(TVertexID v) const{
        CVertexIterator* it= new CVertexIterator(const_cast<CMyGraph*>(this), v);
        return *it;
    }

    CVertexIterator& End()const{
        CVertexIterator*  it = new CVertexIterator(const_cast<CMyGraph*>(this));
        do {
            (*it)++;
        } while (it->ifEnd != 1);
        return *it;
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
    CMyGraph* Transpon(){
        CMyGraph* result = new CMyGraph;
        for (auto i : vertexes){
            result->AddVertex(i);
        }
        for (auto i : links){
            (*result).AddEdge(i.second.first, i.first, i.second.second);
        }
        return result;
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
        else if (tmp.size()!= 0){
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
