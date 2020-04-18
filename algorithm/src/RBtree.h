#pragma once
#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

class RBtree
{
public:
	RBtree(int val);
	RBtree* Insert(int val);
	void PrintGraph(std::ofstream& stream);
	~RBtree();
public:
	enum COLOR
	{
		RED,
		BLACK
	};

	RBtree* left;
	RBtree* right;
	RBtree* parent;
	int m_value;
	int color;

	void RightRotate();
	void LeftRotate();
	RBtree* findUncle() const;
	RBtree* findRoot();
	bool isUncleRed() const;
	void AttachChild(RBtree* Node);
	void balanceTree();

	void digraph(std::ofstream& stream, int lab_count) const;
	void CreateDotBody(RBtree* node, std::ofstream& stream, int lab, int* lab_count);
};

RBtree::RBtree(int val)
{
	right = nullptr;
	left = nullptr;
	m_value = val;
	color = COLOR::BLACK;
}

RBtree::~RBtree()
{
	delete this;
}

RBtree* RBtree::Insert(int val)
{
	RBtree* node = new RBtree(val);
	node->color = COLOR::RED;

	AttachChild(node);
	if (parent == nullptr)
		return this;
	return parent->findRoot();
}

void RBtree::PrintGraph(std::ofstream& stream)
{
	int* lab_count = new int;
	int lab = 0;
	*lab_count = 0;
	digraph(stream, *lab_count);
	CreateDotBody(this, stream, lab, lab_count);
	(*lab_count)++;
	digraph(stream, *lab_count);
	delete lab_count;
}

void RBtree::digraph(std::ofstream& stream, int lab_count) const
{
	if (lab_count == 0)
		stream << "digraph G{ \n" << "node [style=filled, fontcolor=white];" << std::endl;
	else
		stream << "}" << std::endl;
}

void RBtree::CreateDotBody(RBtree* node, std::ofstream& stream, int lab, int* lab_count)
{
	std::string color;
	if (node)
	{
		switch (node->color)
		{
		case COLOR::RED:
			color = "red";
			break;
		case COLOR::BLACK:
			color = "black";
			break;
		}

		stream << "n" << lab;
		stream << "[label=\"" << node->m_value << "\"" << ", color=" << color << "];" << std::endl;

		if (node->left) 
			stream << "n" << lab << " -> " << "n" << (lab + 1) << ";" << std::endl;
		(*lab_count)++;
		CreateDotBody(node->left, stream, (lab + 1), lab_count);

		if (node->right)
			stream << "n" << lab << " -> " << "n" << *lab_count << ";" << std::endl;
		CreateDotBody(node->right, stream, *lab_count, lab_count);
	}
}

void RBtree::RightRotate()
{
	RBtree* pivot = left;
	pivot->parent = parent;
	if (parent != nullptr)
	{
		if (parent->left == this)
			parent->left = pivot;
		else
			parent->right = pivot;
	}
	left = pivot->right;
	if (pivot->right != nullptr)
		pivot->right->parent = this;
	parent = pivot;
	pivot->right = this;
}

void RBtree::LeftRotate()
{
	RBtree* pivot = right;
	pivot->parent = parent;
	if (parent != nullptr)
	{
		if (parent->left == this)
			parent->left = pivot;
		else
			parent->right = pivot;
	}
	right = pivot->left;
	if (pivot->left != nullptr)
		pivot->left->parent = this;
	parent = pivot;
	pivot->left = this;
}

RBtree* RBtree::findUncle() const
{
	RBtree* uncle = nullptr;
	if (parent->parent == nullptr)
		return nullptr;

	if (parent->parent->left == parent)
		uncle = parent->parent->right;
	else if (parent->parent->right == parent)
		uncle = parent->parent->left;

	return uncle;
}

RBtree* RBtree::findRoot()
{
	if (parent == nullptr)
		return this;
	return parent->findRoot();
}

bool RBtree::isUncleRed() const
{
	RBtree* uncle = findUncle();
	if (uncle && uncle->color == COLOR::RED)
		return true;
	return false;
}

void RBtree::AttachChild(RBtree* node)
{
	if (m_value <= node->m_value)
	{
		if (right)
		{
			right->AttachChild(node);
		}
		else
		{
			right = node;
			node->parent = this;
			node->balanceTree();
		}
	}
	else if (m_value > node->m_value)
	{
		if (left)
		{
			left->AttachChild(node);
		}
		else
		{
			left = node;
			node->parent = this;
			node->balanceTree();
		}
	}
	balanceTree();
}

void RBtree::balanceTree()
{
	RBtree* node = this; //to work with Case 3 and Case 4 together
//Case 0: root
	if (parent == nullptr)
	{
		color = COLOR::BLACK;
		return ;
	}
//Case 1: Parent - Black
	else if (parent->color == COLOR::BLACK)
	{
		return;
	}
//Case 2: Uncle - Red
	else if (isUncleRed())
	{
		findUncle()->color = COLOR::BLACK;
		parent->color = COLOR::BLACK;
		parent->parent->color = COLOR::RED;
		parent->parent->balanceTree();
		return ;
	}
//Case 3: Parent and Node is NOT OneWay
	assert(parent->parent);
	if (this == parent->right && parent == parent->parent->left)
	{
		parent->LeftRotate();
		node = left;
	}
	else if (this == parent->left && parent == parent->parent->right)
	{
		parent->RightRotate();
		node = right;
	}
//Case 4:: Parent and Node is OneWay
	assert(node->parent->parent);
	node->parent->color = COLOR::BLACK;
	node->parent->parent->color = COLOR::RED;

	if (node == node->parent->left 
		&& node->parent == node->parent->parent->left)
		node->parent->parent->RightRotate();
	else if(node == node->parent->right 
		&& node->parent == node->parent->parent->right)
		node->parent->parent->LeftRotate();
}