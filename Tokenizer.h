/*
Gavin Cutchin
CMSC 403
Assignment 4 - Create a parser and tokenizer program
Tokenizer.h - header file for Tokenizer.c - includes Givens.h
*/
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"

//given and only function in tokenizer
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

#endif