#include <cassert>
#include <fstream>
#include <limits>

#include "Graph.h"

bool findPair(int x, int y, std::vector<std::pair<int, int>>& CurrentConnections)
{
	for (auto i : CurrentConnections)
	{
		if ((x == i.first && y == i.second) ||
			(x == i.second && y == i.first))
			return true;
	}
	return false;
}

Graph::Graph(GraphsExamples type)
{
	size_t ident = -1;
	switch (type)
	{
	case EXAMPLE_001:
		elements.push_back(std::unique_ptr<Node>(new Node(1, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(2, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(3, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(4, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(5, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(6, ++ident)));
		break;
	case EXAMPLE_002:
		elements.push_back(std::unique_ptr<Node>(new Node(1, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(2, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(3, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(4, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(5, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(6, ++ident)));
		elements.push_back(std::unique_ptr<Node>(new Node(7, ++ident)));
		break;
	default:
		assert(false);
		break;
	}
	CreateGraph(type);
}

void Graph::CreateGraph(GraphsExamples type)
{
	switch (type)
	{
	case EXAMPLE_001:
		elements[0].get()->childs.push_back({ elements[5].get(), 14 });
		elements[0].get()->childs.push_back({ elements[2].get(), 9 });
		elements[0].get()->childs.push_back({ elements[1].get(), 7 });

		elements[1].get()->childs.push_back({ elements[0].get(), 7 });
		elements[1].get()->childs.push_back({ elements[2].get(), 10 });
		elements[1].get()->childs.push_back({ elements[3].get(), 15 });

		elements[2].get()->childs.push_back({ elements[0].get(), 9 });
		elements[2].get()->childs.push_back({ elements[5].get(), 2 });
		elements[2].get()->childs.push_back({ elements[3].get(), 11 });
		elements[2].get()->childs.push_back({ elements[1].get(), 10 });

		elements[3].get()->childs.push_back({ elements[4].get(), 6 });
		elements[3].get()->childs.push_back({ elements[2].get(), 11 });
		elements[3].get()->childs.push_back({ elements[1].get(), 15 });

		elements[4].get()->childs.push_back({ elements[5].get(), 9 });
		elements[4].get()->childs.push_back({ elements[3].get(), 6 });

		elements[5].get()->childs.push_back({ elements[0].get(), 14 });
		elements[5].get()->childs.push_back({ elements[2].get(), 2 });
		elements[5].get()->childs.push_back({ elements[4].get(), 9 });
		break;
	case EXAMPLE_002:
		elements[0].get()->childs.push_back({ elements[1].get(), 7 });
		elements[0].get()->childs.push_back({ elements[6].get(), 5 });

		elements[1].get()->childs.push_back({ elements[0].get(), 7 });
		elements[1].get()->childs.push_back({ elements[6].get(), 9 });
		elements[1].get()->childs.push_back({ elements[2].get(), 8 });
		elements[1].get()->childs.push_back({ elements[3].get(), 7 });

		elements[2].get()->childs.push_back({ elements[1].get(), 8 });
		elements[2].get()->childs.push_back({ elements[3].get(), 5 });

		elements[3].get()->childs.push_back({ elements[2].get(), 5 });
		elements[3].get()->childs.push_back({ elements[1].get(), 7 });
		elements[3].get()->childs.push_back({ elements[6].get(), 15 });
		elements[3].get()->childs.push_back({ elements[5].get(), 8 });
		elements[3].get()->childs.push_back({ elements[4].get(), 9 });

		elements[4].get()->childs.push_back({ elements[3].get(), 9 });
		elements[4].get()->childs.push_back({ elements[5].get(), 11 });

		elements[5].get()->childs.push_back({ elements[6].get(), 6 });
		elements[5].get()->childs.push_back({ elements[3].get(), 8 });
		elements[5].get()->childs.push_back({ elements[4].get(), 11 });

		elements[6].get()->childs.push_back({ elements[0].get(), 5 });
		elements[6].get()->childs.push_back({ elements[1].get(), 9 });
		elements[6].get()->childs.push_back({ elements[3].get(), 15 });
		elements[6].get()->childs.push_back({ elements[5].get(), 6 });
		break;
	default:
		assert(false);
		break;
	}
}


void Graph::PrintGraph(std::ofstream & stream, bool isDebug)
{
	stream << "digraph G{ \n" << "node [style=filled, fontcolor=black];" << std::endl;
	std::vector<std::pair<int, int>> connections;
	for (auto it = elements.begin(); it != elements.end(); ++it)
	{
		stream << "n" << it->get()->identifier;
		stream << "[label=\"" << it->get()->m_value << "\"" << ", color=yellow];" << std::endl;
	}
	for (auto i = elements.begin(); i != elements.end(); ++i)
	{
		for (auto j : i->get()->childs)
		{
			if (isDebug)
				stream << "n" << i->get()->identifier << " -> n" << j.node->identifier 
				<< " [label=\"" << j.weigth << "\"];" << std::endl;
			else
			{
				if (!findPair(i->get()->identifier, j.node->identifier, connections))
				{
					stream << "n" << i->get()->identifier << " -> n" << j.node->identifier
						<< " [arrowhead=none, label=\"" << j.weigth << "\"];" << std::endl;
					connections.push_back({ i->get()->identifier , j.node->identifier });
				}
			}
		}
	}
	stream << "}" << std::endl;
}

void Graph::UseDijkstra(std::ofstream & stream)
{
	std::vector<size_t> distance;
	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		distance.push_back(std::numeric_limits<size_t>::max());
	}

	distance[0] = 0;
	elements[0].get()->DijkstraFlag = true;
	AddVertex(distance, 0, elements[0].get());

	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		i->get()->DijkstraFlag = false;
	}

	DijkstraOut(stream, distance);
}

void Graph::AddVertex(std::vector<size_t>& distance, int weight, Node* root)
{
	struct S
	{
		size_t ident;
		size_t weight;
	} NextVertex{ std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };

	for (auto it : root->childs)
	{
		if (distance[it.node->identifier] > it.weigth + weight && it.node->DijkstraFlag == false)
			distance[it.node->identifier] = it.weigth + weight;
	}

	int j = 0;
	for (auto it = elements.begin(); it != elements.end(); it++, j++)
	{
		if (it->get()->DijkstraFlag == false)
		{
			if (NextVertex.weight > distance[j])
			{
				NextVertex.weight = distance[j];
				NextVertex.ident = j;
			}
		}
	}
	elements.at(NextVertex.ident).get()->DijkstraFlag = true;

	bool endrecursion = true;
	for (auto it = elements.begin(); it != elements.end(); it++)
	{
		if (it->get()->DijkstraFlag == false)
		{
			endrecursion = false;
			break;
		}
	}
	if (!endrecursion) 
		AddVertex(distance, NextVertex.weight, elements.at(NextVertex.ident).get());
}

void Graph::DijkstraOut(std::ofstream & stream, std::vector<size_t>& distance)
{
	stream << "digraph G{ \n" << "node [style=filled, fontcolor=black];" << std::endl;
	int j = 0;
	for (auto i = elements.begin(); i != elements.end(); i++)
	{
		stream << "n" << i->get()->identifier;
		stream << "[label=\"" << i->get()->m_value << "\"" << ", color=yellow];" << std::endl;
	}
	for (auto i = elements.begin(); i != elements.end(); i++, j++)
	{
		if (elements[0].get()->identifier != i->get()->identifier)
			stream << "n" << elements[0].get()->identifier << " -> n" << i->get()->identifier << " [label=\"" << distance[j] << "\"];" << std::endl;
	}
	stream << "}" << std::endl;
}

void Graph::UsePrim(std::ofstream& stream)
{
	elements[0].get()->PrimFlag = true;
	std::pair<Node*, size_t> check = { nullptr, 0 };

	do
	{
		check = AddVertex();
	} while (check.first != nullptr);

	PrimOut(stream);
	for (auto it = elements.begin(); it != elements.end(); it++)
	{
		it->get()->PrimFlag = false;
		it->get()->parent = nullptr;
	}
}

std::pair<Node*, size_t> Graph::AddVertex()
{
	std::pair<Node*, size_t> next = { nullptr, 0 };

	for (auto it = elements.begin(); it != elements.end(); it++)
	{
		if (it->get()->PrimFlag == false)
			continue;
		for (auto j : it->get()->childs)
		{
			if (next.first == nullptr && j.node->PrimFlag == false)
			{
				next.first = j.node;
				next.second = j.weigth;
				next.first->parent = it->get();
			}
			else if (next.second > j.weigth && j.node->PrimFlag == false)
			{
				next.first = j.node;
				next.second = j.weigth;
				next.first->parent = it->get();
			}
		}
	}
	
	if(next.first) 
		next.first->PrimFlag = true;

	return next;
}

void Graph::PrimOut(std::ofstream& stream)
{
	stream << "digraph G{ \n" << "node [style=filled, fontcolor=black];" << std::endl;
	for (auto it = elements.begin(); it != elements.end(); ++it)
	{
		stream << "n" << it->get()->identifier;
		stream << "[label=\"" << it->get()->m_value << "\"" << ", color=yellow];" << std::endl;
	}
	for (auto i = elements.begin(); i != elements.end(); ++i)
	{
		for (auto j : i->get()->childs)
		{
			if (j.node->parent == nullptr)
				continue;
			if(j.node->parent->identifier == i->get()->identifier)
				stream << "n" << i->get()->identifier << " -> n" << 
				j.node->identifier << " [label=\"" << j.weigth << "\"];" << std::endl;
		}
	}
	stream << "}" << std::endl;
}