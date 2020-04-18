#include <iostream>
#include <fstream>
#include <string>
#include "RBtree.h"
#include "Graph.h"

void testRBtree(std::string resultdir)
{
	RBtree* tree1 = new RBtree(22);
	for (int i = 0; i < 22; ++i)
	{
		tree1 = tree1->Insert(i);
	}
	std::ofstream RBtreeExample1(resultdir + "RBtree_001.dot", std::ofstream::out);
	assert(RBtreeExample1.is_open());
	tree1->PrintGraph(RBtreeExample1);
	RBtreeExample1.close();


	RBtree* tree2 = new RBtree(0);
	for (int i = 0; i < 50; ++i)
	{
		tree2 = tree2->Insert(0);
	}
	std::ofstream RBtreeExample2(resultdir + "RBtree_002.dot", std::ofstream::out);
	assert(RBtreeExample2.is_open());
	tree2->PrintGraph(RBtreeExample2);
	RBtreeExample2.close();
}

void testDijkstra(std::string resultdir)
{
	bool Debug = false;

	Graph* graph_1 = new Graph(GraphsExamples::EXAMPLE_001);
	std::ofstream DijkstraBefore001(resultdir + "Before_Dijkstra_001.dot", std::ofstream::out);
	assert(DijkstraBefore001.is_open());
	graph_1->PrintGraph(DijkstraBefore001, Debug);

	std::ofstream DijkstraResult001(resultdir + "Result_Dijkstra_001.dot", std::ofstream::out);
	assert(DijkstraResult001.is_open());
	graph_1->UseDijkstra(DijkstraResult001);

	Graph* graph_2 = new Graph(GraphsExamples::EXAMPLE_002);
	std::ofstream DijkstraBefore002(resultdir + "Before_Dijkstra_002.dot", std::ofstream::out);
	assert(DijkstraBefore002.is_open());
	graph_2->PrintGraph(DijkstraBefore002, Debug);

	std::ofstream DijkstraResult002(resultdir + "Result_Dijkstra_002.dot", std::ofstream::out);
	assert(DijkstraResult002.is_open());
	graph_2->UseDijkstra(DijkstraResult002);

	delete graph_1;
	delete graph_2;

	DijkstraBefore001.close();
	DijkstraBefore002.close();
	DijkstraResult001.close();
	DijkstraResult002.close();
}

void testPrim(std::string resultdir)
{
	bool Debug = false;

	Graph* graph_1 = new Graph(GraphsExamples::EXAMPLE_001);
	std::ofstream PrimBefore001(resultdir + "Before_Prim_001.dot", std::ofstream::out);
	assert(PrimBefore001.is_open());
	graph_1->PrintGraph(PrimBefore001, Debug);

	std::ofstream PrimResult001(resultdir + "Result_Prim_001.dot", std::ofstream::out);
	assert(PrimResult001.is_open());
	graph_1->UsePrim(PrimResult001);

	Graph* graph_2 = new Graph(GraphsExamples::EXAMPLE_002);
	std::ofstream PrimBefore002(resultdir + "Before_Prim_002.dot", std::ofstream::out);
	assert(PrimBefore002.is_open());
	graph_2->PrintGraph(PrimBefore002, Debug);

	std::ofstream PrimResult002(resultdir + "Result_Prim_002.dot", std::ofstream::out);
	assert(PrimResult002.is_open());
	graph_2->UsePrim(PrimResult002);

	delete graph_1;
	delete graph_2;

	PrimBefore001.close();
	PrimBefore002.close();
	PrimResult001.close();
	PrimResult002.close();
}

int main(int argc, char* argv[])
{
	assert(argv[1]);
	std::string ResultDir(argv[1]);
	testRBtree(ResultDir);
	testDijkstra(ResultDir);
	testPrim(ResultDir);
	return 0;
}