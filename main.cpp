#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include "lexer.h"
#include <string>
#include <queue>

using namespace std;

struct Tokens
{
	//int id;
	string name;
	int lineNum;
	//int tokenCount;
};

struct Syms {
	string name;	// name of symbol
	bool isTerm;	// is symbol term or not
					//vector<Syms> kids;	// kids
					//int kidCount;	// number of kids
};

struct Node {
	int id = 0;
	Syms symbol;
	int ruleNum = 0;
	Node* pMom;
	Syms* pKids[4];
	int kidCount = 0;
	int position = 0;
};

queue<Tokens> tokens;
stack<Syms> LLStack;

int ParseMatrix[21][23] =
{
	/*Pgm*/	{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Block*/{ 0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Stmts*/{ 0,	0,	3,	0,	4,	0,	0,	4,	0,	0,	4,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Stmt*/{ 0,	0,	0,	0,	5,	0,	0,	6,	0,	0,	7,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Astmt*/{ 0,	0,	0,	0,	9,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Y*/	{ 0,	0,	0,	0,	10,	0,	11,	0,	10,	0,	0,	0,	0,	0,	0,	10,	10,	10,	0,	0,	0,	0,	0 },
	/*Ostmt*/{ 0,	0,	0,	0,	0,	0,	0,	12,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Wstmt*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	13,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Fstmt*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Else2*/{ 0,	0,	0,	15,	0,	0,	0,	0,	0,	0,	0,	0,	16,	17,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Elist*/{ 0,	0,	0,	0,	18,	0,	0,	0,	18,	19,	0,	0,	0,	0,	0,	18,	18,	18,	0,	0,	0,	0,	0 },
	/*Elist2*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	39,	0,	0,	0,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*E'*/	{ 0,	0,	0,	21,	0,	0,	0,	0,	0,	21,	0,	0,	0,	0,	21,	0,	0,	0,	22,	22,	0,	0,	0 },
	/*E*/	{ 0,	0,	0,	0,	23,	0,	0,	0,	23,	0,	0,	0,	0,	0,	0,	23,	23,	23,	0,	0,	0,	0,	0 },
	/*T'*/	{ 0,	0,	0,	24,	0,	0,	0,	0,	0,	24,	0,	0,	0,	0,	24,	0,	0,	0,	24,	24,	25,	25,	25 },
	/*T*/	{ 0,	0,	0,	0,	26,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	26,	26,	26,	0,	0,	0,	0,	0 },
	/*F*/	{ 0,	0,	0,	0,	27,	0,	0,	0,	28,	0,	0,	0,	0,	0,	0,	27,	27,	27,	0,	0,	0,	0,	0 },
	/*Pexpr*/{ 0,	0,	0,	0,	0,	0,	0,	0,	29,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Fatom*/{ 0,	0,	0,	0,	30,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	31,	32,	33,	0,	0,	0,	0,	0 },
	/*Opadd*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	35,	0,	0,	0 },
	/*Opmul*/{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	36,	37,	38 }
};

//Function Prototypes
void parseMachine();
queue<Tokens> tokenize();
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

	// Set up LL Parsing Stack
	Syms eof{ "$", true };
	LLStack.push(eof);

	Syms pgm{ "Pgm", false };
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
		if (LLStack.top().name == "$" && tokens.empty())
		{
			LLStack.pop();
		}
		else if (LLStack.top().isTerm == true)
		{
			if (LLStack.top().name == tokens.front().name)
			{
				LLStack.pop();
				tokens.pop();
			}
			else
			{
				cout << "Error on line number: " << tokens.front().lineNum << " with token:  " << tokens.front().name << endl;
				exit(-1);
			}
		}

		else
		{
			performRule(ParseMatrix[getRowNum(LLStack.top().name)][getColNum(tokens.front().name)]);
		}
	}
	cout << "No issues found with input" << endl;
}

void performRule(int rule)
{

	switch (rule)
	{
		case 0: // Cell is empty
		{
			cout << "Error on line number: " << tokens.front().lineNum << " with token:  " << tokens.front().name << endl;
			exit(-1);
			break;
		}
		case 1: // Pgm = kwdprog Block
		{
			Syms Block = { "Block", false };
			Syms kwdprog = { "kwdprog", true };

			LLStack.pop();
			LLStack.push(Block);
			LLStack.push(kwdprog);
			break;
		}
		case 2: // Block = brace1 Stmts brace2
		{
			Syms brace1 = { "brace1", true };
			Syms Stmts = { "Stmts", false };
			Syms brace2 = { "brace2", true };

			LLStack.pop();
			LLStack.push(brace2);
			LLStack.push(Stmts);
			LLStack.push(brace1);
			break;
		}
		case 3: // Stmts = eps
		{
			LLStack.pop();
			break;
		}
		case 4: // Stmts = Stmt semi Stmt
		{
			Syms Stmt = { "Stmt", false };
			Syms semi = { "semi", true };
			Syms Stmts = { "Stmts", false };

			LLStack.pop();
			LLStack.push(Stmts);
			LLStack.push(semi);
			LLStack.push(Stmt);
			break;
		}
		case 5: // Stmt = Astmt
		{
			Syms Astmt = { "Astmt", false };

			LLStack.pop();
			LLStack.push(Astmt);
			break;
		}
		case 6: // Stmt = Ostmt
		{
			Syms Ostmt = { "Ostmt", false };

			LLStack.pop();
			LLStack.push(Ostmt);
			break;
		}

		case 7: // Stmt = Wstmt
		{
			Syms Wstmt = { "Wstmt", false };

			LLStack.pop();
			LLStack.push(Wstmt);
			break;
		}
		case 8: // Stmt = Fstmt
		{
			Syms Fstmt = { "Fstmt", false };

			LLStack.pop();
			LLStack.push(Fstmt);
			break;
		}
		case 9: // Astmt = id equal Y
		{
			Syms id = { "ident", true };
			Syms equal = { "equal", true };
			Syms Y = { "Y", false };

			LLStack.pop();
			LLStack.push(Y);
			LLStack.push(equal);
			LLStack.push(id);
			break;
		}
		case 10: // Y = E
		{
			Syms E = { "E", false };

			LLStack.pop();
			LLStack.push(E);
			break;
		}
		case 11: // Y = kwdinput
		{
			Syms kwdinput = { "kwdinput", true };

			LLStack.pop();
			LLStack.push(kwdinput);
			break;
		}
		case 12: // Ostmt = kwdprint paren1 Elist paren2
		{
			Syms kwdprint = { "kwdprint", true };
			Syms paren1 = { "parens1", true };
			Syms Elist = { "Elist", false };
			Syms paren2 = { "parens2", true };

			LLStack.pop();
			LLStack.push(paren2);
			LLStack.push(Elist);
			LLStack.push(paren1);
			LLStack.push(kwdprint);
			break;
		}
		case 13: // Wstmt = kwdwhile Pexpr Block
		{
			Syms kwdwhile = { "kwdwhile", true };
			Syms Pexpr = { "Pexpr", false };
			Syms Block = { "Block", false };

			LLStack.pop();
			LLStack.push(Block);
			LLStack.push(Pexpr);
			LLStack.push(kwdwhile);
			break;
		}
		case 14: // Fstmt = kwdif Pexpr Block Else2
		{
			Syms kwdif = { "kwdif", true };
			Syms Pexpr = { "Pexpr", false };
			Syms Block = { "Block", false };
			Syms Else2 = { "Else2", false };

			LLStack.pop();
			LLStack.push(Else2);
			LLStack.push(Block);
			LLStack.push(Pexpr);
			LLStack.push(kwdif);
			break;
		}
		case 15: // Else2 = eps
		{
			LLStack.pop();
			break;
		}
		case 16: //Else2 = kwdelseif Pexpr Block Else2
		{
			Syms kwdelseif = { "kwdelseif", true };
			Syms Pexpr = { "Pexpr", false };
			Syms Block = { "Block", false };
			Syms Else2 = { "Else2", false };

			LLStack.pop();
			LLStack.push(Else2);
			LLStack.push(Block);
			LLStack.push(Pexpr);
			LLStack.push(kwdelseif);
			break;
		}
		case 17: //Else2 = kwdelse Block
		{
			Syms kwdelse = { "kwdelse", true };
			Syms Block = { "Block", false };

			LLStack.pop();
			LLStack.push(Block);
			LLStack.push(kwdelse);
			break;
		}
		case 18: // Elist = E Elist2
		{
			Syms E = { "E", false };
			Syms Elist2 = { "Elist2", false };

			LLStack.pop();
			LLStack.push(Elist2);
			LLStack.push(E);
			break;
		}
		case 19: // Elist = eps
		{
			LLStack.pop();
			break;
		}
		case 20: // Elist2 = comma Elist
		{
			Syms comma = { "comma", true };
			Syms Elist = { "Elist", false };

			LLStack.pop();
			LLStack.push(Elist);
			LLStack.push(comma);
			break;
		}
		case 21: // E'= eps
		{
			LLStack.pop();
			break;
		}
		case 22: // E' = Opadd T E'
		{
			Syms Opadd = { "Opadd", false };
			Syms T = { "T", false };
			Syms Eprime = { "E'", false };

			LLStack.pop();
			LLStack.push(Eprime);
			LLStack.push(T);
			LLStack.push(Opadd);
			break;
		}
		case 23: // E = T E'
		{
			Syms T = { "T", false };
			Syms Eprime = { "E'", false };

			LLStack.pop();
			LLStack.push(Eprime);
			LLStack.push(T);
			break;
		}
		case 24: // T'= eps
		{
			LLStack.pop();
			break;
		}
		case 25: // T'= Opmul F T'
		{
			Syms Opmul = { "Opmul", false };
			Syms F = { "F", false };
			Syms Tprime{ "T'", false };

			LLStack.pop();
			LLStack.push(Tprime);
			LLStack.push(F);
			LLStack.push(Opmul);
			break;
		}
		case 26: // T = FT'
		{
			Syms F = { "F", false };
			Syms Tprime{ "T'", false };

			LLStack.pop();
			LLStack.push(Tprime);
			LLStack.push(F);
			break;
		}
		case 27: //F = Fatom
		{
			Syms Fatom = { "Fatom", false };

			LLStack.pop();
			LLStack.push(Fatom);
			break;
		}
		case 28: // F = Pexpr
		{
			Syms Pexpr = { "Pexpr", false };

			LLStack.pop();
			LLStack.push(Pexpr);
			break;
		}
		case 29: // Pexpr = paren1 E paren2
		{
			Syms paren1 = { "parens1", true };
			Syms E{ "E", false };
			Syms paren2 = { "parens2", true };

			LLStack.pop();
			LLStack.push(paren2);
			LLStack.push(E);
			LLStack.push(paren1);
			break;
		}
		case 30: // Fatom = id
		{
			Syms id = { "ident", true };

			LLStack.pop();
			LLStack.push(id);
			break;
		}
		case 31: // Fatom = int
		{
			Syms integer = { "int", true };

			LLStack.pop();
			LLStack.push(integer);
			break;
		}
		case 32: // Fatom = float
		{
			Syms floatN = { "float", true };

			LLStack.pop();
			LLStack.push(floatN);
			break;
		}
		case 33: // Fatom = string
		{
			Syms stringl = { "string", true };

			LLStack.pop();
			LLStack.push(stringl);
			break;
		}
		case 34: // Opadd = plus
		{
			Syms plus = { "plus", true };

			LLStack.pop();
			LLStack.push(plus);
			break;
		}
		case 35: // Opadd = minus
		{
			Syms minus = { "minus", true };

			LLStack.pop();
			LLStack.push(minus);
			break;
		}
		case 36: // Opmul = aster
		{
			Syms aster = { "aster", true };

			LLStack.pop();
			LLStack.push(aster);
			break;
		}
		case 37: // Opmul = slash
		{
			Syms slash = { "slash", true };

			LLStack.pop();
			LLStack.push(slash);
			break;
		}
		case 38: // Opmul = caret
		{
			Syms caret = { "caret", true };

			LLStack.pop();
			LLStack.push(caret);
			break;
		}
		case 39: // Elist2 = eps
		{
			LLStack.pop();
			break;
		}

	}
}

int getRowNum(string name)
{
	// Return a row number in parse matrix for the non-terminal
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

queue<Tokens> tokenize()
{
	char x;
	bool hasNum = false;
	bool isSet = false;
	bool foundName = false;
	Tokens token;
	queue<Tokens> outputTokens;
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
				//token.id = getColNum(token.name);
				outputTokens.push(token);
				// Reset name for the next token
				token.name = "";
			}
		}
	}
	return outputTokens;
}

int getColNum(string name)
{
	// Return a column number in parse matrix for the token
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
