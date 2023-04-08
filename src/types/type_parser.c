#include "types.h"
#include <string.h>



//returns [TYPE* , data (either char* int* etc)]
//input must be the string after the '='
void** parseType(char* line) {

}

// //
// static TYPE getVarType(char* line) {
//   for(int i=0; line[i] != '\0'; i++) {
//     if(line[i] == '"') {

//     } else if() {

//     }
//   }
// }


//used to extract String enclosed in a " ... " assignment
static getString(char *line) {
  int str_len=0;
  for(int i=0; line[i] != '"'; i++) {
    str_len++;
  }
  char* parsed_str=malloc(sizeof(char)*str_len+1);
  for(int i=0; i < str_len; i++) {
    parsed_str[i]=line[i];
  }
  parsed_str[str_len]='\0';
  return parsed_str;
}