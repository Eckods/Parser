#include <iostream>
#include <fstream>
#include <stack>
#include <list>
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
void readStream();
void initMatrix(list<symbols> box[][21]);

int main()
{
    initMatrix(Matrix);    

    //Set up LL Parsing Stack
    stack<symbols> LLStack;

    //Bottom of stack is always $
    //Followed by Start Symbol (Pgm)
    LLStack.push(symbols::$);
    LLStack.push(symbols::Pgm);

    //Example code showing access and printing from matrix
    cout << symNames[Matrix[0][0].front()] << endl;
    Matrix[0][0].pop_front();
    cout << symNames[Matrix[0][0].front()] << endl;
    Matrix[0][0].pop_front();

    //readStream();

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

    //TODO: FILL IN REST OF VALUES  
}

void readStream()
{
    char x;
    ifstream input("stream.txt");

    if(!input.is_open())
    {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }
    
    while(input.get(x))
    {
       cout << x;
    }
}
