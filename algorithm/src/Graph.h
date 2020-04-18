#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "Node.h"

enum GraphsExamples
{
	EXAMPLE_001,
	EXAMPLE_002
};

class Graph
{
public:
	Graph(GraphsExamples type);

	void PrintGraph(std::ofstream& stream, bool isDebug);
	void UseDijkstra(std::ofstream& stream);
private:
	std::vector<std::unique_ptr<Node>> elements;

	void CreateGraph(GraphsExamples type);
	void AddVertex(std::vector<size_t>& distance, int weight, Node* root);
	void DijkstraDebug(std::ofstream& stream, std::vector<size_t>& distance);
};