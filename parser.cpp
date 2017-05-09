#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <queue>
#include <string>
using namespace std;

enum symbols {Pgm, Block, Stmts, Stmt, Astmt, Y, Ostmt,
              Wstmt, Fstmt, Else2, Elist, Elist2, Eprime, E,
              Tprime, T, F, Pexpr, Fatom, Opadd, Opmul, $, 
              kwdprog, brace1, brace2, semi, id, equal, kwdinput,
              kwdprint, paren1, paren2, kwdwhile, kwdif, kwdelseif,
              kwdelse, comma, SymbInt, SymbFloat, string, plus, minus, aster,
              slash, caret, eps};

const char* symNames[] = {"Pgm", "Block", "Stmts", "Stmt", "Astmt", 
                          "Y", "Ostmt", "Wstmt", "Fstmt", "Else2",
                          "Elist", "Elist2", "Eprime", "E", "Tprime",
                          "T", "F", "Pexpr", "Fatom", "Opadd", "Opmul", "$", 
                          "kwdprog", "brace1", "brace2", "semi", "id", "equal", 
                          "kwdinput", "kwdprint", "paren1", "paren2",
                          "kwdwhile", "kwdif", "kwdelseif", "kwdelse",
                          "comma", "SymbInt", "SymbFloat", "string",
                          "plus", "minus", "aster", "slash", "caret", "epsilon"};

//3d LL Prediction Matrix
list<symbols> Matrix[23][21];

//Function Prototypes
void readStream(queue<symbols> charStream);
void initMatrix(list<symbols> box[][21]);

int main()
{
    initMatrix(Matrix);    

    //Set up LL Parsing Stack
    stack<symbols> LLStack;

    //Set up input stream
    queue<symbols> charStream;

    //Bottom of stack is always $
    //Followed by Start Symbol (Pgm)
    LLStack.push(symbols::$);
    LLStack.push(symbols::Pgm);

    //Example code showing access and printing from matrix
    // cout << symNames[Matrix[0][0].front()] << endl;
    //Matrix[0][0].pop_front();
    //cout << symNames[Matrix[0][0].front()] << endl;
    //Matrix[0][0].pop_front();

    readStream(charStream);

    return 0;
}

void initMatrix(list<symbols> box[][21])
{
    box[0][0].push_front(symbols::Block);
    box[0][0].push_front(symbols::kwdprog);

    box[1][1].push_front(symbols::brace2);
    box[1][1].push_front(symbols::Stmts);
    box[1][1].push_front(symbols::brace1);

    box[2][2].push_front(symbols::eps);

    box[2][4].push_front(symbols::Stmts);
    box[2][4].push_front(symbols::semi);
    box[2][4].push_front(symbols::Stmts);

    box[2][7].push_front(symbols::Stmts);
    box[2][7].push_front(symbols::semi);
    box[2][7].push_front(symbols::Stmts);

    box[2][10].push_front(symbols::Stmts);
    box[2][10].push_front(symbols::semi);
    box[2][10].push_front(symbols::Stmts);

    box[2][11].push_front(symbols::Stmts);
    box[2][11].push_front(symbols::semi);
    box[2][11].push_front(symbols::Stmts);

    box[3][4].push_front(symbols::Astmt);

    box[3][7].push_front(symbols::Ostmt);

    box[3][10].push_front(symbols::Wstmt);

    box[3][11].push_front(symbols::Fstmt);
   
    box[4][4].push_front(symbols::Y); 
    box[4][4].push_front(symbols::equal); 
    box[4][4].push_front(symbols::id);

    box[5][4].push_front(symbols::E);
    
    box[5][6].push_front(symbols::kwdinput);

    box[5][8].push_front(symbols::E); 

    box[5][15].push_front(symbols::E); 
    box[5][16].push_front(symbols::E); 
    box[5][17].push_front(symbols::E); 

    box[6][7].push_front(symbols::paren2);
    box[6][7].push_front(symbols::Elist);
    box[6][7].push_front(symbols::paren1);
    box[6][7].push_front(symbols::kwdprint);

    box[7][10].push_front(symbols::Block);
    box[7][10].push_front(symbols::Pexpr);
    box[7][10].push_front(symbols::kwdwhile);

    box[8][11].push_front(symbols::Else2);
    box[8][11].push_front(symbols::Block);
    box[8][11].push_front(symbols::Pexpr);
    box[8][11].push_front(symbols::kwdif);

    box[9][3].push_front(symbols::eps);

    box[9][12].push_front(symbols::Else2);
    box[9][12].push_front(symbols::Block);
    box[9][12].push_front(symbols::Pexpr);
    box[9][12].push_front(symbols::kwdelseif);

    box[9][13].push_front(symbols::Block);
    box[9][13].push_front(symbols::kwdelse);
//Elist
    box[10][4].push_front(symbols::Elist2);
    box[10][4].push_front(symbols::E);

    box[10][8].push_front(symbols::Elist2);
    box[10][8].push_front(symbols::E);

    box[10][9].push_front(symbols::eps);

    box[10][15].push_front(symbols::Elist2);
    box[10][15].push_front(symbols::E);

    box[10][16].push_front(symbols::Elist2);
    box[10][16].push_front(symbols::E);

    box[10][17].push_front(symbols::Elist2);
    box[10][17].push_front(symbols::E);

//Elist2
    box[11][14].push_front(symbols::Elist);
    box[11][14].push_front(symbols::comma);

//E'
    box[12][3].push_front(symbols::eps);

    box[12][9].push_front(symbols::eps);

    box[12][14].push_front(symbols::eps);

    box[12][18].push_front(symbols::Eprime);
    box[12][18].push_front(symbols::T);
    box[12][18].push_front(symbols::Opadd);

    box[12][19].push_front(symbols::Eprime);
    box[12][19].push_front(symbols::T);
    box[12][19].push_front(symbols::Opadd);

    //E
    box[13][4].push_front(symbols::Eprime);
    box[13][4].push_front(symbols::T);

    box[13][8].push_front(symbols::Eprime);
    box[13][8].push_front(symbols::T);

    box[13][15].push_front(symbols::Eprime);
    box[13][15].push_front(symbols::T);

    box[13][16].push_front(symbols::Eprime);
    box[13][16].push_front(symbols::T);

    box[13][17].push_front(symbols::Eprime);
    box[13][17].push_front(symbols::T);

    //T'
    box[14][3].push_front(symbols::eps);

    box[14][9].push_front(symbols::eps);

    box[14][14].push_front(symbols::eps);

    box[14][18].push_front(symbols::eps);

    box[14][19].push_front(symbols::eps);

    box[14][20].push_front(symbols::Tprime);
    box[14][20].push_front(symbols::F);
    box[14][20].push_front(symbols::Opmul);

    box[14][21].push_front(symbols::Tprime);
    box[14][21].push_front(symbols::F);
    box[14][21].push_front(symbols::Opmul);

    box[14][22].push_front(symbols::Tprime);
    box[14][22].push_front(symbols::F);
    box[14][22].push_front(symbols::Opmul);

    //T
    box[15][4].push_front(symbols::Tprime);
    box[15][4].push_front(symbols::F);

    box[15][8].push_front(symbols::Tprime);
    box[15][8].push_front(symbols::F);

    box[15][15].push_front(symbols::Tprime);
    box[15][15].push_front(symbols::F);

    box[15][16].push_front(symbols::Tprime);
    box[15][16].push_front(symbols::F);

    box[15][17].push_front(symbols::Tprime);
    box[15][17].push_front(symbols::F);

    //F
    box[16][4].push_front(symbols::Fatom);

    box[16][8].push_front(symbols::Pexpr);

    box[16][15].push_front(symbols::Pexpr);
    
    box[16][16].push_front(symbols::Pexpr);

    box[16][17].push_front(symbols::Pexpr);

    //Pexpr
    box[17][8].push_front(symbols::paren2);
    box[17][8].push_front(symbols::E);
    box[17][8].push_front(symbols::paren1);

    //Fatom
    box[18][4].push_front(symbols::id);

    box[18][15].push_front(symbols::SymbInt);

    box[18][16].push_front(symbols::SymbFloat);

    box[18][17].push_front(symbols::string);
 
    //Opadd
    box[19][18].push_front(symbols::plus);

    box[19][19].push_front(symbols::minus);

    //Opmul
    box[20][20].push_front(symbols::aster);
    box[20][21].push_front(symbols::slash);
    box[20][22].push_front(symbols::caret);
}

void readStream(queue<symbols> charStream)
{
    char x;
    std::string word;

    ifstream input("stream.txt");

    if(!input.is_open())
    {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }
    
    //find third word which is separated by spaces
    int spaceCount = 0;

    //when spaceCount == 2, the next read word will be stored 
    //and looked up as a symbol
    while(input.get(x))
    {
        if(x == ' ')
	{
	    spaceCount++;
	}
        

        if(x == ')' || x == '\n')
        {
            if(x == '\n')
	    { 
		spaceCount = 0;
		word = "";
		continue;
            }
           // cout << "The word grabbed is: ";
           // cout << word << endl;
            
            //Code to store words here
            int wordLocation;
            for(wordLocation = 0; wordLocation < 46; wordLocation++)
	    {
		cout << wordLocation << ": " << symNames[wordLocation] << " " << word << endl;
      		if(strcmp(symNames[wordLocation], word.c_str()) == 0)
		{
		    cout << symNames[wordLocation] << endl;
                    break;
	        }
            }
	    charStream.push(symbols(wordLocation));
            
            cout << charStream.front() << endl;
	    charStream.pop();

	    spaceCount = 0;
            word = "";
 	    continue;
        }

        if(spaceCount == 2)
        {
            word += x;                    	    
        }
    }
}
