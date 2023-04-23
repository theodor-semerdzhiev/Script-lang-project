#include "math_expr.h"
#include "../bool.h"
#include "../utils/lineparsing.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static int checkNumberDeclarationSyntax(const char* expression, int start, int end);
//TODO <--- 
Prefix_Tree* parse_Arithmetic_exp(char* expression) {
  
  printf("%s",expression);
  

  return NULL;
}

//TODO
/*
There is a bug where the checkNumberDeclarationSyntax is starting from the last operator 
and it goes over the operator char causing problems
We only want to parse the value and leave the operator alone

*/

//will do a one pass scan for string to check if arithmetic expression as proper syntax
//1 --> good syntax
//0 --> bad syntax
int isArithmeticExprValid(char* expression) {
  int prev_index_of_op=-1; //used to keep track of last index of prev math operator char
  boolean sub_expr_inscope=FALSE; //used to keep track if sub expression is within scope (as we have to yet to encounter a math operator after the sub expression)
  boolean as_prev_op=TRUE; //there to check if have a previously valid math operator
  for(int i=0; expression[i] != '\0'; i++) {
    //if its whitespace we can skip
    if(isspace(expression[i]) != 0) continue;

    //these will check for the operator and then make sure they use proper values
    //will also update fields properly
    if(expression[i] == '*') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
    } else if(expression[i] == '+') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
    } else if(expression[i] == '-') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
    } else if(expression[i] == '/') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
    } else if(expression[i] == '%') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
    } else if(expression[i] == '(') {

      //if the last char (whitespace does not count), or if a sub expression is already in scope, we return 0
      if(sub_expr_inscope == TRUE || as_prev_op == FALSE) return 0;

      //we get the sub expression
      char* sub_exp=getScopedDelimitedString(&(expression[i]), '(', ')', 0);

      //if sub expression is invalid
      if(sub_exp == NULL) return 0; 

      //we have a recursive call to see if the sub expression is valid
      if(isArithmeticExprValid(sub_exp) == 0) return 0;
      //increments the i to the length of the sub expression (-1 because the for loop will increment)
      i+=strlen(sub_exp)+1;
      free(sub_exp);

      //update fields
      prev_index_of_op=i;
      sub_expr_inscope=TRUE;
      as_prev_op=FALSE;

    } else if(sub_expr_inscope==TRUE || as_prev_op == FALSE){

      //if there is no valid previous operator, then this must be an error
      return 0;
    }
  }
  return 1;
}

//this function will return 1 if the syntax for delclaration is proper 
//it will return 0 otherwise
static int checkNumberDeclarationSyntax(const char* expression, int start, int end) {
  boolean as_Met_Char=FALSE;
  boolean as_Met_end=FALSE;
  for(int i=start+1; i < end; i++) {
    
    if(isspace(expression[i]) != 0) continue;
    
    //if declaration is a variable
    if(expression[i] == '$') {
      //first character must be a letter
      if(isalpha(expression[i+1]) == 0) return 0;

      for(int j=i+2; j < end; j++) {
        //if we have met the end of token we expect whitespace
        if( as_Met_end == TRUE && 
            isspace(expression[j]) == 0) 
            return 0;
        
        //if its whitespace
        if(isspace(expression[j]) != 0) {
          as_Met_end=TRUE;
          continue;
        }
        //if the char is a alpha numeric, then its invalid syntax
        if(isalnum(expression[j]) == 0) return 0; 
      }
      return 1;
    } else if(isdigit(expression[i]) != 0) {
      int dotcount=0;
      for(int j=i; j < end; j++) {
        //if we have the end of the token, then we should only expect a space
        if(as_Met_end == TRUE && isspace(expression[j]) == 0) {
          return 0;
        
        //if its a digit we are good 
        } else if(isdigit(expression[j]) != 0) {
          as_Met_Char=TRUE;
        //if its a space then that means the end of token
        } else if(isspace(expression[j]) != 0) {
          as_Met_end=TRUE;
        //if its a dot then it must a be fraction, and we can only have one
        } else if( dotcount == 0 && expression[j] == '.') {
          dotcount++;
        //if its none of that then we return 0
        } else {
          return 0;
        }
      }
      //if we went through loop without problem, then we return 1
      return 1;
    } else {
      return 0;
    }
  }
  return 1;
}

int main() {
  char* str="* 123123 1 *";
  printf("%d", isArithmeticExprValid("(3.14 * 2.2 + 4.7) / 0.5 - 6.9 + 1.3 * (5.6 - 2.1) - 0.7 / 0.312a"));

}



