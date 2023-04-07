#ifndef TYPES_H
#define TYPES_H

typedef enum{
  INTEGER,
  FLOAT,
  CHAR,
  STRING,
  ARRAY
} TYPE;

TYPE getType(char* keyword);

#endif /* TYPES_H */
