#include <iostream>
#include <fstream>
#include <string>
#include "RBtree.h"

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

int main(int argc, char* argv[])
{
	assert(argv[1]);
	std::string ResultDir(argv[1]);
	testRBtree(ResultDir);
	return 0;
}