#include <string.h>


typedef enum {
  INTEGER,
  FLOAT,
  CHAR,
  STRING,
  ARRAY,
  UNKNOWN
} TYPE;


//takes a keyword string
//returns its corresponding type
//return -1, if keyword not valid
TYPE getType(char* keyword) {
  if(strcmp(keyword,"int") == 0) {
    return INTEGER;
  } else if(strcmp(keyword,"float") == 0) {
    return FLOAT;
  } else if(strcmp(keyword,"char") == 0) {
    return CHAR;
  } else if(strcmp(keyword,"String") == 0) {
    return STRING;
  } else if(strcmp(keyword,"Array") == 0) {
    return ARRAY;
  } else {
    return UNKNOWN;
  }
}