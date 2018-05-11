#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

#include <stdio.h>
#include <structs.h>

void enqueue(char** puzzle, int depth);
void addVisited(char** puzzle);
Node* dequeue();
void freeMatrix(char** matrix);
void freeNode(Node* n);
char** createMatrix(int N, int M);
void printMatrix(char** matriz, int N, int M);
char** readFile(FILE* entrada);
int compareMatrix(char** matriz1, char** matriz2);
char** createFinalAnswer();
Position searchElementMatrix(char** puzzle,char c);
char** copyMatrix(char** src);
char** swap(char** puzzle,Position pos1, Position pos2);
void printArray(Array* a);
int isVisited(char** puzzle);
int sizeQueue();
void printVisited();
int sizeVisited();
int compareVisited();
void solution(char** puzzle);
int max(Array* a);
void inicio();

#endif