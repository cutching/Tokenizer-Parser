/*
Gavin Cutchin
CMSC 403
Assignment 4 - Create a parser and tokenizer program
Parser.h - header file for Parser.c - includes Givens.h
*/

#ifndef PARSER_H
#define PARSER_H

#include "Givens.h"
//A list of all methods used in the EBNF grammer
//Written top-down from the grammar
int getToken(struct lexics *someLexics);
_Bool parser(struct lexics *someLexics, int numberOfLexics);
_Bool function(struct lexics *someLexics);
_Bool header(struct lexics *someLexics);
_Bool argDecl(struct lexics *someLexics);
_Bool body(struct lexics *someLexics);
_Bool statementList(struct lexics *someLexics);
_Bool statement(struct lexics *someLexics);
_Bool checkWhile(struct lexics *someLexics);
_Bool checkReturn(struct lexics *someLexics);
_Bool assignment(struct lexics *someLexics);
_Bool expression(struct lexics *someLexics);
_Bool term(struct lexics *someLexics);
_Bool vartype(struct lexics *someLexics);
_Bool identifier(struct lexics *someLexics);
_Bool leftParenthesis(struct lexics *someLexics);
_Bool rightParenthesis(struct lexics *someLexics);
_Bool leftBracket(struct lexics *someLexics);
_Bool comma(struct lexics *someLexics);
_Bool rightBracket(struct lexics *someLexics);
_Bool whileKeyword(struct lexics *someLexics);
_Bool returnKeyword(struct lexics *someLexics);
_Bool EOLKeyword(struct lexics *someLexics);
_Bool equalKeyword(struct lexics *someLexics);

#endif