#include "gaincontainer.h"
#include "out.h"

GainContainer::GainContainer(const Hypergraph& graph) :
	graph(graph)
{
	gainVec = std::vector<int>(graph.getVertNum());
	initGain();
}

Hypergraph GainContainer::getGraph() const
{
	return graph;
}

MyPair GainContainer::extractMaxGain()
{
	MyPair pair;

	switch (graph.getSide())
	{
	case SIDE::Left:
		pair.first  = left.rbegin()->first;
		pair.second = *(left.rbegin()->second.begin());
		left.rbegin()->second.erase(pair.second);
		if (left.rbegin()->second.empty())
		{
			left.erase(pair.first);
		}
		break;
	case SIDE::Right:
		pair.first = right.rbegin()->first;
		pair.second = *(right.rbegin()->second.begin());
		right.rbegin()->second.erase(pair.second);
		if (right.rbegin()->second.empty())
		{
			right.erase(pair.first);
		}
		break;
	default:
		exit(2);
	}

	return pair;
}

bool GainContainer::isEmpty()
{
	return right.empty() && left.empty() ||
		graph.getSide() == SIDE::Left && left.empty() ||
		graph.getSide() == SIDE::Right && right.empty();
}

void GainContainer::apply(MyPair move)
{
//#ifdef _LOG_
//	outFd << "Current gain apply: " << move.first << std::endl;
//	outFd << "Right: ";
//	for (auto it : right)
//	{
//		outFd << it.first << " ";
//	}
//	outFd << "\nLeft: ";
//	for (auto it : left)
//	{
//		outFd << it.first << " ";
//	}
//	outFd << std::endl;
//#endif // _LOG_

	for (size_t i = 0; i < graph.getVertCellNum(move.second); i++)
	{
		size_t edge = graph.getEdge(move.second, i);

		bool isEqualSides = true;
		bool isSoloVertex = true;
		size_t toDst = 0;
		size_t toSrc = 0;
		size_t dstVertex = 0;
		size_t srcVertex = 0;

		// Loop for all neigbourhoods
		for (size_t j = 0; j < graph.getEdgeCellSize(edge); j++)
		{
			size_t vertex = graph.getVertex(edge, j);


			if (graph.getSide() != graph.getSide(vertex))
			{
				isEqualSides = false;
				toDst++;
				dstVertex = vertex;
			}
			else if (vertex != move.second)
			{
				toSrc++;
				srcVertex = vertex;
			}

			if ((graph.getSide(vertex) == graph.getSide(move.second)) && (vertex != move.second))
			{
				isSoloVertex = false;
			}
		}

		if (isEqualSides)
		{
			for (size_t j = 0; j < graph.getEdgeCellSize(edge); j++)
			{
				size_t vertex = graph.getVertex(edge, j);
				update(vertex, true);
			}
		}
		if (isSoloVertex)
		{
			for (size_t j = 0; j < graph.getEdgeCellSize(edge); j++)
			{
				size_t vertex = graph.getVertex(edge, j);
				update(vertex, false);
			}
		}

		if (toSrc == 1)
		{
			update(srcVertex, true);
		}
		if (toDst == 1)
		{
			update(dstVertex, false);
		}
	}
	erase(move.second);
	locked.insert(move.second);

	graph.changeSide(move.second);
	graph.changeSide();
}

void GainContainer::update(size_t vertex, bool isIncrease)
{
	if (locked.count(vertex))
	{
		return;
	}
	erase(vertex);
	if (isIncrease)
	{
		gainVec[vertex] += 1;
	}
	else
	{
		gainVec[vertex] -= 1;
	}

	switch (graph.getSide(vertex))
	{
	case SIDE::Left:
		left[gainVec[vertex]].insert(vertex);
		break;
	case SIDE::Right:
		right[gainVec[vertex]].insert(vertex);
		break;
	default:
		exit(2);
	}
}

void GainContainer::erase(size_t vertex)
{
	switch (graph.getSide(vertex))
	{
	case SIDE::Left:
		left[gainVec[vertex]].erase(vertex);
		if (left[gainVec[vertex]].empty())
		{
			left.erase(gainVec[vertex]);
		}
		break;
	case SIDE::Right:
		right[gainVec[vertex]].erase(vertex);
		if (right[gainVec[vertex]].empty())
		{
			right.erase(gainVec[vertex]);
		}
		break;
	default:
		exit(2);
	}
}

void GainContainer::initGain()
{
	left.clear();
	right.clear();

	for (size_t i = 0; i < graph.getVertNum(); ++i)
	{
		int gain = 0;
		SIDE side = graph.getSide(i);

		for (size_t j = 0; j < graph.getVertCellNum(i); ++j)
		{
			size_t edgeId = graph.getEdge(i, j);

			// For every vertex there are 3 possibilities for each edge it is included
			// gain += 1, if the vertex is alone in left/right side => after transfering it there will be no side crossing
			// gain -= 1, if the vertex is on the same side as other vertices
			// gain  = 0, if there some other vertices on both sides

			bool isIncrementGain = true;
			bool isDecrementGain = true;

			for (size_t k = 0; k < graph.getEdgeCellSize(edgeId); ++k)
			{
				size_t vertId = graph.getVertex(edgeId, k);

				if (side != graph.getSide(vertId))
				{
					isDecrementGain = false;
				}
				if (side == graph.getSide(vertId) && vertId != i)
				{
					isIncrementGain = false;
				}
			}
			gain = gain + isIncrementGain - isDecrementGain;
		}

		if (side == SIDE::Left)
		{
			left[gain].insert(i);
		}
		else
		{
			right[gain].insert(i);
		}

		gainVec[i] = gain;
	}
}



