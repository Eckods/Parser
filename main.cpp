#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <vector>
#include "lexer.h"
#include <string>

using namespace std;



struct Rules
{
	int id;
	int lhs_symID;
	int rhs_symID[10];
	int rhs_symCount;
};


struct Tokens
{
	int id;
	string name;
	int lineNum;
	int tokenCount;
};

struct Syms {
	string name;	// name of symbol
	bool isTerm;	// is symbol term or not
	vector<Syms> kids;	// kids
	int kidCount;	// number of kids
};

int tokenCount;
vector<Tokens> tokens;
//vector<Syms*> symbols;
//vector<Rules*> rules;
stack<Syms> LLStack;

int ParseMatrix[21][23] =
{
	/*Pgm*/{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Block*/{ 0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Stmts*/{ 0,	0,	3,	0,	4,	0,	0,	4,	0,	0,	4,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Stmt*/{ 0,	0,	0,	0,	5,	0,	0,	6,	0,	0,	7,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Astmt*/{ 0,	0,	0,	0,	9,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Y*/{ 0,	0,	0,	0,	10,	0,	11,	0,	10,	0,	0,	0,	0,	0,	0,	10,	10,	10,	0,	0,	0,	0,	0 },
	/*Ostmt*/{ 0,	0,	0,	0,	0,	0,	0,	12,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Wstmt*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Fstmt*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Else2*/{ 0,	0,	0,	15,	0,	0,	0,	0,	0,	0,	0,	0,	16,	17,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Elist*/{ 0,	0,	0,	0,	18,	0,	0,	0,	18,	19,	0,	0,	0,	0,	0,	18,	18,	18,	0,	0,	0,	0,	0 },
	/*Elist2*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*E'*/{ 0,	0,	0,	20,	0,	0,	0,	0,	0,	21,	0,	0,	0,	0,	21,	0,	0,	0,	22,	22,	0,	0,	0 },
	/*E*/{ 0,	0,	0,	0,	23,	0,	0,	0,	23,	0,	0,	0,	0,	0,	0,	23,	23,	23,	0,	0,	0,	0,	0 },
	/*T'*/{ 0,	0,	0,	24,	0,	0,	0,	0,	0,	24,	0,	0,	0,	0,	24,	0,	0,	0,	24,	24,	25,	25,	25 },
	/*T*/{ 0,	0,	0,	0,	26,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	26,	26,	26,	0,	0,	0,	0,	0 },
	/*F*/{ 0,	0,	0,	0,	27,	0,	0,	0,	28,	0,	0,	0,	0,	0,	0,	27,	27,	27,	0,	0,	0,	0,	0 },
	/*Pexpr*/{ 0,	0,	0,	0,	0,	0,	0,	0,	29,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Fatom*/{ 0,	0,	0,	0,	30,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	31,	32,	33,	0,	0,	0,	0,	0 },
	/*Opadd*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	35,	0,	0,	0 },
	/*Opmul*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	36,	37,	38 }
};

//Function Prototypes
void parseMachine();
//void getRules(int rule);
vector<Tokens> tokenize();
int getColNum(string name);
int getRowNum(string name);
void performRule(int rule);

int main()
{
	// Run the lexer to get stream used for parser
	Lexer lexer;
	lexer.runLexer();
	system("cls"); // System call, I know, I know

				   // Tokenize the stream file
	cout << "Converting lexer output into tokens" << endl;
	tokens = tokenize();
	tokenCount = 0;

	// Set up LL Parsing Stack
	//Node eof{ -1, {}, 0 };
	//LLStack.push(eof);
	//Node pgm{ 0, {}, 0 };
	//LLStack.push(pgm);

	Syms eof{ "$", true, {}, 0 };
	LLStack.push(eof);

	Syms pgm{ "Pgm", false, {}, 0 };
	LLStack.push(pgm);

	// Run the parser
	parseMachine();

	return 0;
}

void parseMachine()
{
	while (!LLStack.empty())
	{

		// If end of file, pop off stack and done
		if (LLStack.top().name == "eof")
		{
			LLStack.pop();
		}
		else if (LLStack.top().isTerm == true)
		{ 
			if (LLStack.top().name == tokens[tokenCount].name)
			{
				LLStack.pop();
				tokenCount++;
				//getRules(LLStack.top().id);
			}
			else
				cout << "Error on line number: " << tokens[tokenCount].lineNum << " with token:  " << tokens[tokenCount].name << endl;
		}
		
		else
		{
			performRule(ParseMatrix[getRowNum(LLStack.top().name)][tokens[tokenCount].id]);
		}
		//else if (LLStack.top() == )
	}
}

void performRule(int rule)
{

	switch (rule) {
	case 1: // Pgm = kwdprog Block
	{
		Syms Block = { "Block", false, {}, 0 };
		Syms kwdprog = { "kwdprog", true, {}, 0 };
		LLStack.top().kids.push_back(kwdprog);
		LLStack.top().kids.push_back(Block);

		LLStack.pop();
		LLStack.push(Block);
		LLStack.push(kwdprog);

		break;
	}
	case 2: //Block = brace1 Stmts brace2
	{
		Syms brace2 = { "brace2", true,{}, 0 };
		Syms Stmts = { "Stmts", false,{}, 0 };
		Syms brace1 = { "brace1", true,{}, 0 };
		LLStack.top().kids.push_back(brace1);
		LLStack.top().kids.push_back(Stmts);
		LLStack.top().kids.push_back(brace2);

		LLStack.pop();
		LLStack.push(brace2);
		LLStack.push(Stmts);
		LLStack.push(brace1);
		break;
	}
	case 13:


	break;
	case 3:
	break;
	case 4:
	break;
	case 5:
	break;
	case 6:
	break;
	}
}
int getRowNum(string name)
{
	// Return a row number in parse matrix for the corresponding string
	if (name == "Pgm")
		return 0;
	else if (name == "Block")
		return 1;
	else if (name == "Stmts")
		return 2;
	else if (name == "Stmt")
		return 3;
	else if (name == "Astmt")
		return 4;
	else if (name == "Y")
		return 5;
	else if (name == "Ostmt")
		return 6;
	else if (name == "Wstmt")
		return 7;
	else if (name == "Fstmt")
		return 8;
	else if (name == "Else2")
		return 9;
	else if (name == "Elist")
		return 10;
	else if (name == "Elist2")
		return 11;
	else if (name == "E'")
		return 12;
	else if (name == "E")
		return 13;
	else if (name == "T'")
		return 14;
	else if (name == "T")
		return 15;
	else if (name == "F")
		return 16;
	else if (name == "Pexpr")
		return 17;
	else if (name == "Fatom")
		return 18;
	else if (name == "Opadd")
		return 19;
	else if (name == "Opmul")
		return 20;
}

vector<Tokens> tokenize()
{
	char x;
	bool hasNum = false;
	bool isSet = false;
	bool foundName = false;
	Tokens token;
	vector<Tokens> outputTokens;
	ifstream input("stream.txt");

	if (!input.is_open())
	{
		cout << "Error opening file." << endl;
		exit(EXIT_FAILURE);
	}

	// Read in the stream generated by lexer
	while (input.get(x))
	{
		// Once the 1st whitespace has been detected, locate and set line number for token
		if (isspace(x) && hasNum == false)
		{
			x++;
			input.get(x);
			token.lineNum = x - '0';
			x += 2;
			hasNum = true;
		}
		// Once the 2nd space has been detected, begin adding the characters of the name for the token name
		else if (isspace(x) && foundName == false)
		{
			x++;
			input.get(x);
			token.name += x;
			foundName = true;
		}
		// Continue adding characters to the name unless a ) or whitespace has been detected
		else if (isSet == false && foundName == true)
		{
			if (x == ')' || isspace(x))
				isSet = true;
			else
				token.name += x;
		}
		// After the lineNum and name are set, call the getColNum function for the token id,
		// reset the boolean values needed for next token check, and add token to the vector
		else if (isSet == true)
		{
			if (x == '\n')
			{
				hasNum = false;
				foundName = false;
				isSet = false;
				token.id = getColNum(token.name);
				outputTokens.push_back(token);
				// Reset name for the next token
				token.name = "";
			}
		}
	}
	return outputTokens;
}

int getColNum(string name)
{
	// Return a column number in parse matrix for the corresponding string
	if (name == "kwdprog")
		return 0;
	else if (name == "brace1")
		return 1;
	else if (name == "brace2")
		return 2;
	else if (name == "semi")
		return 3;
	else if (name == "ident")
		return 4;
	else if (name == "equal")
		return 5;
	else if (name == "kwdinput")
		return 6;
	else if (name == "kwdprint")
		return 7;
	else if (name == "parens1")
		return 8;
	else if (name == "parens2")
		return 9;
	else if (name == "kwdwhile")
		return 10;
	else if (name == "kwdif")
		return 11;
	else if (name == "kwdelseif")
		return 12;
	else if (name == "kwdelse")
		return 13;
	else if (name == "comma")
		return 14;
	else if (name == "int")
		return 15;
	else if (name == "float")
		return 16;
	else if (name == "string")
		return 17;
	else if (name == "plus")
		return 18;
	else if (name == "minus")
		return 19;
	else if (name == "aster")
		return 20;
	else if (name == "slash")
		return 21;
	else if (name == "caret")
		return 22;
}
