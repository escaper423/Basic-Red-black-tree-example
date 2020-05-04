#pragma once

struct Node
{
	Node *left;
	Node *right;
	Node *parent;
	int data;
	int color;
};

class RBTree
{
private:
	Node* root;
public:
	~RBTree();
	RBTree();
	Node* Grandparent(Node *x);
	Node* Uncle(Node *x);
	Node* Sibling(Node *x);
	void PrintTree(const RBTree *x);
	void PrintNode(const Node* n);
	Node* CreateNode(int color, int val, Node* p);
	void Insert(int val);
	void InsertRB(Node *n);
	void Delete(int val);
	void DeleteOneNode(Node *n);
	void Replace(Node* n, Node* c);
	void DeleteRB(Node *n);
	void Rotate_Right(Node *n);
	void Rotate_Left(Node *n);
	Node* Successor(int x);
	Node* Predecessor(int x);
	Node* Find(int x);
	bool isInvalid(Node* n);
};