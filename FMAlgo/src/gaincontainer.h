#pragma once

#include <map>
#include <list>
#include <set>

#include "hypergraph.h"

typedef std::pair<int, size_t> MyPair;
class GainContainer
{
public:
    GainContainer(const Hypergraph& graph);

    inline int  getCut() const;
    inline void setCut(int newVal);
    inline void changeSide(size_t vertex);

    Hypergraph getGraph() const;
    MyPair extractMaxGain();
    bool isEmpty();
    void apply(MyPair move);

    ~GainContainer() {}
private:
    void update(size_t vertex, bool isIncrease);
    void erase(size_t vertex);
    void initGain();

    std::map<int, std::set<size_t>> left;
    std::map<int, std::set<size_t>> right;
    std::set<size_t> locked;
    std::vector<int> gainVec;

    Hypergraph graph;
};

inline int GainContainer::getCut() const
{
    return graph.cut;
}

inline void GainContainer::setCut(int newVal)
{
    graph.cut = newVal;
}

inline void GainContainer::changeSide(size_t vertex)
{
    graph.changeSide(vertex);
}
