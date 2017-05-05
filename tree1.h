#pragma once
#include <iostream>
#include <string>
using namespace std;

	struct nodeTree
	{
	private:
		nodeTree * lKid;
		nodeTree * rKid;
		int symID;
		string name;
		string symKind;
		int ruleID;
		int mtxID;

	public:
		nodeTree * root;
		nodeTree()
		{
			root = NULL;
		}
		void addNodeTree(int);  //Pretty much the node would set up by the INT for the symbol id or rule id;
	};
