#include "hypergraph.h"
#include "out.h"

Hypergraph::Hypergraph(const std::string& fileName)
{
#ifdef _LOG_
	outFd << "Creating Hypergraph..." << std::endl;
#endif // _LOG_

	std::ifstream fd;
	fd.open(fileName);

	size_t vertNum = 0;
	size_t edgeNum = 0;

	fd >> edgeNum >> vertNum;
	verts        = std::vector<std::vector<int>>(vertNum);
	indexPartion = std::vector<SIDE>(vertNum);
	edges        = std::vector<std::vector<int>>(edgeNum);

#ifdef _LOG_
	outFd << "Vertices:   " << vertNum << std::endl;
	outFd << "Edges:      " << edgeNum << std::endl;
#endif // _LOG_

	std::string line;
	int edgeCount = 0;
	int vertCurr = 0;
	int skipInit = true;

	while (std::getline(fd, line))
	{
		if (skipInit)
		{
			skipInit = false;
			continue;
		}

		std::istringstream stream(line);
		while (stream >> vertCurr)
		{
			vertCurr -= 1;

			verts[vertCurr].push_back(edgeCount);
			edges[edgeCount].push_back(vertCurr);
		}
		edgeCount++;
	}

#ifdef _LOG_
	outFd << "Edges read: " << edgeCount << std::endl;
#endif // _LOG_

	fd.close();

	initPartions();
	calculateCut();
}

Hypergraph::~Hypergraph()
{}

void Hypergraph::initPartions()
{
	// Move the first half of vertices to left and remaining to the right
	for (size_t i = 0; i <= verts.size() / 2; ++i)
	{
		indexPartion[i] = SIDE::Left;
	}
	for (size_t i = verts.size() / 2 + 1; i < verts.size(); ++i)
	{
		indexPartion[i] = SIDE::Right;
	}
}

void Hypergraph::print()
{
	// In the constructor we reduced the size of the vertices by one.
	// So, now we must get the one back

#ifdef _LOG_
	outFd << "Left:" << std::endl;
	for (size_t i = 0; i < indexPartion.size(); ++i)
	{
		if (indexPartion[i] == SIDE::Left)
		{
			outFd << i + 1 << std::endl;
		}
	}
	outFd << "\nRight:" << std::endl;
	for (size_t i = 0; i < indexPartion.size(); ++i)
	{
		if (indexPartion[i] == SIDE::Right)
		{
			outFd << i + 1 << std::endl;
		}
	}
#endif // _LOG_
}

void Hypergraph::out()
{
	for (auto it : indexPartion)
	{
		outFd << static_cast<uint32_t>(it) << "\n";
	}
}

void Hypergraph::calculateCut()
{
	cut = 0;
	for (size_t i = 0; i < edges.size(); ++i)
	{
		SIDE side = indexPartion[edges[i][0]];
		for (size_t j = 0; j < edges[i].size(); ++j)
		{
			if (side != indexPartion[edges[i][j]])
			{
				// Another vertex within the same edge is on the other side -> boundary crossing
				cut++;
				break;
			}
		}
	}
}