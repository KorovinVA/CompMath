#pragma once
#include <vector>

class Node
{
public:
	Node(int val, size_t ident);
public:
	struct Vertex
	{
		Node* node;
		size_t weigth;
	};
	std::vector<Vertex> childs;
	Node* parent;

	int m_value;
	int identifier;
	bool DijkstraFlag;
	bool PrimFlag;
};