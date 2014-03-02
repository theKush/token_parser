#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <iomanip>
#include <ios>
#include "scanner.h"
#include "parser.h"
using namespace std;

//main function of the program reads in the input text file and prints the final stack
int main(int argc, char *argv[])
{
	string aLine;						//each line of the input file
	string allLines;					//final string created from the input file
	stack<char> charStack;				//stack of input characters 
	stack<token> tokens;				//final stack 
	
	string inputfile;
	if ( argc != 2 ) // argc should be 2 for correct execution
    //We print argv[0] assuming it is the program name
	{cout<<"usage: "<< argv[0] <<" <filename>\n";}
	else{inputfile = argv[1];}
	
	
	ifstream testFile;
	testFile.open(inputfile.c_str());			//open the text file
	//testFile.open("input.txt");
	if (testFile.is_open())
	{
		while (testFile.good())			//read through the end of the text file.
		{
			getline(testFile,aLine);	//read each line of the text file
			while(testFile){
				allLines = allLines + " " + aLine;	// combine each line of test file into one single string.
				getline(testFile,aLine);
				
			}	
			cout << endl;
			Break_Up_Into_Stack(allLines, charStack);		//break that input string into a stack of characters.
			
			//if stack of characters is not empty perform the scanner function and print the final stack
			if(!charStack.empty())
			{
				tokens = scanner(charStack);	//perform the scanner function on the stack of characters
				
				tokens = final_stack(tokens);	//remove any comments from the final stack
				print_stack(tokens);
				stringstream ss;
				
				int indent = 0;
				parser(&tokens, &ss, &indent); 
				string s;
				getline(ss, s);
				if(s == "error"){cout << "error"<< endl;}
				else{
					cout << ss.str();
				}
				
			}
		}
		testFile.close();						//close the text file
	}
	else cout << "Unable to open file" << endl;	//if text file not found

}