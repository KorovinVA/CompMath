#include "Node.h"

Node::Node(int val, size_t ident)
{
	identifier = ident;
	m_value = val;
	DijkstraFlag = false;
}
