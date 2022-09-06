/*
Gavin Cutchin
CMSC 403
Assignment 4 - Create a parser and tokenizer program
Tokenizer.c
*/
#include "Tokenizer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf){
    //initialize count to track numLex
    int count = 0;
    //create a string to hold the stated max possible char in a line (256)
    char fileToString[MY_CHAR_MAX];

    //read the file while there are still elements to read
    while(fgets(fileToString, MY_CHAR_MAX, inf) != NULL){
        //int to keep track of where we are in the file line
        int currentLineChar = 0;

        //loop while we check that we arent at the end of the line either with \0 or \n
        while((fileToString[currentLineChar] != '\n') && (fileToString[currentLineChar] != '\0')){
            //create string to populate with the current lexeme
            char newLexeme[MY_CHAR_MAX];
            //int to keep track of string length and location
            int lexLocation = 0;
            //token variable to keep track of enum
            int currToken = 0;
            
            //check if current character is alphabetical
            if(isalpha(fileToString[currentLineChar])){

                //loops through the rest of the word copying to the new lexeme string as long as it's alphanumeric
                while(isalpha(fileToString[currentLineChar]) || isdigit(fileToString[currentLineChar])){
                    newLexeme[lexLocation] = fileToString[currentLineChar];
                    lexLocation++;
                    currentLineChar++;
                }

                //terminate the lexeme string with \0
                newLexeme[lexLocation + 1] = '\0';
                
                /*
                compares for an appropriate enum type to set the token to
                */
                if(strncmp(newLexeme, "while\0", 5) == 0){
                    currToken = WHILE_KEYWORD;
                }
                else if(strncmp(newLexeme, "return\0", 6) == 0){
                    currToken = RETURN_KEYWORD;
                }
                else if(strncmp(newLexeme, "int\0", 3) == 0 || strncmp(newLexeme, "void\0", 4) == 0){
                    currToken = VARTYPE;
                }
                else{
                    currToken = IDENTIFIER;
                }
            } //check if current character is numeric otherwise
            else if(isdigit(fileToString[currentLineChar])){
                //copy numeric value to the new lexeme
                newLexeme[lexLocation] = fileToString [currentLineChar];
                lexLocation++;
                currentLineChar++;
                while(isdigit(fileToString[currentLineChar])){
                    newLexeme[lexLocation] = fileToString [currentLineChar];
                    lexLocation++;
                    currentLineChar++;
                }
                //token can only be number if all numeric
                currToken = NUMBER;
            } //check for spaces and ignore
            else if(isspace(fileToString[currentLineChar])){
                lexLocation++;
                currentLineChar++;
                //return to beginning of loop
                continue;
            }
            else{
                //handles special characters going down the line from the given EBNF grammar and sets the token accordingly
                newLexeme[lexLocation] = fileToString[currentLineChar];

                if(fileToString[currentLineChar] == '('){
                    currToken = LEFT_PARENTHESIS;
                }
                else if(fileToString[currentLineChar] == ')'){
                    currToken = RIGHT_PARENTHESIS;
                }
                else if(fileToString[currentLineChar] == '{'){
                    currToken = LEFT_BRACKET;
                }
                else if(fileToString[currentLineChar] == '}'){
                    currToken = RIGHT_BRACKET;
                }
                else if(fileToString[currentLineChar] == ','){
                    currToken = COMMA;
                }
                else if(fileToString[currentLineChar] == ';'){
                    currToken = EOL;
                }
                else if((fileToString[currentLineChar] == '+') || (fileToString[currentLineChar] == '*') || (fileToString[currentLineChar] == '%')){
                    currToken = BINOP;
                } //check for != operator
                else if(fileToString[currentLineChar] == '!'){
                    if(fileToString[currentLineChar + 1] == '='){
                        lexLocation++;
                        currentLineChar++;
                        newLexeme[lexLocation] = fileToString[currentLineChar];
                        currToken = BINOP;
                    }
                    else{
                        return FALSE;
                    }
                } //check for == operator
                else if(fileToString[currentLineChar] == '='){
                    if((fileToString[currentLineChar + 1] == '=') && (fileToString[currentLineChar + 2] != '=')){
                        lexLocation++;
                        currentLineChar++;
                        newLexeme[lexLocation] = fileToString[currentLineChar];
                        currToken = BINOP;
                    }
                    else{
                        currToken = EQUAL;
                    }
                }
                else{
                    //do nothing
                }
                lexLocation++;
                currentLineChar++;
            }

            
            //allocate memory for a new Lexic structure to store our lexemes and tokens
            struct lexics *newLexicStructure = (struct lexics *) malloc(sizeof(struct lexics));
            
            //copy all lexemes to the structure
            for(int i = 0; i < lexLocation; i++){
                (*newLexicStructure).lexeme[i] = newLexeme[i];
            }

            //update token for the structure
            (*newLexicStructure).token = currToken;

            //update the aLex array to the new structure
            aLex[count] = *newLexicStructure;

            //update count
            count++;

            //reset the newLexeme variable
            memset(newLexeme, 0, MY_CHAR_MAX);
        }
    }

    //update numLex to the total count
    *numLex = count;
    return TRUE;
}