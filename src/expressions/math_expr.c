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

//will do a one pass scan for string to check if arithmetic expression as proper syntax
//1 --> good syntax
//0 --> bad syntax
int isArithmeticExprValid(char* expression) {
  int prev_index_of_op=-1; //used to keep track of last index of prev math operator char
  
  //used to keep track if sub expression is within scope (as we have to yet to encounter a math operator after the sub expression)
  boolean sub_expr_inscope=FALSE; 

  //used to check if we have a previously valid math operator (+-*/%)
  boolean as_prev_op=TRUE;
  //used to keep track if we have a digit in the current scope
  
  boolean as_met_digit_or_var=FALSE;
  for(int i=0; expression[i] != '\0'; i++) {
    if(isspace(expression[i]) != 0) continue;//if its whitespace we can skip
    //if(sub_expr_inscope == TRUE || as_prev_op == FALSE) return 0;
    if(isdigit(expression[i]) != 0 || expression[i] == '$' && sub_expr_inscope==FALSE) {
      as_met_digit_or_var=TRUE;
    }
    /*
    these will check for the operator and then make sure they use proper values
    will also update fields properly
    */
    if(expression[i] == '*') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
      as_met_digit_or_var=FALSE;
    } else if(expression[i] == '+') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
      as_met_digit_or_var=FALSE;
    } else if(expression[i] == '-') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
      as_met_digit_or_var=FALSE;
    } else if(expression[i] == '/') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
      as_met_digit_or_var=FALSE;
    } else if(expression[i] == '%') {
      if(checkNumberDeclarationSyntax(expression,prev_index_of_op,i) == 0)
        return 0;
      prev_index_of_op=i;
      sub_expr_inscope=FALSE;
      as_prev_op=TRUE;
      as_met_digit_or_var=FALSE;
    } else if(expression[i] == '(') {

      //if the last char (whitespace does not count), or if a sub expression is already in scope, we return 0
      if(sub_expr_inscope == TRUE || as_prev_op == FALSE || as_met_digit_or_var == TRUE) return 0;

      //we get the sub expression
      char* sub_exp=getScopedDelimitedString(&(expression[i]), '(', ')', 0);

      //if sub expression is invalid
      if(sub_exp == NULL) return 0; 

      //we have a recursive call to see if the sub expression is valid
      if(isArithmeticExprValid(sub_exp) == 0) return 0;
      //increments the i to the length of the sub expression (-1 because the for loop will increment)
      i+=strlen(sub_exp)+1;
      free(sub_exp);

      //updates fields
      sub_expr_inscope=TRUE;
      as_prev_op=FALSE;

    } else if(sub_expr_inscope==TRUE || as_prev_op == FALSE){

      //if there is no valid previous operator, then this must be an error
      return 0;
    
    //this will handle the last value declaration of our expression (not applied to sub expressions, this is handled seperately)
    } else if(expression[i+1] == '\0' && 
              sub_expr_inscope == FALSE && 
              checkNumberDeclarationSyntax(expression,prev_index_of_op,i+1) == 0) {
      return 0;
    }
  }
  if(as_met_digit_or_var == FALSE && sub_expr_inscope == FALSE) return 0;
  return 1;
}

//this function will return 1 if the syntax for delclaration is proper 
//it will return 0 otherwise
static int checkNumberDeclarationSyntax(const char* expression, int start, int end) {
  boolean as_Met_Char=FALSE;
  boolean as_Met_end=FALSE;
  
  if(start+1 == end) return 0; //that means there cannot be numbers in between

  for(int i=start+1; i < end; i++) {
    
    if(isspace(expression[i]) != 0) continue;
    
    //if declaration is a variable
    if(expression[i] == '$') {

      //TODO write variableDeclaration parser and use that instead

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

      //if its a random letter then its not proper syntax
    } else if(isalpha(expression[i]) != 0) {
      return 0;
      //if its a parenthesis, the calling function will check for that
    } else if(expression[i] == '(') {
      return 1;
    } else {
      return 0;
    }
  }
  return 1;
}

int main() {

  printf("----------VALID SYNTAX TESTS (output 1) ---------\n");

  printf("CASE 1:%d\n", isArithmeticExprValid("5 * (10 + 3) - 7 % 3"));
  printf("CASE 2:%d\n", isArithmeticExprValid("10 / 3 + 8.451342342 * 2"));
  printf("CASE 3:%d\n", isArithmeticExprValid("15 - 3 * 4.452 + 6 / 2"));
  printf("CASE 4:%d\n", isArithmeticExprValid("7 + 2 * 6.34 / 3 - 1.3423"));
  printf("CASE 5:%d\n", isArithmeticExprValid("(2 + 3.23) * (4 + 5) % 6"));
  printf("CASE 6:%d\n", isArithmeticExprValid("$num123 - 6.23 * (2 - 1) + 8 / 4"));
  printf("CASE 7:%d\n", isArithmeticExprValid("2 + 2 + 2 + 2 * 2"));
  printf("CASE 8:%d\n", isArithmeticExprValid("1 + 2 + 3.231 + 4 + 5"));
  printf("CASE 9:%d\n", isArithmeticExprValid("4 - 6 / 2 * 2"));
  printf("CASE 10:%d\n", isArithmeticExprValid("(5 + 3) % 4 * 6"));
  printf("CASE 11:%d\n", isArithmeticExprValid("2 * 3 + 5 * 4"));
  printf("CASE 12:%d\n", isArithmeticExprValid("20 / 2 / 2.15159 * 5"));
  printf("CASE 13:%d\n", isArithmeticExprValid("9 % 4 + 7 - 6 * 2"));
  printf("CASE 14:%d\n", isArithmeticExprValid("10 * 2 / 5.1 + 8.32"));
  printf("CASE 15:%d\n", isArithmeticExprValid("(3 + 2) * (8 - 4) / 6.5"));
  printf("CASE 16:%d\n", isArithmeticExprValid("5 * (8 / (6 - 2))"));
  printf("CASE 17:%d\n", isArithmeticExprValid("2 * (3.45 + 5) / (6 - 1)"));
  printf("CASE 18:%d\n", isArithmeticExprValid("7.23 * 3 - 6 / 2"));
  printf("CASE 19:%d\n", isArithmeticExprValid("(1 + 2.52 + 3) * (4 + 5) % 2"));
  printf("CASE 20:%d\n", isArithmeticExprValid("$num / (6 * (2.23 - 1)) + 8 % 4"));

  printf("----------INVALID SYNTAX TESTS (output 0) ---------\n");

  printf("CASE 1:%d\n", isArithmeticExprValid("5 * (10 + 3) - 7 % 3 +"));
  printf("CASE 2:%d\n", isArithmeticExprValid("10 / 3 +* 8 * 2"));
  printf("CASE 3:%d\n", isArithmeticExprValid("15 - 3 4 + 6 / 2"));
  printf("CASE 4:%d\n", isArithmeticExprValid("7 + 2 * 6 / 3 -"));
  printf("CASE 5:%d\n", isArithmeticExprValid("(2 + 3) * (4 + 5 % 6"));
  printf("CASE 6:%d\n", isArithmeticExprValid("12 - 6 * (2 - $var + 1 +) 8 / 4"));
  printf("CASE 7:%d\n", isArithmeticExprValid("2 + 2 ++ 2 + 2 * 2"));
  printf("CASE 8:%d\n", isArithmeticExprValid("1 + 2 + 3 + 4 + 5 ="));
  printf("CASE 9:%d\n", isArithmeticExprValid("4 - $var + 6 / 2 *"));
  printf("CASE 10:%d\n", isArithmeticExprValid("(5 + 3) % 4 * 6)"));
  printf("CASE 11:%d\n", isArithmeticExprValid("2 * 3 +* 5 * 4"));
  printf("CASE 12:%d\n", isArithmeticExprValid("20 / 2 / 2 +* 5"));
  printf("CASE 13:%d\n", isArithmeticExprValid("9 % 4 + 7 - $var + 6 * 2 +"));
  printf("CASE 14:%d\n", isArithmeticExprValid("10 * 2 / 5 8"));
  printf("CASE 15:%d\n", isArithmeticExprValid("($num + 2 * (8 - $var + 4) / 6"));
  printf("CASE 16:%d\n", isArithmeticExprValid("5 * (8 / (6 - $var + 2)) +"));
  printf("CASE 17:%d\n", isArithmeticExprValid("2 * (3 + 5 / (6 - $var + 1)"));
  printf("CASE 18:%d\n", isArithmeticExprValid("7 * 3 - 6 /"));
  printf("CASE 19:%d\n", isArithmeticExprValid("(1 + 2 + 3) * (4 + 5) %"));
  printf("CASE 20:%d\n", isArithmeticExprValid("12 / (6 * (2 - 1) + 8 % 4"));

  printf("----------LONG EXPRESSION TESTS (output 1) ---------\n");
  printf("CASE 1:%d\n", isArithmeticExprValid("((12.5 + (8.0 / (2.5 * (3 + 1)))) * ((6 * 3.7) - (4.8 * (3 - 1) / 0.5)) + (1.25 * (9 - 4) / (1.25 + 2.5)) - ((7.5 * 2.0 / (2 + 2)) + (3 - 1) * 1.5)) / (10 - 3 * (3 - (1 + 2 * (3 - 2) / 2)))"));
  printf("CASE 2:%d\n", isArithmeticExprValid("($num1 + 2 * ($var2 + 4) / 6) - (($var3 * 3 - $num2 / 5 + 1) * ($var4 + 2) + $num3) * ($var5 - 3) / ($num4 + $var6) + $var7 - $num5 * ($var8 + 2) / (($var9 - 1) * ($num6 + 4) + 2) * (($num7 + $var10) * ($var11 - $num8) / 5 - ($var12 * 3 + $num9) * ($var13 + 1) + $num10)"));
  printf("CASE 2:%d\n", isArithmeticExprValid("((1.5 * (2.5 + (3.2 - 0.7) * 2)) / (7 - 1.3 * (3 - (0.5 + 1.2 * (2 - 0.3) / 1.5))) + (4.8 - 1.2) * ((5.7 + 2.8) / (1.5 * (0.7 + 0.8))) - ((2.5 * 2.0 / (1 + 1)) + (2.2 - 0.8) * 1.5)) * (1.2 - 0.4 * (3 - (1 + 0.2 * (4 - 2) / 2))) + (0.6 - 0.2) * ((2.8 + 1.4) / (0.7 * (0.3 + 0.4))) - (($a1.25 * 2.0 / (0.5 + 0.5)) + (1.1 - 0.4) * 1.5)"));

  printf("----------VAR EXPRESSION TESTS (output 1) ---------\n");
  printf("CASE 1:%d\n", isArithmeticExprValid("$var1 * ($var2 + ($var3 / $var4) - $var5) - ($var6 + $var7) + ($var8 * ($var9 - $var10)) / $var11"));
  printf("CASE 2:%d\n", isArithmeticExprValid("($a + $b - $c) * ($d / $e) - ($f * $g) + $h"));
  printf("CASE 3:%d\n", isArithmeticExprValid("($var1 + $var2 + $var3) * $var4 / ($var5 - $var6)"));
  printf("CASE 4:%d\n", isArithmeticExprValid("$x + $y * $z - ($p / $q) + ($r - $s)"));
  printf("CASE 5:%d\n", isArithmeticExprValid("$a * ($b / $c) + $d - $e / ($f + $g) * $h + $i"));
  printf("CASE 6:%d\n", isArithmeticExprValid("($var1 + $var2 + $var3) * ($var4 + $var5 - $var6) / ($var7 * $var8)"));
  printf("CASE 7:%d\n", isArithmeticExprValid("$x * ($y + $z) / ($p - $q) + $r - $s + $t"));
  printf("CASE 8:%d\n", isArithmeticExprValid("($a - $b) * ($c / $d) + $e / $f - $g + $h * $i - $j / $k"));
  printf("CASE 9:%d\n", isArithmeticExprValid("$var1 * $var2 / ($var3 + $var4) - ($var5 + $var6 * $var7) + ($var8 - $var9)"));
  printf("CASE 10:%d\n", isArithmeticExprValid("($x + $y - $z) / ($p * $q) - $r * $s + $t / $u"));
}



