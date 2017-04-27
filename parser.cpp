#include <iostream>
#include <fstream>
using namespace std;

void readStream();

int main()
{

    readStream();

    return 0;
}

void readStream()
{

    ifstream input("stream.txt");

    if(!input.is_open())
    {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }
    
    char x;
    while(input.get(x))
    {
       cout << x;
    }
}
