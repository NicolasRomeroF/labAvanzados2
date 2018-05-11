#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct Node
{
	struct Node* next;
	char** puzzle;
	int depth;
}Node;

typedef struct Position
{
	int x,y;
}Position;

typedef struct Array
{
	int* array;
	int size;
}Array;

#endif