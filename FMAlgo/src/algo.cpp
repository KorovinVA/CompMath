#include "algo.h"
#include "gaincontainer.h"
#include "out.h"

Algo::Algo(const Hypergraph& graph) :
	graph(graph)
{}

Algo::~Algo()
{}

void Algo::run()
{
	bool isImproved = true;
#ifdef _LOG_
	GainContainer init(graph);
	outFd << "Initial cut: " << init.getCut() << std::endl;
#endif // _LOG_

    while (isImproved)
    {
		GainContainer gc(graph);
		int best = pass(gc);

		if (best >= gc.getCut())
		{
			isImproved = false;
		}
		else
		{
			gc.setCut(best);
		}

		graph = gc.getGraph();

#ifdef _LOG_
		outFd << "Cut: " << best << std::endl;
#endif // _LOG_
	}
}

void Algo::print()
{
	graph.print();
}

int Algo::pass(GainContainer& gc)
{
	int cost = gc.getCut();
	int best = cost;
	std::set<size_t> changeSide;

	while (!gc.isEmpty())
	{
		auto highest = gc.extractMaxGain();
		cost -= highest.first;
		changeSide.insert(highest.second);
		if (cost < best)
		{
			best = cost;
			changeSide.clear();
		}
		gc.apply(highest);
	}

	for (auto it : changeSide)
	{
		gc.changeSide(it);
	}
	return best;
}
