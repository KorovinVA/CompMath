#pragma once

#include <assert.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <utility>

static int uniq = 0;

class Graph
{
public:
	Graph();
	void PrintGraph(std::ofstream& stream, bool isDebug);
	void UseDijkstra(std::ofstream& stream);
	Graph* getChild(int child) const;
	~Graph();
private:
	struct Vertex
	{
		Graph* node;
		int weigth;
	};

	int m_value;
	int Identifier;
	bool isprinted;
	std::vector<Vertex> childs;

	bool DijkstraFlag;

	Graph(int val);

	void CreateGraph();
	void PrintDot(std::ofstream& stream, bool isDebug, std::vector<std::pair<int, int>>& CurrentConnections);
	bool findPair(int x, int y, std::vector<std::pair<int, int>>& CurrentConnections);
	void AddVertex(std::vector<int>& distance, int weight);
	friend static void FillValues();
};

static std::vector<std::unique_ptr<Graph>> values;
static void FillValues()
{
	values.push_back(std::unique_ptr<Graph>(new Graph(1)));
	values.push_back(std::unique_ptr<Graph>(new Graph(6)));
	values.push_back(std::unique_ptr<Graph>(new Graph(3)));
	values.push_back(std::unique_ptr<Graph>(new Graph(2)));
	values.push_back(std::unique_ptr<Graph>(new Graph(5)));
	values.push_back(std::unique_ptr<Graph>(new Graph(4)));
};

Graph::Graph()
{
	FillValues();
	CreateGraph();
}

Graph::Graph(int val)
{ 
	Identifier = uniq;
	uniq++;
	m_value = val;
	isprinted = false;
	DijkstraFlag = false;
}

void Graph::CreateGraph()
{
	m_value = values[0].get()->m_value;

	childs.push_back({ values[1].get(), 14 });
	childs.push_back({ values[2].get(), 9 });
	childs.push_back({ values[3].get(), 7 });

	childs[0].node->childs.push_back({ values[0].get(), 14 });
	childs[0].node->childs.push_back({ values[2].get(), 2 });
	childs[0].node->childs.push_back({ values[4].get(), 9 });

	childs[1].node->childs.push_back({ values[1].get(), 2 });
	childs[1].node->childs.push_back({ values[0].get(), 9 });
	childs[1].node->childs.push_back({ values[3].get(), 10 });
	childs[1].node->childs.push_back({ values[5].get(), 11 });

	childs[2].node->childs.push_back({ values[0].get(), 7 });
	childs[2].node->childs.push_back({ values[2].get(), 10 });
	childs[2].node->childs.push_back({ values[5].get(), 15 });

	Graph *FirstNode = childs[0].node->childs[2].node;
	FirstNode->childs.push_back({ values[1].get(), 9 });
	FirstNode->childs.push_back({ values[5].get(), 6 });

	Graph *LastNode = FirstNode->childs[1].node;
	LastNode->childs.push_back({ values[4].get(), 6 });
	LastNode->childs.push_back({ values[2].get(), 11 });
	LastNode->childs.push_back({ values[3].get(), 15 });
}

Graph* Graph::getChild(int child) const
{
	return childs[child].node;
}

void Graph::UseDijkstra(std::ofstream& stream)
{
	std::vector<int> distance;
	for (auto i = values.begin(); i != values.end(); i++)
	{
		distance.push_back(std::numeric_limits<int>::max());
	}

	distance[Identifier] = 0;
	values.at(Identifier).get()->DijkstraFlag = true;
	AddVertex(distance, 0);

	for (auto i = values.begin(); i != values.end(); i++)
	{
		i->get()->DijkstraFlag = false;
	}

	stream << "digraph G{ \n" << "node [style=filled, fontcolor=black];" << std::endl;
	int j = 0;
	for (auto i = values.begin(); i != values.end(); i++)
	{
		stream << "n" << i->get()->Identifier;
		stream << "[label=\"" << i->get()->m_value << "\"" << ", color=yellow];" << std::endl;
	}
	for (auto i = values.begin(); i != values.end(); i++, j++)
	{
		if(Identifier != i->get()->Identifier)
			stream << "n" << Identifier << " -> n" << i->get()->Identifier << " [label=\"" << distance[j] << "\"]" << std::endl;
	}
	stream << "}" << std::endl;
}

void Graph::AddVertex(std::vector<int>& distance, int weight)
{
	struct S
	{
		int ident;
		int weight;
	} NextVertex{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

	for (auto i : childs)
	{
		if (distance[i.node->Identifier] > i.weigth + weight && i.node->DijkstraFlag == false)
			distance[i.node->Identifier] = i.weigth + weight;
	}

	int j = 0;
	for (auto it = values.begin(); it != values.end(); it++, j++)
	{
		if (it->get()->DijkstraFlag == false)
		{
			if ((unsigned int)NextVertex.weight > (unsigned int)(distance[j]))
			{
				NextVertex.weight = distance[j];
				NextVertex.ident = j;
			}
		}
	}
	values.at(NextVertex.ident).get()->DijkstraFlag = true;
	
	bool endrecursion = true;
	for (auto it = values.begin(); it != values.end(); it++)
	{
		if (it->get()->DijkstraFlag == false)
		{
			endrecursion = false;
			break;
		}
	}

	if(!endrecursion) values.at(NextVertex.ident).get()->AddVertex(distance, NextVertex.weight);
}

void Graph::PrintGraph(std::ofstream& stream, bool isDebug)
{
	stream << "digraph G{ \n" << "node [style=filled, fontcolor=black];" << std::endl;
	std::vector<std::pair<int, int>> connections;
	PrintDot(stream, isDebug, connections);
	stream << "}" << std::endl;
}

void Graph::PrintDot(std::ofstream& stream, bool isDebug, std::vector<std::pair<int, int>>& CurrentConnections)
{
	stream << "n" << Identifier;
	stream << "[label=\"" << m_value << "\"" << ", color=yellow];" << std::endl;
	isprinted = true;
	for (auto i : childs)
	{
		if(!(i.node->isprinted)) 
			i.node->PrintDot(stream, isDebug, CurrentConnections);

		if (isDebug)
			stream << "n" << Identifier << " -> n" << i.node->Identifier << " [label=\"" << i.weigth << "\"]" << std::endl;
		else
		{
			if (!findPair(Identifier, i.node->Identifier, CurrentConnections))
			{
				stream << "n" << Identifier << " -> n" << i.node->Identifier << " [arrowhead=none, label=\"" << i.weigth << "\"]" << std::endl;
				CurrentConnections.push_back({ Identifier , i.node->Identifier });
			}
		}
	}
}

bool Graph::findPair(int x, int y, std::vector<std::pair<int, int>>& CurrentConnections)
{
	for (auto i : CurrentConnections)
	{
		if ((x == i.first && y == i.second) ||
			(x == i.second && y == i.first))
			return true;
	}
	return false;
}

Graph::~Graph()
{}