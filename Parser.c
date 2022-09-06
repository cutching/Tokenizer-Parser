/*
Gavin Cutchin
CMSC 403
Assignment 4 - Create a parser and tokenizer program
Parser.c - using single symbol lookahead recursively checks each lexeme 
of the input and validates it against the EBNF grammar - includes Givens.h
*/
#include "Parser.h"
#include "Givens.h"

static int lexicPos = 0;

//helper function that returns the token of the current position in the lexics structure
int getToken(struct lexics *someLexics){
    return someLexics[lexicPos].token;
}

//single symbol lookahead recursive decent parser
_Bool parser(struct lexics *someLexics, int numberOfLexics){
    //starts the first part of the grammar
    //parser -> function
    if(function(someLexics)){
        if(lexicPos == numberOfLexics){ //END OF ARRAY
            return TRUE;
        }
        else{
            return FALSE;
        }
    }
    else{
        return FALSE;
    }
}

//checks both header and body formatting
//function -> header body
_Bool function(struct lexics *someLexics){
    
    _Bool result = header(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    result = body(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    return result;
}

//checks the header formatting in the grammer
//header -> vartype identifier leftParenthesis [argdecl] rightParenthesis
_Bool header(struct lexics *someLexics){
    
    _Bool result = vartype(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    result = identifier(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    result = leftParenthesis(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    result = argDecl(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    result = rightParenthesis(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    return result;
}

//checks the argDecl formatting against the grammar
//argDecl -> vartype identifier {comma vartype identifier}
//vartype(someLexics) must return true when false because argDecl is optional
_Bool argDecl(struct lexics *someLexics){
    _Bool result = vartype(someLexics);
    if(result != TRUE){
        return TRUE;
    }

    result = identifier(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    //loops through each iteration of the format:
    //{comma vartype identifier}
    while(getToken(someLexics) == COMMA){
        
        result = comma(someLexics);
        if(result != TRUE){
            return FALSE;
        }

        result = vartype(someLexics);
        if(result != TRUE){
            return FALSE;
        }

        result = identifier(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    return result;
}

//checks the body format against the grammar
//body -> leftBracket [statementList] rightBracket
_Bool body(struct lexics *someLexics){
    
    _Bool result = leftBracket(someLexics);
    if(result != TRUE){
        return FALSE;
    }

    //only occurs if statementList is there, it is optional
    if(getToken(someLexics) == WHILE_KEYWORD || getToken(someLexics) == RETURN_KEYWORD 
    || getToken(someLexics) == IDENTIFIER || getToken(someLexics) == LEFT_BRACKET){
        result = statementList(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }

    result = rightBracket(someLexics);
    if(result != TRUE){
        return FALSE;
    }
    return result;
}

//check statementList format against the grammar
//statementList -> statement {statement}
_Bool statementList(struct lexics *someLexics){

    _Bool result = statement(someLexics);
    if(result != TRUE){
        return FALSE;
    }
    
    //checks for another iteration of statementList and then recursively calls it again
    if(getToken(someLexics) == WHILE_KEYWORD){
        return statementList(someLexics);
    }
    else if(getToken(someLexics) == RETURN_KEYWORD){
        return statementList(someLexics);
    }
    else if(getToken(someLexics) == IDENTIFIER){
        return statementList(someLexics);
    }
    else if(getToken(someLexics) == LEFT_BRACKET){
        return statementList(someLexics);
    }

    return result;
}

//checks statement format against the grammar
//statement -> while | return | assignment | body
_Bool statement(struct lexics *someLexics){
    _Bool result = TRUE;
    //only one of these value will be true otherwise false
    if(getToken(someLexics) == WHILE_KEYWORD){
        result = checkWhile(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    else if(getToken(someLexics) == RETURN_KEYWORD){
        result = checkReturn(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    else if(getToken(someLexics) == IDENTIFIER){
        result = assignment(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    else if(getToken(someLexics) == LEFT_BRACKET){
        //loops back up in the grammar to body
        result = body(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    else{
        return FALSE;
    }

    return result;
}

//check while format against the grammar
//while -> whileKeyword leftParenthesis expression rightParenthesis statement
_Bool checkWhile(struct lexics *someLexics){

    _Bool result = whileKeyword(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = leftParenthesis(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = expression(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = rightParenthesis(someLexics);
    if(result != TRUE){
            return FALSE;
    }
    //loops back up in the grammar to statement
    result = statement(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    return result;
}

//checks return format against the grammar
//return -> returnKeyword expression EOL
_Bool checkReturn(struct lexics *someLexics){

    _Bool result = returnKeyword(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = expression(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = EOLKeyword(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    return result;
}

//checks assignment format against the grammar
//assignment -> identifier equal expression EOL
_Bool assignment(struct lexics *someLexics){

    _Bool result = identifier(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = equalKeyword(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = expression(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    result = EOLKeyword(someLexics);
    if(result != TRUE){
            return FALSE;
    }

    return result;
}

//checks both sides of the grammar for expression
//expression -> term {BINOP term} | leftParenthesis expression rightParenthesis
_Bool expression(struct lexics *someLexics){
    _Bool result = TRUE;
    //checks for the left side of the expression
    if(getToken(someLexics) != LEFT_PARENTHESIS){
        result = term(someLexics);
        if(result != TRUE){
            return FALSE;
        }
        //since {BINOP term} can be repeated, checks for every iteration
        while(getToken(someLexics) == BINOP){
            lexicPos++;
            result = term(someLexics);
            if(result != TRUE){
                return FALSE;
            }
        }
    }
    else{
        //the right side of the expression
        //leftParenthesis expression rightParenthesis
        result = leftParenthesis(someLexics);
        if(result != TRUE){
            return FALSE;
        }

        result = expression(someLexics);
        if(result != TRUE){
            return FALSE;
        }

        result = rightParenthesis(someLexics);
        if(result != TRUE){
            return FALSE;
        }
    }
    return result;
}

/*****************************************
all functions that increment lexicPos below
*******************************************/


//checks the term format against the grammar
//term -> identifier | number
_Bool term(struct lexics *someLexics){
    //checks for either input and moves the lexicPos if true
    if(getToken(someLexics) == IDENTIFIER || getToken(someLexics) == NUMBER){
		lexicPos++;
		return TRUE;
	}
    else{
        return FALSE;
    }
}

//checks the vartype against the lexical structure by checking the token
//vartype -> int | void
_Bool vartype(struct lexics *someLexics){
    if(getToken(someLexics) == VARTYPE){
		lexicPos++;
		return TRUE;
	}
	else{
        return FALSE;
    }
}

//checks the identifier against the lexical structure by checking the token
//identifier -> [a-zA-Z][a-zA-Z0-9]*
_Bool identifier(struct lexics *someLexics){
    if(getToken(someLexics) == IDENTIFIER){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the leftParenthesis against the lexical structure by checking the token
//leftParenthesis -> (
_Bool leftParenthesis(struct lexics *someLexics){
    if(getToken(someLexics) == LEFT_PARENTHESIS){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the rightParenthesis against the lexical structure by checking the token
//rightParenthesis -> )
_Bool rightParenthesis(struct lexics *someLexics){
    if(getToken(someLexics) == RIGHT_PARENTHESIS){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the leftBracket against the lexical structure by checking the token
//leftBracket -> {
_Bool leftBracket(struct lexics *someLexics){
    if(getToken(someLexics) == LEFT_BRACKET){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the rightBracket against the lexical structure by checking the token
//leftBracket -> }
_Bool rightBracket(struct lexics *someLexics){
    if(getToken(someLexics) == RIGHT_BRACKET){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the comma against the lexical structure by checking the token
//comma -> ,
_Bool comma(struct lexics *someLexics){
    if(getToken(someLexics) == COMMA){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the whileKeyword against the lexical structure by checking the token
//whileKeyword -> while
_Bool whileKeyword(struct lexics *someLexics){
    if(getToken(someLexics) == WHILE_KEYWORD){
        lexicPos++;
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//checks the returnKeyword against the lexical structure by checking the token
//returnKeyword -> return
_Bool returnKeyword(struct lexics *someLexics){
    if(getToken(someLexics) == RETURN_KEYWORD){
		lexicPos++;
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//checks the EOLKeyword against the lexical structure by checking the token
//EOLKeyword -> ;
_Bool EOLKeyword(struct lexics *someLexics){
    if(getToken(someLexics) == EOL){
		lexicPos++;
		return TRUE;
	}
    else{
        return FALSE;
    }
}

//checks the equalKeyword against the lexical structure by checking the token
//equalKeyword -> =
_Bool equalKeyword(struct lexics *someLexics){
    if(getToken(someLexics) == EQUAL){
		lexicPos++;
		return TRUE;
	}
    else{
        return FALSE;
    }
}