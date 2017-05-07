#pragma once
#include <iostream>
#include <string>
#include <fstream>

const int TABLELENGTH = 84;

class Lexer
{
public:
	enum States {
		start = 1, ident = 2, endIdent = 3, num = 4, endNum = 5, eq = 6, eqeq = 7, semi = 8, floater = 9, endfloater = 10, stringStart = 11, stringEnd = 12, aster = 13, caret = 14, colon = 15,
		dot = 16, minus = 17, plus = 18, slash = 19, comment = 20, comma = 21, angle1 = 22, angle2 = 23, brace1 = 24, brace2 = 25, bracket1 = 26, bracket2 = 27, parens1 = 28, parens2 = 29, oparrow = 30,
		opne = 31, ople = 32, opge = 33, opshl = 34, opshr = 35, exclaim = 36
	};

	bool isKey = false;


	// Initialize the lexer
	int runLexer() {
		std::ofstream output("stream.txt");
		std::ifstream input("mycode.txt"); //Read in the code
		if (!input.is_open())
		{
			std::cout << "Error opening code file.\n";
		}

		int stateTable[37][TABLELENGTH];  //Generate the State Table
		createTable(stateTable);
		printTable(stateTable);

		int charLocation;
		int tokenLocation;
		int strLength = 0;
		int lineNum = 0;
		char c;
		std::string textLine;
		std::string currentToken;
		States currentState = start;

		while (std::getline(input, textLine))
		{
			lineNum++;
			charLocation = 0;
			tokenLocation = 0;
			//std::cout << textLine << std::endl;
			strLength = textLine.length();
			int ix = 1;
			//std::cout << " Length: " <<  strLength << std::endl;
			while (charLocation <= strLength)
			{

				switch (currentState)
				{
				case start:
				{
					c = textLine[charLocation];
					charLocation++;

					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}


					if (index > TABLELENGTH)
					{
						break;
					}
					//std::cout << "index: " << index << std::endl;


					//SateChange Function
					currentState = States(stateChange(stateTable, start, index));
					//std::cout << "index: " << index << " character: " << c << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl << std::endl;

					if (index == 71) //for whitespace
					{
						//ix++;
						tokenLocation++;
					}
					break;
				}
				case ident:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//StateChange Function
					currentState = States(stateChange(stateTable, ident, index));
					//std::cout << "index: " << index << " character: " << c << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl << std::endl;

					break;
				}
				case endIdent:
				{
					//Create ident token, return to starting state, back up character pointer
					int stringSize = charLocation - tokenLocation;
					char* temp = new char[stringSize];
					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//std::cout << "stringSize: " << stringSize << std::endl;
					for (int i = 0; i < stringSize - 1; i++)
					{
						// If the character is a tab, do not include in character array
						if ((int)textLine[tokenLocation] == 9)
						{
							tokenLocation++;
							stringSize -= 1;
							i--;
							//ix++;
						}
						else
						{
							temp[i] = textLine[tokenLocation];
							tokenLocation++;
						}
					}
					temp[stringSize - 1] = '\0';
					currentToken = std::string(temp);

					delete[] temp;

					currentState = start;
					charLocation--;

					// Check if identifier is a keyword
					// If true, then print formatted message for keyword. Else use other format
					currentToken = checkForKey(currentToken);

					if (isKey == true) {
						output << "(:token " << lineNum << " " << currentToken << ")\n";
						std::cout << "(:token " << lineNum << " " << currentToken << ")" << std::endl;
						ix++;
					}
					else {

						output << "(:token " << lineNum << " ident :ix " << ix << " :str \"";
						std::cout << "(:token " << lineNum << " ident :ix " << ix << " :str \"";

						ix++;

						output << currentToken << "\")\n";
						std::cout << currentToken << "\")" << std::endl;
						//std::cout << "Going To State: " << currentState << std::endl;
						//std::cout << "charLocation: " << charLocation << std::endl << std::endl;
						//tokenLocation = charLocation;
					}

					break;
				}
				case num:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//std::cout << "index: " << index;
					//std::cout << TABLELENGTH;
					if (index > TABLELENGTH)
					{
						output << "(:token " << lineNum << " int :str \"";
						std::cout << "(:token " << lineNum << " int :str \"";
						int stringSize = charLocation - tokenLocation;
						char* temp = new char[stringSize];

						//std::cout << "tokenLocation: " << tokenLocation << std::endl;
						//std::cout << "stringSize: " << stringSize << std::endl;
						for (int i = 0; i < stringSize - 1; i++)
						{
							temp[i] = textLine[tokenLocation];
							tokenLocation++;
						}
						temp[stringSize - 1] = '\0';
						currentToken = std::string(temp);
						delete[] temp;

						currentState = start;
						charLocation--;

						output << currentToken << "\")\n";
						std::cout << currentToken << "\")" << std::endl;
						break;
					}

					//SateChange Function
					currentState = States(stateChange(stateTable, num, index));
					//std::cout << "index: " << index << " character: " << c << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl << std::endl;

					break;
				}
				case endNum:
				{
					//Create num token, return to starting state, back up character pointer
					output << "(:token " << lineNum << " int :str \"";
					std::cout << "(:token " << lineNum << " int :str \"";
					int stringSize = charLocation - tokenLocation;
					char* temp = new char[stringSize];

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//std::cout << "stringSize: " << stringSize << std::endl;
					for (int i = 0; i < stringSize - 1; i++)
					{
						temp[i] = textLine[tokenLocation];
						tokenLocation++;
					}
					temp[stringSize - 1] = '\0';
					currentToken = std::string(temp);
					delete[] temp;

					currentState = start;
					charLocation--;

					output << currentToken << "\")\n";
					std::cout << currentToken << "\")" << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl;
					//std::cout << "charLocation: " << charLocation << std::endl << std::endl;
					//tokenLocation = charLocation;

					ix++;
					break;
				}
				case eq:
				{
					c = textLine[charLocation];
					charLocation++;

					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//SateChange Function
					currentState = States(stateChange(stateTable, eq, index));

					if (currentState != eqeq) //equal token else its opeq token
					{
						output << "(:token " << lineNum << " equal)\n";
						std::cout << "(:token " << lineNum << " equal)" << std::endl;
						charLocation--; //Back up location
						ix++;
					}

					if (currentState == eqeq)
					{
						charLocation--;
					}

					/*
					std::cout << "index: " << index << " character: " << c << std::endl;
					std::cout << "Going To State: " << currentState << std::endl;
					std::cout << "charLocation: " << charLocation << std::endl << std::endl;
					*/

					tokenLocation = charLocation;

					break;
				}
				case eqeq:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, eqeq, index));

					output << "(:token " << lineNum << " opeq)\n";
					std::cout << "(:token " << lineNum << " opeq)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case semi:
				{
					output << "(:token " << lineNum << " semi)\n";
					std::cout << "(:token " << lineNum << " semi)" << std::endl;
					currentState = start;
					ix++;
					break;
				}
				case floater:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}


					//SateChange Function
					currentState = States(stateChange(stateTable, floater, index));

					//std::cout << "index: " << index << " character: " << c << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl << std::endl;
					if (index > TABLELENGTH)
					{
						//Create num token, return to starting state, back up character pointer
						output << "(:token " << lineNum << " float :str \"";
						std::cout << "(:token " << lineNum << " float :str \"";
						int stringSize = charLocation - tokenLocation;
						char* temp = new char[stringSize];

						//std::cout << "tokenLocation: " << tokenLocation << std::endl;
						//std::cout << "stringSize: " << stringSize << std::endl;
						for (int i = 0; i < stringSize - 1; i++)
						{
							temp[i] = textLine[tokenLocation];
							tokenLocation++;
						}
						temp[stringSize - 1] = '\0';
						currentToken = std::string(temp);
						delete[] temp;

						currentState = start;
						charLocation--;

						output << currentToken << "\")\n";
						std::cout << currentToken << "\")" << std::endl;
					}
					ix++;
					break;
				}
				case endfloater:
				{
					//Create num token, return to starting state, back up character pointer
					output << "(:token " << lineNum << " float :str \"";
					std::cout << "(:token " << lineNum << " float :str \"";
					int stringSize = charLocation - tokenLocation;
					char* temp = new char[stringSize];

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//std::cout << "stringSize: " << stringSize << std::endl;
					for (int i = 0; i < stringSize - 1; i++)
					{
						temp[i] = textLine[tokenLocation];
						tokenLocation++;
					}
					temp[stringSize - 1] = '\0';
					currentToken = std::string(temp);
					delete[] temp;

					currentState = start;
					charLocation--;

					output << currentToken << "\")\n";
					std::cout << currentToken << "\")" << std::endl;
					//std::cout << "Going To State: " << currentState << std::endl;
					//std::cout << "charLocation: " << charLocation << std::endl << std::endl;
					//tokenLocation = charLocation;
					break;
				}
				case stringStart:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//std::cout << "charLocation: " << charLocation << std::endl << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//SateChange Function
					currentState = States(stateChange(stateTable, stringStart, index));
					if (index >= TABLELENGTH)
					{
						currentState = stringStart;
					}


					break;
				}
				case stringEnd:
				{
					//charLocation - 1;
					//std::cout << "charLocation: " << charLocation << std::endl;
					//Create num token, return to starting state, back up character pointer
					output << "(:token " << lineNum << " string :str ";
					std::cout << "(:token " << lineNum << " string :str ";
					int stringSize = charLocation - tokenLocation;
					char* temp = new char[stringSize];

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//std::cout << "stringSize: " << stringSize << std::endl;
					for (int i = 0; i < stringSize - 1; i++)
					{
						// If the character is a tab, do not include in character array
						if ((int)textLine[tokenLocation] == 9)
						{
							tokenLocation++;
							stringSize -= 1;
							i--;
						}
						else
						{
							temp[i] = textLine[tokenLocation];
							tokenLocation++;
						}
					}
					temp[stringSize - 1] = '\0';
					currentToken = std::string(temp);
					delete[] temp;

					currentState = start;
					charLocation--;

					output << currentToken << "\")\n";
					std::cout << currentToken << "\")" << std::endl;

					tokenLocation++;
					charLocation++;
					//std::cout << "Going To State: " << currentState << std::endl;
					//std::cout << "charLocation: " << charLocation << std::endl;
					//std::cout << "tokenLocation: " << tokenLocation << std::endl << std::endl;

					ix++;
					break;
				}
				case aster:
				{
					output << "(:token " << lineNum << " aster)\n";
					std::cout << "(:token " << lineNum << " aster)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case caret:
				{
					output << "(:token " << lineNum << " caret)\n";
					std::cout << "(:token " << lineNum << " caret)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case colon:
				{
					output << "(:token " << lineNum << " colon)\n";
					std::cout << "(:token " << lineNum << " colon)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case dot:
				{
					output << "(:token " << lineNum << " dot)\n";
					std::cout << "(:token " << lineNum << " dot)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case minus:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//StateChange Function
					currentState = States(stateChange(stateTable, minus, index));
					if (currentState == 30) // if the next state ->
					{
						charLocation--;
						//getting send to ->
					}
					else if (currentState != 4) //send to digit
					{
						output << "(:token " << lineNum << " minus)\n";
						std::cout << "(:token " << lineNum << " minus)" << std::endl;
						currentState = start;
						charLocation--;
					}


					tokenLocation++;
					ix++;
					break;
				}
				case plus:
				{
					output << "(:token " << lineNum << " plus)\n";
					std::cout << "(:token " << lineNum << " plus)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case slash:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//StateChange Function
					currentState = States(stateChange(stateTable, slash, index));

					if (currentState != 20)
					{
						output << "(:token " << lineNum << " slash)\n";
						std::cout << "(:token " << lineNum << " slash)" << std::endl;
						currentState = start;
						charLocation--;
						tokenLocation++;
					}
					if (currentState)
					{
						ix++;
						break;
					}
					ix++;
					break;
				}
				case comment:
				{
					do
					{
						c = textLine[charLocation];
						charLocation++;
					} while (c != '\n' && c != '\0');
					currentState = start;
					ix++;

					break;
				}
				case comma:
				{
					output << "(:token " << lineNum << " comma)\n";
					std::cout << "(:token " << lineNum << " comma)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;

					break;
				}
				case angle1:
				{
					c = textLine[charLocation];
					charLocation++;

					//std::cout << "tokenLocation: " << tokenLocation << std::endl;
					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//StateChange Function
					currentState = States(stateChange(stateTable, angle1, index));

					if (currentState == 32 || currentState == 34) //32 - ople <= || 34 - opshl <<
					{
						//dont need to print anything here, it will print in the next state
						//tokenLocation++ ?
						charLocation--;
						break;
					}
					else
					{
						output << "(:token " << lineNum << " angle1)\n";
						std::cout << "(:token " << lineNum << " angle1)" << std::endl;
						currentState = start;
						tokenLocation++;
					}
					ix++;

					break;
				}
				case angle2:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//StateChange Function
					currentState = States(stateChange(stateTable, angle2, index));

					if (currentState == 33 || currentState == 35) //33 - opge >= || 35 - opshr >>
					{
						//dont need to print anything here, it will print in the next state
						charLocation--;
						break;
					}
					else
					{
						output << "(:token " << lineNum << " angle2)\n";
						std::cout << "(:token " << lineNum << " angle2)" << std::endl;
						currentState = start;
						tokenLocation++;
					}
					ix++;

					break;
				}
				case brace1:
				{
					output << "(:token " << lineNum << " brace1)\n";
					std::cout << "(:token " << lineNum << " brace1)" << std::endl;
					currentState = start;
					//tokenLocation++;
					ix++;

					break;
				}
				case brace2:
				{
					output << "(:token " << lineNum << " brace2)\n";
					std::cout << "(:token " << lineNum << " brace2)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;

					break;
				}
				case bracket1:
				{
					output << "(:token " << lineNum << " brackets1)\n";
					std::cout << "(:token " << lineNum << " brackets1)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;

					break;
				}
				case bracket2:
				{
					output << "(:token " << lineNum << " brackets2)\n";
					std::cout << "(:token " << lineNum << " brackets2)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;

					break;
				}
				case parens1:
				{
					output << "(:token " << lineNum << " parens1)\n";
					std::cout << "(:token " << lineNum << " parens1)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;

					break;
				}
				case parens2:
				{
					output << "(:token " << lineNum << " parens2)\n";
					std::cout << "(:token " << lineNum << " parens2)" << std::endl;
					currentState = start;
					tokenLocation++;
					ix++;
					break;
				}
				case oparrow:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, oparrow, index));

					output << "(:token " << lineNum << " oparrow)\n";
					std::cout << "(:token " << lineNum << " oparrow)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case opne:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, opne, index));

					output << "(:token " << lineNum << " opne)\n";
					std::cout << "(:token " << lineNum << " opne)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case ople:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, ople, index));

					output << "(:token " << lineNum << " ople)\n";
					std::cout << "(:token " << lineNum << " ople)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case opge:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, opge, index));

					output << "(:token " << lineNum << " opge)\n";
					std::cout << "(:token " << lineNum << " opge)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case opshl:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, opshl, index));

					output << "(:token " << lineNum << " opshl)\n";
					std::cout << "(:token " << lineNum << " opshl)" << std::endl;
					tokenLocation = charLocation;

					//std::cout << "Going To State: " << currentState << std::endl;
					//std::cout << "charLocation: " << charLocation << std::endl;
					ix++;

					break;
				}
				case opshr:
				{
					c = textLine[charLocation];
					charLocation++;

					int index = 0;
					currentState = States(stateChange(stateTable, opshr, index));

					output << "(:token " << lineNum << " opshr)\n";
					std::cout << "(:token " << lineNum << " opshr)" << std::endl;
					tokenLocation = charLocation;
					ix++;

					break;
				}
				case exclaim:
				{
					c = textLine[charLocation];
					charLocation++;

					//loop through table and find character
					int index = 0;
					while (stateTable[0][index] != int(c) && index <= TABLELENGTH)
					{
						index++;
					}

					//SateChange Function
					currentState = States(stateChange(stateTable, exclaim, index));
					charLocation--;

					//Dont need to print this out
					ix++;

					break;
				}
				}
			}
		}

		return -1;
	}

	void createTable(int arr[][TABLELENGTH])
	{
		//Top row is filled in with all possible characters//
		arr[0][0] = 95; //_
		for (int i = 1; i < 27; i++) //[a-z]
		{
			arr[0][i] = i + 96;
		}
		for (int i = 27; i < 53; i++) //[A-Z]
		{
			arr[0][i] = i + 38;
		}
		for (int i = 53; i < 63; i++) //[0-9]
		{
			arr[0][i] = i - 5;
		}

		//Other punctuation
		arr[0][63] = 42; // *
		arr[0][64] = 94; // ^
		arr[0][65] = 58; // :
		arr[0][66] = 46; // .
		arr[0][67] = 61; // =
		arr[0][68] = 45; // -
		arr[0][69] = 43; // +
		arr[0][70] = 47; // /
		arr[0][71] = 32; // whitespace

						 //Unpaired Delimiters
		arr[0][72] = 44; // ,
		arr[0][73] = 59; // ;

		arr[0][74] = 34; // "

						 //Paired Delimiters
		arr[0][75] = 60; // <
		arr[0][76] = 62; // >
		arr[0][77] = 123; // {
		arr[0][78] = 125; // }
		arr[0][79] = 91; // [
		arr[0][80] = 93; // ]
		arr[0][81] = 40; // (
		arr[0][82] = 41; // )
		arr[0][83] = 33; // !

						 /*-------------------State 1--------------------*/ //start
		for (int i = 0; i < 53; i++) // LU send to state 2
		{
			arr[1][i] = 2;
		}
		for (int i = 53; i < 63; i++) //[0-9] to state 4
		{
			arr[1][i] = 4;
		}

		arr[1][63] = 13;
		arr[1][64] = 14;
		arr[1][65] = 15;
		arr[1][66] = 16;
		arr[1][67] = 6;
		arr[1][68] = 17; // state 17 will check if its a negative digit
		arr[1][69] = 18;
		arr[1][70] = 19;
		arr[1][71] = 1;
		arr[1][72] = 21; //comma
		arr[1][73] = 8;
		arr[1][74] = 11;

		arr[1][75] = 22; // <
		arr[1][76] = 23; // >
		arr[1][77] = 24; // {
		arr[1][78] = 25; // }
		arr[1][79] = 26; // [
		arr[1][80] = 27; // ]
		arr[1][81] = 28; // (
		arr[1][82] = 29; // )
		arr[1][83] = 36; // !

						 /*-------------------State 2--------------------*/ //ident
		for (int i = 0; i < 63; i++) // LUD send to state 2
		{
			arr[2][i] = 2;
		}

		for (int i = 63; i < TABLELENGTH; i++) // Anything not LUD send to state 3
		{
			arr[2][i] = 3;
		}

		/*-------------------State 3--------------------*/ //end ident
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to state 1
		{
			arr[3][i] = 1;
		}

		/*-------------------State 4--------------------*/ //number
		for (int i = 0; i < 53; i++) // Anything to state 5 except digits
		{
			arr[4][i] = 5;
		}

		for (int i = 53; i < 63; i++) // Digits
		{
			arr[4][i] = 4;
		}

		for (int i = 63; i < TABLELENGTH; i++)
		{
			arr[4][i] = 5;
		}

		arr[4][66] = 9; // Send to float state

						/*-------------------State 5--------------------*/ //end number
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to state 1
		{
			arr[5][i] = 1;
		}

		/*-------------------State 6--------------------*/ // =
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1 except = 
		{
			arr[6][i] = 1;
		}

		arr[6][67] = 7; // = to state 7
						/*-------------------State 7--------------------*/ // ==
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[7][i] = 1;
		}

		/*-------------------State 8--------------------*/ // ;
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[8][i] = 1;
		}

		/*-------------------State 9--------------------*/ // floating point num 
		for (int i = 0; i < TABLELENGTH; i++) // Anything to 10 except digits
		{
			arr[9][i] = 10;
		}
		for (int i = 53; i < 63; i++) // Digits
		{
			arr[9][i] = 9;
		}

		/*-------------------State 10--------------------*/ // end floating point
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[10][i] = 1;
		}

		/*-------------------State 11--------------------*/ // string start
		for (int i = 0; i < TABLELENGTH; i++) // Anything to 11 except "
		{
			arr[11][i] = 11;
		}
		arr[11][74] = 12;

		/*-------------------State 12--------------------*/ // string end
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[12][i] = 1;
		}

		/*-------------------State 13--------------------*/ // aster *
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[13][i] = 1;
		}

		/*-------------------State 14--------------------*/ // caret ^
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[14][i] = 1;
		}

		/*-------------------State 15--------------------*/ // colon :
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[15][i] = 1;
		}

		/*-------------------State 16--------------------*/ // dot .
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[16][i] = 1;
		}

		/*-------------------State 17--------------------*/ // minus -   needs to check next char to see if it should get sent to state 4
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[17][i] = 1;
		}
		for (int i = 53; i < 63; i++) // Digits
		{
			arr[17][i] = 4;
		}

		arr[17][76] = 30;  //send to oparrow ->

						   /*-------------------State 18--------------------*/ // plus +
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[18][i] = 1;
		}

		/*-------------------State 19--------------------*/ // slash /
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[19][i] = 1;
		}
		arr[19][70] = 20;

		/*-------------------State 20--------------------*/ // comment //
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[20][i] = 1;
		}

		/*-------------------State 21--------------------*/ // comma ,
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[21][i] = 1;
		}

		/*-------------------State 22--------------------*/ // <
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[22][i] = 1;
		}
		arr[22][67] = 32;  //send to <=
		arr[22][75] = 34; //send to <<

						  /*-------------------State 23--------------------*/ // >
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[23][i] = 1;
		}
		arr[23][67] = 33;  //send to >=
		arr[23][76] = 35; //send to >>

						  /*-------------------State 24--------------------*/ // {
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[24][i] = 1;
		}

		/*-------------------State 25--------------------*/ // }
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[25][i] = 1;
		}

		/*-------------------State 26--------------------*/ // [
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[26][i] = 1;
		}

		/*-------------------State 27--------------------*/ // ]
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[27][i] = 1;
		}

		/*-------------------State 28--------------------*/ // (
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[28][i] = 1;
		}

		/*-------------------State 29--------------------*/ // )
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[29][i] = 1;
		}
		/*--------------------------30-------------------*/
		for (int i = 0; i < TABLELENGTH; i++)
		{
			arr[30][i] = 1;
		}
		for (int i = 0; i < TABLELENGTH; i++)
		{
			arr[30][i] = 1;
		}
		/*--------------------------31-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[31][i] = 1;
		}
		/*--------------------------32-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[32][i] = 1;
		}
		/*--------------------------33-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[33][i] = 1;
		}
		/*--------------------------34-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[34][i] = 1;
		}
		/*--------------------------35-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) // Anything back to 1
		{
			arr[35][i] = 1;
		}
		/*--------------------------36-------------------*/
		for (int i = 0; i < TABLELENGTH; i++) //send everything to error state-----------------------------------------------------------------------------------------------MAKE ERROR STATE
		{
			arr[36][i] = 1;
		}
		arr[36][67] = 31; // will send to opne !=

	}

	std::string checkForKey(std::string toCheck)
	{
		std::string temp = toCheck;

		if (toCheck == "prog")
			temp = "kwdprog";
		else if (toCheck == "main")
			temp = "kwdmain";
		else if (toCheck == "fcn")
			temp = "kwdfcn";
		else if (toCheck == "class")
			temp = "kwdclass";
		else if (toCheck == "float")
			temp = "kwdfloat";
		else if (toCheck == "int")
			temp = "kwdint";
		else if (toCheck == "string")
			temp = "kwdstring";
		else if (toCheck == "if")
			temp = "kwdif";
		else if (toCheck == "elseif")
			temp = "kwdelseif";
		else if (toCheck == "else")
			temp = "kwdelse";
		else if (toCheck == "while")
			temp = "kwdwhile";
		else if (toCheck == "input")
			temp = "kwdinput";
		else if (toCheck == "print")
			temp = "kwdprint";
		else if (toCheck == "new")
			temp = "kwdnew";
		else if (toCheck == "return")
			temp = "kwdreturn";

		if (temp != toCheck) // Its a keyword, so change bool to true
			isKey = true;
		else
			isKey = false;

		return temp;
	}

	void printTable(int arr[][TABLELENGTH])
	{
		std::cout << "-------------------Print Table------------------" << std::endl;
		int j = 0;
		for (int i = 0; i < TABLELENGTH; i++)
		{
			if (j == 24)
			{
				std::cout << std::endl;
				j = 0;
			}
			std::cout << char(arr[0][i]) << ' ';
			j++;
		}
		std::cout << std::endl << "------------------------------------------------" << std::endl;
	}

	int stateChange(int arr[][TABLELENGTH], int currState, int index)
	{
		return arr[currState][index];
	}

	char peek_token()
	{
		return 0;
	}
};