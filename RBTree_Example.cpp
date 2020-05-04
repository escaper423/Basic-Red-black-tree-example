#include <iostream>
#include "RBTree_Example.h"
#define NIL_VAL 0
enum { black = 0, red };
char colors[2] = { 'B','R' };

Node* NIL = new Node();

RBTree::~RBTree() 
{
	delete root;
	root = nullptr;
}

RBTree::RBTree()
{
	root = nullptr;
}

Node* RBTree::Grandparent(Node *n)
{
	if (n->parent != nullptr && n->parent->parent != nullptr)
	{
		return n->parent->parent;
	}
	else return nullptr;
}

Node* RBTree::Sibling(Node *n)
{
	if (n->parent == nullptr)
		return nullptr;

	if (n == n->parent->left)
		return n->parent->right;
	else return n->parent->left;
}

Node* RBTree::Uncle(Node *n)
{
	//No parent, No uncle
	//No grandparent, no uncle as well
	if (n->parent == nullptr || Grandparent(n) == nullptr)
	{
		return nullptr;
	}
	else
	{
		//N's parent is N's Grandparent's left
		if (Grandparent(n)->left != nullptr && Grandparent(n)->left == n->parent)
		{
			return Grandparent(n)->right;
		}

		//N's parent is N's Grandparent's right
		else if (Grandparent(n)->right != nullptr && Grandparent(n)->right == n->parent)
		{
			return Grandparent(n)->left;
		}
		else return NIL;
	}
}

void RBTree::PrintTree(const RBTree *t)
{
	Node *x = t->root;

	std::cout << x->data << colors[x->color] << std::endl;

	if (x->left != nullptr)
	{
		PrintNode(x->left);
	}

	if (x->right != nullptr)
	{
		PrintNode(x->right);
	}
}

void RBTree::PrintNode(const Node* n)
{
	std::cout << n->data << colors[n->color] << std::endl;

	if (n->left != nullptr)
		PrintNode(n->left);

	if (n->right != nullptr)
		PrintNode(n->right);
}

Node* RBTree::CreateNode(int color, int val, Node* p)
{
	Node *N = new Node();
	N->color = color;
	N->data = val;
	N->left = NIL;
	N->right = NIL;
	N->parent = p;
	return N;
}

bool RBTree::isInvalid(Node *n)
{
	if (n == NIL || n == nullptr)
		return true;
	else return false;
}
void RBTree::Insert(int val)
{
	std::cout << "Insert " << val << std::endl;
	//n starts with root
	Node *n = root;
	Node *p = nullptr;

	if (root == nullptr)
	{
		root = CreateNode(red, val, p);
		InsertRB(root);
	}

	else {
		while (n != nullptr && n != NIL) {
			//the value to be inserted is larger than current node's
			p = n;
			if (n->data < val)
				n = n->right;
			//the value to be inserted is smaller or equal the current node's
			else
				n = n->left;
		}

		if (p->data < val)
		{
			p->right = CreateNode(red, val, p);
			n = p->right;
		}

		else
		{
			p->left = CreateNode(red, val, p);
			n = p->left;
		}
		InsertRB(n);
	}

}

void RBTree::InsertRB(Node *n)
{
	Node* u = Uncle(n);
	Node* g = Grandparent(n);

	//case 1 : parent is null.

	if (n->parent == nullptr) {
		n->color = black;
		return;
	}
	//case 2 : parent is black.

	if (n->parent->color == black)
		return;

	//case 3 : uncle is red, and parent is also red.

	if (u != nullptr && u->color == red)
	{
		n->parent->color = black;
		u->color = black;
		g->color = red;
		InsertRB(g);
		return;
	}

	//case 4 : LR case or RL case

	if ((n == n->parent->right) && (n->parent == g->left)) {
		Rotate_Left(n->parent);
		n = n->left;
	}
	else if ((n == n->parent->left) && (n->parent == g->right)) {
		Rotate_Right(n->parent);
		n = n->right;
	}

	//case 5 : LL case or RR case

	n->parent->color = black;
	g->color = red;
	if (n == n->parent->left) {
		Rotate_Right(Grandparent(n));
		n = n->right;
	}
	else {
		Rotate_Left(Grandparent(n));
		n = n->left;
	}

	return;
}

void RBTree::Delete(int val)
{
	std::cout << "Delete " << val << std::endl;
	Node *n = Find(val);
	Node *ps = nullptr;
	if (isInvalid(n))
	{
		std::cout << "Couldn't find node" << std::endl;
		return;
	}
	//in case of 1 or 0 child.
	if (isInvalid(n->right) || isInvalid(n->left))
		DeleteOneNode(n);

	//in case of n have 2 children.
	else
	{
		ps = Successor(val);
		n->data = ps->data;
		DeleteOneNode(ps);
	}
}
void RBTree::Replace(Node* n, Node* c)
{
	if (n->parent == nullptr)
		root = c;
	else if (n == n->parent->left)
		n->parent->left = c;
	else
		n->parent->right = c;

	c->parent = n->parent;

}

void RBTree::DeleteOneNode(Node* n)
{
	Node *c;
	c = (!isInvalid(n->left)) ? n->left : n->right;

	//node has 1 child
	if (!isInvalid(c->left) || !isInvalid(c->right))
	{
		Replace(n, c);
		if (n->color == black)
		{
			if (c->color == red)
			{
				c->color = black;
				free(n);
			}
			else
			{
				DeleteRB(c);
				free(n);
			}
		}
	}
	//case : node has 0 child (all children are NIL nodes)
	else
	{
		//M is red. just delete.
		if (n->color == red)
		{
			Replace(n, c);
			free(n);
		}
		//M is black, it's complex case.
		else
		{
			Replace(n, c);
			DeleteRB(c);
			free(n);
		}
	}
}

void RBTree::DeleteRB(Node * n)
{
	Node *s = Sibling(n);

	std::cout << "Delete case 1" << std::endl;
	//case 1 : N is new root.
	if (n->parent == nullptr)
		return;

	//case 2 : S is red
	std::cout << "Delete case 2" << std::endl;
	if (s->color == red)
	{
		n->parent->color = red;
		s->color = black;
		if (n == n->parent->left)
			Rotate_Left(n->parent);
		else
			Rotate_Right(n->parent);
	}

	std::cout << "Delete case 3" << std::endl;
	//case 3 : P,S,S's children are black
	if (n->parent->color == black && s->color == black && s->left->color == black && s->right->color == black)
	{
		s->color = red;
		DeleteRB(n->parent);
		return;
	}

	std::cout << "Delete case 4" << std::endl;
	//case 4 : S, S's children are black, P is red
	if (n->parent->color == red && s->color == black && s->left->color == black && s->right->color == black)
	{
		n->parent->color = black;
		s->color = red;
		return;
	}

	std::cout << "Delete case 5" << std::endl;
	//case 5 : S is black, S's left is red, S's right is black, N is left or right child of its parent
	if (s->color == black)
	{
		if (n == n->parent->left && s->right->color == black && s->left->color == red)
		{
			s->color = red;
			s->left->color = black;
			Rotate_Right(s);
		}
		else if (n == n->parent->right && s->left->color == black && s->right->color == red)
		{
			s->color = red;
			s->right->color = black;
			Rotate_Left(s);
		}
	}

	std::cout << "Delete case 6" << std::endl;
	//case 6 : S is black, S's right is red, N is left or right of P.
	s->color = n->parent->color;
	n->parent->color = black;

	if (n == n->parent->left)
	{
		s->right->color = black;
		Rotate_Left(n->parent);
	}
	else
	{
		s->left->color = black;
		Rotate_Right(n->parent);
	}
	return;
}

void RBTree::Rotate_Right(Node* n)
{
	Node *save_n = nullptr;
	Node *save_c_right = n->left->right;

	if (n->parent != nullptr)
	{
		if (n->parent->left == n)
		{
			save_n = n->parent->left;
			n->parent->left = n->left;

		}
		else
		{
			save_n = n->parent->right;
			n->parent->right = n->left;
		}

		n->left->parent = n->parent;
		n->left->right = save_n;
		save_n->parent = n->left;
		save_n->left = save_c_right;
		save_c_right->parent = save_n;

	}

	else
	{
		save_n = n->left->parent;
		n->left->right = n->left->parent;
		save_n->parent = n->left;
		root = n->left;
		n->left->parent = nullptr;
		save_n->left = save_c_right;
		save_c_right->parent = save_n;
	}


}

void RBTree::Rotate_Left(Node* n)
{
	Node *save_n = nullptr;
	Node *save_c_left = n->right->left;
	if (n->parent != nullptr)
	{
		if (n->parent->left == n)
		{
			save_n = n->parent->left;
			n->parent->left = n->right;

		}
		else
		{
			save_n = n->parent->right;
			n->parent->right = n->right;
		}

		n->right->parent = n->parent;
		n->right->left = save_n;
		save_n->parent = n->right;
		save_n->right = save_c_left;
		save_c_left->parent = save_n;
	}

	else
	{
		save_n = n->right->parent;
		n->right->left = n->right->parent;
		save_n->parent = n->right;
		root = n->right;
		n->right->parent = nullptr;
		save_n->right = save_c_left;
		save_c_left->parent = save_n;
	}
}

Node* RBTree::Find(int x)
{
	Node* p = root;
	while (p != nullptr && p != NIL)
	{
		if (x < p->data)
		{
			p = p->left;
		}
		else if (x > p->data)
		{
			p = p->right;
		}
		else
		{
			return p;
		}

	}
	return NIL;
}

Node * RBTree::Predecessor(int x)
{
	Node* n = Find(x);
	Node *a, *p;
	if (isInvalid(n)) return nullptr;

	//Successor case 1 : the node has left subtree
	if (!isInvalid(n->left))
	{
		n = n->left;
		while (!isInvalid(n->right))
		{
			n = n->right;
		}
		std::cout << "Precedessor's value : " << n->data << std::endl;
		return n;
	}
	//Successor case 2 : the node has no right subtree
	else
	{
		a = root;
		p = nullptr;
		while (a != n)
		{
			if (n->data > a->data)
			{
				p = a;
				a = a->right;
			}
			else
				a = a->left;
		}
		std::cout << "Precedessor's value : " << p->data << std::endl;
		return p;
	}
}

Node * RBTree::Successor(int x)
{
	Node* n = Find(x);
	Node *a, *s;
	if (isInvalid(n)) return nullptr;

	//Successor case 1 : the node has right subtree
	if (!isInvalid(n->right))
	{
		n = n->right;
		while (!isInvalid(n->left))
		{
			n = n->left;
		}
		std::cout << "Successor's value : " << n->data << std::endl;
		return n;
	}
	//Successor case 2 : the node has no right subtree
	else
	{
		a = root;
		s = nullptr;
		while (a != n)
		{
			if (n->data < a->data)
			{
				s = a;
				a = a->left;
			}
			else
				a = a->right;
		}
		std::cout << "Successor's value : " << s->data << std::endl;
		return s;
	}
}

int main()
{
	RBTree* t = new RBTree();
	int select, x;
	do {
		std::cout << "Choose one" << std::endl;
		std::cout << "1. Insert" << std::endl;
		std::cout << "2. Delete" << std::endl;
		std::cout << "3. Print" << std::endl;
		std::cout << "4. Find" << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cin >> select;

		switch (select)
		{
		case 1:
			std::cout << "Value : ";
			std::cin >> x;
			if (x) t->Insert(x);
			else
				std::cout << "nope." << std::endl;
			break;
		case 2:
			std::cout << "Value : ";
			std::cin >> x;
			if (x) t->Delete(x);
			else
				std::cout << "nope." << std::endl;
			break;
		case 3:
			std::cout << "- Printing Tree... -" << std::endl;
			t->PrintTree(t);
			break;
		case 4:
			std::cout << "Value : ";
			std::cin >> x;
			if (t->Find(x) != NIL)
				std::cout << "found." << std::endl;
			else
				std::cout << "nope." << std::endl;
			break;
		case 0:
		default:
			break;

		}

	} while (select != 0);
	return 0;
}