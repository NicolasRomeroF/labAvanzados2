#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "structs.h"
#include "funciones.h"

//Posiciones para recorrer el puzzle
int dx[4] = {-1,0,1,0};
int dy[4] = {0,-1,0,1};

//Nodos que indican el inicio el final de la cola
struct Node* start = NULL;
struct Node* end = NULL;

//Nodos que indican el inicio el final de la pila
struct Node* stackStart = NULL;
struct Node* stackEnd = NULL;

//Minimo actual
int minMov = INT_MAX;

//Nodos que marcan el inicio y el final de la lista de visitados
struct Node* visited =NULL;
struct Node* visitedEnd =NULL;

void push(char** puzzle,int depth)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->puzzle = puzzle;
	node->depth = depth;
	node->next = stackStart;

	if(stackStart == NULL && stackEnd == NULL)
	{
		stackStart = stackEnd = node;
		return;
	}
	stackStart = node;
}

Node* pop()
{
	Node* node = stackStart;
	if(stackStart == NULL)
	{
		return NULL;
	}
	if(stackStart==stackEnd)
	{
		stackStart = NULL;
		stackEnd = NULL;
	}
	else
	{
		stackStart = stackStart->next;
	}
	return node;
}

//Funcion que encola un puzzle junto a su profundidad
void enqueue(char** puzzle, int depth)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->puzzle = puzzle;
	node->depth = depth;
	node->next = NULL;

	if(start == NULL && end == NULL)
	{
		start = end = node;
		return;
	}
	end->next = node;
	end = node;

}

//Funcion que añade un puzzle a lista de visitados
void addVisited(char** puzzle)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->puzzle = puzzle;
	node->depth = -1;
	node->next = NULL;

	if(visited == NULL && visitedEnd == NULL)
	{
		visited = visitedEnd = node;
		return;
	}
	visitedEnd->next = node;
	visitedEnd = node;


}

void freeVisited()
{
	Node* aux;
	while(visited != NULL)
	{
		freeMatrix(visited->puzzle);
		aux=visited->next;
		free(visited);
		visited=aux;
		
	}
	visited=NULL;
	visitedEnd=NULL;
}

//Funcion que desencola un elemento de la cola y lo retorna
//Salida: Nodo
Node* dequeue()
{
	Node* node = start;
	if(start == NULL)
	{
		return NULL;
	}
	if(start==end)
	{
		start = NULL;
		end = NULL;
	}
	else
	{
		start = start->next;
	}
	return node;
}

//Funcion que libera la memoria ocupada por una matriz (puzzle)
void freeMatrix(char** matrix)
{
	int i;
	for(i=0;i<3;i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

//Funcion que libera el espacio ocupado por un nodo
void freeNode(Node* n)
{
	//freeMatrix(n->puzzle);
	free(n);
}

//Funcion que asigna espacio para una matriz de NxM
char** createMatrix(int N, int M)
{
	char** matriz = (char**)malloc(sizeof(char*)*N);
	int i, j;
	if (matriz == NULL)
	{
		printf("ERROR: No se pudo asignar memoria a la matriz\n");
		return NULL;
	}
	for (i = 0; i < N; i++)
	{
		matriz[i] = (char*)malloc(sizeof(char) * M);
		if (matriz[i] == NULL)
		{
			printf("ERROR: No se pudo asignar memoria a la matriz\n");
			return NULL;
		}
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			matriz[i][j] = ' ';
		}
	}
	return matriz;
}

//Funcion que muestra por pantalla una matriz (puzzle)
void printMatrix(char** matriz, int N, int M)
{
	printf("\nInicio Matriz\n\n");
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%c ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\nFin Matriz\n\n");
}

//Funcion que lee el archivo de entrada y guarda los elemenos en una matriz
//Salida: Matriz con los elementos leidos
char** readFile(FILE* entrada)
{
	char** matriz = createMatrix(3,3);
	fscanf(entrada,"%c %c %c\n",&matriz[0][0],&matriz[0][1],&matriz[0][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[1][0],&matriz[1][1],&matriz[1][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[2][0],&matriz[2][1],&matriz[2][2]);
	return matriz;
}

//Funcion que compara dos matrices (puzzle)
//Salida: Entero que indica si son iguales o no (0: distindos, 1: iguales)
int compareMatrix(char** matriz1, char** matriz2)
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(matriz1[i][j] != matriz2[i][j])
			{
				return 0;
			}
		}
	}
	return 1;
}

//Funcion que crea la matriz solucion para comparar con los estados generados
//Salida: Matriz solucion
char** createFinalAnswer()
{
	int i,j;
	char count=1+'0';
	char** finalAnswer = createMatrix(3,3);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(count=='9')
			{
				finalAnswer[i][j]='x';
			}
			else
			{
				finalAnswer[i][j]=count;
			}
			count++;
		}
	}
	return finalAnswer;
}

//Funcion que busca un elemento en una matriz
//Salida: Posicion del elemento en la matriz
Position searchElementMatrix(char** puzzle,char c)
{
	Position pos;
	pos.x=-1;
	pos.y=-1;
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(puzzle[i][j]==c)
			{
				pos.x=i;
				pos.y=j;
				return pos;
			}
		}
	}
	return pos;
}

//Funcion que copia una matriz
//Salida: Copia de la matriz
char** copyMatrix(char** src)
{
	char** copy = createMatrix(3,3);
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			copy[i][j]=src[i][j];
		}
	}
	return copy;
}

//Funcion que retorna una matriz con los elementos cambiados de otra matriz
//Salida: Matriz con dupla de elementos intercambiados
char** swap(char** puzzle,Position pos1, Position pos2)
{
	char** new = copyMatrix(puzzle);
	new[pos1.x][pos1.y]=puzzle[pos2.x][pos2.y];
	new[pos2.x][pos2.y]=puzzle[pos1.x][pos1.y];
	return new;
}

//Funcion que muestra por pantalla un arreglo de tamaño dinamico
void printArray(Array* a)
{
	int i,size;
	size = a->size;
	for(i=0;i<size;i++)
	{
		printf("%d ",a->array[i]);
	}
	printf("\n");
}

//Funcion que comprueba si un estado ha sido visitado o no
//Salida: Entero que indica si fue visitado o no
int isVisited(char** puzzle)
{
	Node* n = visited;
	while(n!=NULL)
	{
		if(compareMatrix(puzzle,n->puzzle)==1)
		{
			return 1;
		}
		n=n->next;
	}
	return 0;
}

//Funcion que retorna el tamaño de la cola
//Salida: Tamaño de la cola
int sizeQueue()
{
	Node* n = start;
	int size=0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

//Funcion que retorna el tamaño de la pila
//Salida: Tamaño de la pila
int sizeStack()
{
	Node* n = stackStart;
	int size=0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

//Funcion que muestra por pantalla las matrices que ya han sido visitadas
void printVisited()
{
	Node* n = visited;
	while(n != NULL)
	{
		printMatrix(n->puzzle,3,3);
		n=n->next;
	}
}

//Funcion que retorna el tamaño de la lista de visitados
//Salida: Tamaño de la lista de salidas
int sizeVisited()
{
	Node* n = visited;
	int size = 0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

//Funcion que busca todos los caminos posibles que dado un puzzle inicial, se llegue al puzzle solucion
//Salida: Arreglo con las profundidades de todas las soluciones (pasos necesarios)
void solution(char** puzzle)
{	
	Position startPosition,swapPos; 
	push(puzzle,0);
	Node* node;
	int i;
	char** finalAnswer=createFinalAnswer();

	int size = 1;
	int cont=0;
	
	char** puzzleAux=puzzle;
	if(compareMatrix(puzzle,finalAnswer)==1)
	{
		minMov=0;

	}
	
	//
	//while(cont<1)
	while(stackStart != NULL)
	{
		node=pop();
		addVisited(node->puzzle);
		printf("cont: %d visited:%d stack:%d depth:%d minMov:%d\n",cont,sizeVisited(),sizeStack(),node->depth,minMov);		
		//printf("cont: %d depth:%d minMov:%d\n",cont,node->depth,minMov);

		if(node->depth < minMov)
		{
			startPosition = searchElementMatrix(node->puzzle,'x');
			//printf("start x: %d y:%d\n",startPosition.x,startPosition.y);
			for(i=0;i<4;i++)
			{

				if(startPosition.x + dx[i] < 3 && startPosition.x + dx[i] >= 0 && startPosition.y + dy[i] < 3 && startPosition.y + dy[i] >= 0)
				{
					swapPos.x=startPosition.x+dx[i];
					swapPos.y=startPosition.y+dy[i];
					
					puzzleAux = swap(node->puzzle,startPosition,swapPos);
					if(isVisited(puzzleAux)==0)
					{
						if(compareMatrix(puzzleAux,finalAnswer)==0)
						{
							//printf("ifswap x: %d y:%d\n",swapPos.x,swapPos.y);

							push(puzzleAux,(node->depth)+1);
							
						}		
						else
						{ 
							printf("solution\n");
							minMov=(node->depth)+1;
							printMatrix(puzzleAux,3,3);
							freeVisited();
						}

					}
					else
					{
						freeMatrix(puzzleAux);
					}
				}
			}
		}
		else{
			freeVisited();
		}
		
		cont++;

		freeNode(node);		
	}
}

void inicio()
{
	FILE* entrada = fopen("Entrada.in","r");
	char** puzzle = readFile(entrada);
	fclose(entrada);
	printf("Archivo leido\n");
	printf("Puzzle de entrada: \n");
	printMatrix(puzzle,3,3);
	printf("Buscando solucion...\n");
	printf("...\n");
	printf("...\n");
	printf("...\n");
	solution(puzzle);
	printf("Se requieren %d movimientos\n", minMov);
	FILE* salida = fopen("Salida.out","w");
	fprintf(salida,"Se requieren %d movimientos\n",minMov);
	fclose(salida);
	printf("Archivo de salida creado con exito\n");
}