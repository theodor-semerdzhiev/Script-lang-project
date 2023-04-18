#include "types.h"
#include "./lists.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static int resize(List *list);

List* createList(int initialSize) {
  List* list = malloc(sizeof(List));
  list->length=0;
  list->Capacity=initialSize*2;
  list->list=malloc(sizeof(list->list)*list->Capacity);

  return list;
}

void append(List* list, Variable* var) {
  int index_to_insert_at=list->length;
  if(list->length == list->Capacity) {
    resize(list);
  }
  list->list[index_to_insert_at]=var;
  list->length++;
}

//this function removes an element at a specific index
//will return NULL if index is out of bounds
//pop does NOT call free on the returned variable struct
Variable* pop(List* list, int index) {  
  if(abs(index) >= list->length)
    return NULL;

  int offset_index;

  if(index < 0) offset_index=list->length+index;
  else          offset_index=index;   

  Variable* var=list->list[offset_index];
  for(int i=offset_index; i<list->length; i++) {
    list->list[i]=list->list[i+1];
  }
  list->length--;
  return var;
}

//this function return variable at specified index
//will return NULL if and only if index is out of bounds
Variable* get(List* list, int index) {
  if(abs(index) > list->length) {
    return NULL;
  } else if(index < 0) {
    return list->list[list->length+index-1];
  } else {
    return list->list[index];
  }
}

void list_free(List* list) {
  for(int i=0; i<list->length; i++) {
    freeVariableStruct(list->list[i]);
  }
  free(list);
}

void list_clear(List* list) {
  for(int i=0; i < list->length; i++) {
    free(list->list[i]);
  }
  list->length=0;

}

//resizes the array and returns the new size of the array
static int resize(List *list) {
  Variable** resized_list=malloc(sizeof(Variable*)*list->Capacity*2);
  list->Capacity*=2;
  for(int i=0; i<list->length; i++) {
    resized_list[i]=list->list[i];
  }
  free(list->list);
  list->list=resized_list;
  return list->Capacity;
}

// int main() {
//   List* list = createList(10);
//   for(int i=0; i< 100; i++) {
//     char* str=malloc(sizeof(char)*25);
//     strcpy(str,"var");
//     int *num=malloc(sizeof(int));
//     *num=i;
//     Variable *var=createVariableStruct(INTEGER,str,num,-1);
//     append(list,var);
//   }
//   for(int i=0; i<list->length ;i++) printf("%d,",list->list[i]->data.integer);
//   printf("\n%d",get(list,-1)->data.integer);
// }

