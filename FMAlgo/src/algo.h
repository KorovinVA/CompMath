#pragma once

#include "hypergraph.h"
#include "gaincontainer.h"

class Algo
{
public:
	Algo(const Hypergraph& graph);
	~Algo();

	void run();
	void print();
private:
	int pass(GainContainer& gc);

	Hypergraph graph;
};