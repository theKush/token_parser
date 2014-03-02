//Kush Patel
//Parser Project
//parser follows the recursive descent approach. 
//There are functions in the program that are defined to parse different components of the grammar.
//This functions rely on each other to parse the file. 

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "scanner.h"
using namespace std;
stringstream ss;

void program(stack<token> *tokens, stringstream *ss, int *indent);
void stmtlist(stack<token> *tokens, stringstream *ss, int *indent);
void stmt(stack<token> *tokens, stringstream *ss, int *indent);
void expr(stack<token> *tokens, stringstream *ss, int *indent);
void termtail(stack<token> *tokens, stringstream *ss, int *indent);
void term(stack<token> *tokens, stringstream *ss, int *indent);
void factortail(stack<token> *tokens, stringstream *ss, int *indent);
void factor(stack<token> *tokens, stringstream *ss, int *indent);
void addop(stack<token> *tokens, stringstream *ss, int *indent);
void multop(stack<token> *tokens, stringstream *ss, int *indent);
void addindent(string text, stringstream *ss, int *indent);
void match(stack<token> *tokens, string expected, stringstream *ss, int *indent);

//begin parsing
void parser(stack<token> *tokens,  stringstream* ss, int *indent){
	addindent("<program>", ss, indent);
	*indent += 1;
	program(tokens, ss, indent);
	*indent -= 1;
	addindent("</program>", ss, indent);
}
//prgogram
void program(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	if (temp.type == "id" || temp.type == "read" || temp.type == "write" || temp.type == "$$"){
		stmtlist(tokens, ss, indent);
		match(tokens, "$$", ss, indent);
	}
	else{
		ss->str("");
		*ss << "error";
		*ss << "\n";
	}
	}
}
//stmtlist
void stmtlist(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<stmt_list>", ss, indent);
	*indent +=1;
	if (temp.type == "id" || temp.type == "read" || temp.type == "write"){
		stmt(tokens, ss, indent);
		stmtlist(tokens, ss, indent);
		
	}
	else if (temp.type == "$$"){
		
	}
	else{
		ss->str("");
		*ss << "error";
		*ss << "\n";
	}
	*indent -= 1;
	addindent("</stmt_list>", ss, indent);
	}
}
//stmt
void stmt(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<stmt>", ss, indent);
	*indent +=1;
	if (temp.type == "id" ){
		match(tokens, "id", ss, indent);
		match(tokens, "assign", ss, indent);
		expr(tokens, ss, indent);	
	}
	else if (temp.type == "read"){
		match(tokens, "read", ss, indent);
		match(tokens, "id", ss, indent);
	}
	else if (temp.type == "write"){
		match(tokens, "write", ss, indent);
		expr(tokens, ss, indent);	
		
	}
	else{ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</stmt>", ss, indent);
	}
}
//expr
void expr(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<expr>", ss, indent);
	*indent +=1;
	if (temp.type == "id" || temp.type == "number" || temp.type == "left-parentheses" ){
		term(tokens, ss, indent);
		termtail(tokens, ss, indent);
	}
	else{ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</expr>", ss, indent);
	}
}
//termtail
void termtail(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<term_tail>", ss, indent);
	*indent += 1;
	if (temp.type == "plus" || temp.type == "minus"){
		addop(tokens, ss, indent);
		term(tokens, ss, indent);
		termtail(tokens, ss, indent);
	}
	else if(temp.type == "right-parentheses" || temp.type == "id" || temp.type == "read" || temp.type == "write" || temp.type == "$$"){
	}
	else {ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</term_tail>", ss, indent);
	}
}
//term
void term(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<term>", ss, indent);
	*indent +=1;
	if (temp.type == "id" || temp.type == "number" || temp.type == "left-parentheses" ){
		factor(tokens, ss, indent);
		factortail(tokens, ss, indent);
	}
	else{ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</term>", ss, indent);
	}
}
//factortail
void factortail(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<factor_tail>", ss, indent);
	*indent += 1;
	if (temp.type == "times" || temp.type == "divide"){
		multop(tokens, ss, indent);
		factor(tokens, ss, indent);
		factortail(tokens, ss, indent);
	}
	else if(temp.type == "plus" || temp.type == "minus" || temp.type == "right-parentheses" || temp.type == "id" || temp.type == "read" || temp.type == "write" || temp.type == "$$"){
	}
	else {
		ss->str("");
		*ss << "error";
		*ss << "\n";
		}
	*indent -= 1;
	addindent("</factor_tail>", ss, indent);
	}
}
//factor
void factor(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<factor>", ss, indent);
	*indent +=1;
	if (temp.type == "id"){
		match(tokens,"id", ss, indent);
	}
	else if (temp.type == "number"){
		match(tokens,"number", ss, indent);
	}
	else if (temp.type == "left-parentheses"){
		match(tokens,"left-parentheses", ss, indent);
		expr(tokens, ss, indent);
		match(tokens, "right-parentheses", ss, indent);
	}
	else{
		
		ss->str("");
		*ss << "error";
		*ss << "\n";

	}
	*indent -= 1;
	addindent("</factor>", ss, indent);
	}
}
//addop
void addop(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<add_op>", ss, indent);
	*indent +=1;
	if (temp.type == "plus"){
		match(tokens,"plus", ss, indent);
	}
	else if (temp.type == "minus"){
		match(tokens,"minus", ss, indent);
	}
	else{ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</add_op>", ss, indent);
	}
}
//multop
void multop(stack<token> *tokens, stringstream* ss, int *indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	addindent("<mult_op>", ss, indent);
	*indent +=1;
	if (temp.type == "times"){
		match(tokens,"times", ss, indent);
	}
	else if (temp.type == "divide"){
		match(tokens,"divide", ss, indent);
	}
	else{ss->str("");
		*ss << "error";
		*ss << "\n";}
	*indent -= 1;
	addindent("</mult_op>", ss, indent);
	}
}
//match - checks if the expected token is the right token , if it is then match function consumes the token and puts it in the buffer. else returns error.
void match(stack<token> *tokens, string expected, stringstream* ss, int * indent){
	if (!tokens->empty()){
	token temp = tokens->top();
	if (temp.type != expected){
		
		ss->str("");
		*ss << "error";
		*ss << "\n";
	}
	if (temp.type == "id" || temp.type == "assign" || temp.type == "read" || temp.type == "write" || temp.type == "plus" || temp.type == "minus" || temp.type == "times" || temp.type == "divide" || temp.type == "number"){
		addindent(("<" + temp.type + ">"), ss, indent);
		*indent += 1;
		addindent(temp.value, ss, indent);
		*indent -= 1;
		addindent(("</" + temp.type + ">"), ss, indent);
	}
	else {
		/*if (temp.type == "$$"){}
		else{
			*ss << "12error";}*/
	}
	tokens->pop();
	if (!tokens->empty()){
		temp = tokens->top();
	}
	}
}
//addindet - adds the tabs and the new lines to the output buffer
void addindent(string text, stringstream* ss, int *indent)
{
  int count = 0;
  
  // Add the proper number of indentations
  for (count=0; count<*indent; count++)
  {
    *ss << "\t";
  }

  // Add the text to the output buffer
  *ss << text;
  // Add a newline to the output buffer
  *ss << "\n";
}

#endif

