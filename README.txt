Raul Esquivias, Steve Sanchez, David Moreno
CPSC 323
Charles Ciska
Language & Compiler Design Project 2

Additions:  LL(1) Parse Machine.  Utilizes character stream as a queue and LLStack as a stack.  Will successfully parse the information and generate a parse tree.  It will also let you know if there is an error in the grammar.

Issues:  Tree traversal has brought several issues in the form of segmentation faults.  Despite the tree being created using a list to store children, accessing them recursively has failed.

Compiling and Running: Compiled and Tested with c++11. In terminal, the proper command is as follows:
                       g++ -std=c++11 parser.cpp

Files Included:
	parser.cpp
  stream.txt
  a.out -included for convenience
  README.txt
