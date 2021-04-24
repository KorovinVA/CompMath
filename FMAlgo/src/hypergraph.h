#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>

enum class SIDE
{
	Left,
	Right
};

class Hypergraph
{
public:
	int cut = 0;
public:
	Hypergraph(const std::string& fileName);
	~Hypergraph();

	void calculateCut();

	inline size_t getVertNum() const;
	inline size_t getEdgeNum() const;

	inline size_t getVertex(size_t row, size_t column) const;
	inline size_t getEdge(size_t row, size_t column) const;

	inline size_t getVertCellNum(size_t row) const;
	inline size_t getEdgeCellSize(size_t row) const;

	inline SIDE getSide(size_t index) const;
	inline SIDE getSide() const;

	inline void changeSide();
	inline void changeSide(size_t vert);

	Hypergraph& operator=(const Hypergraph& oth);
	void print();

private:
	void initPartions();

	std::vector<std::vector<int>> verts;
	std::vector<std::vector<int>> edges;
	std::vector<SIDE> indexPartion;

	SIDE workingSide = SIDE::Left;
};

inline size_t Hypergraph::getVertNum() const
{
	return verts.size();
}

inline size_t Hypergraph::getEdgeNum() const
{
	return edges.size();
}

inline size_t Hypergraph::getEdge(size_t row, size_t column) const
{
	return verts[row][column];
}

inline size_t Hypergraph::getVertex(size_t row, size_t column) const
{
	return edges[row][column];
}

inline size_t Hypergraph::getVertCellNum(size_t row) const
{
	return verts[row].size();
}

inline size_t Hypergraph::getEdgeCellSize(size_t row) const
{
	return edges[row].size();
}

inline SIDE Hypergraph::getSide(size_t index) const
{
	return indexPartion[index];
}

inline SIDE Hypergraph::getSide() const
{
	return workingSide;
}

inline void Hypergraph::changeSide()
{
	switch (workingSide)
	{
	case SIDE::Left:
		workingSide = SIDE::Right;
		break;
	case SIDE::Right:
		workingSide = SIDE::Left;
		break;
	default:
		exit(2);
	}
}

inline void Hypergraph::changeSide(size_t vert)
{
	switch (indexPartion[vert])
	{
	case SIDE::Left:
		indexPartion[vert] = SIDE::Right;
		break;
	case SIDE::Right:
		indexPartion[vert] = SIDE::Left;
		break;
	default:
		exit(2);
	}
}

inline Hypergraph& Hypergraph::operator=(const Hypergraph& oth)
{
	cut = oth.cut;
	workingSide = oth.workingSide;
	verts = std::vector<std::vector<int>>(oth.verts.size());
	edges = std::vector<std::vector<int>>(oth.edges.size());
	indexPartion = std::vector<SIDE>(oth.indexPartion.size());

	for (size_t i = 0; i < oth.verts.size(); ++i)
	{
		verts[i] = oth.verts[i];
	}
	for (size_t i = 0; i < oth.edges.size(); ++i)
	{
		edges[i] = oth.edges[i];
	}
	for (size_t i = 0; i < oth.indexPartion.size(); ++i)
	{
		indexPartion[i] = oth.indexPartion[i];
	}

	return *this;
}