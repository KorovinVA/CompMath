#include <iostream>
#include <benchmark/benchmark.h>

#include "algo.h"
#include "hypergraph.h"
#include "out.h"

//////////////////////////////////////////////////////////////////////////
/// main.cpp
/// File contains main function for Fiduccia–Mattheyses algorithm
///		https://ieeexplore.ieee.org/document/1585498
//////////////////////////////////////////////////////////////////////////

std::ofstream outFd;
static std::string inputFileName;

void testAlgo(benchmark::State& state)
{
	outFd.open(inputFileName + ".part.2");
	Hypergraph graph = Hypergraph(inputFileName);
	Algo FM = Algo(graph);

	for (auto _ : state)
	{
		FM.run();
	}

	FM.print();
	outFd.close();
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "USAGE: ./fm path_to_benchmark" << std::endl;
		exit(1);
	}

	inputFileName = std::string(argv[1]);

	benchmark::RegisterBenchmark("FMAlgo", testAlgo);
	benchmark::RunSpecifiedBenchmarks();
}