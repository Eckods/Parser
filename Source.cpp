#include <iostream>
#include <string>
#include "tree1.h";
using namespace std;
nodeTree * node;

void addNodeTree(int id);

int main()
{

	// read into the stuff



}


void nodeTree::addNodeTree(int id)
{
	string identifier;
	string kind;
	int mtx;

	if (node == NULL)		// for symbol also for the oarent node
	{
		node = new nodeTree;
		node->symID = id;
		node->name = identifier;
		node->symKind = kind;
		node->mtxID = mtx;
		node->lKid = node->rKid = NULL;
	}
	else
	{
		node = new nodeTree;		//If not empty
		if (id < node->symID)
		{
			node->lKid = node;
			node->symID = id;
		}
			
		else {
			node->rKid = node;
			node->symID = id;
		}
	}
}