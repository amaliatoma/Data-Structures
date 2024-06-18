#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Prajitura Prajitura;
typedef struct Node Node;
typedef struct Stack Stack;
typedef struct Queue Queue;

struct Prajitura {
	int cod;
	char* nume;
	float numarCalorii;
};

struct Node {
	Prajitura prajitura;
	Node* next;
};

struct Stack {
	Node* top;
};

struct Queue {
	Node* front;
	Node* rear;
};

Prajitura citirePrajituraDinFisier (FILE* f){
	Prajitura prajitura;
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	prajitura.cod = atoi(buffer);

	fgets(buffer, LINESIZE, f);
	prajitura.nume = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(prajitura.nume, buffer);

	fgets(buffer, LINESIZE, f);
	prajitura.numarCalorii = atof(buffer);

	return prajitura;
}

void citirePrajituriDinFisier(const char* numeFisier, Prajitura** prajituriVector, int* numarPrajituri) {

	FILE* f;
	f = fopen(numeFisier, "r");
	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	*numarPrajituri = atoi(buffer);

	*prajituriVector = malloc((*numarPrajituri) * sizeof(Prajitura));
	for (int i = 0; i < (*numarPrajituri); i++) {
		(*prajituriVector)[i] = citirePrajituraDinFisier(f);
	}

	fclose(f);
}

void afisarePrajituraConsola(Prajitura prajitura) {
	printf("---------\nCOD: %d\nNume: %sNumar calorii: %5.2f\n", prajitura.cod, prajitura.nume, prajitura.numarCalorii);
}

void initializareStack(Stack* stack) {
	stack->top = NULL;
}

void initializareQueue(Queue* queue) {
	queue->front = queue->rear = NULL;
}

// operatii pentru stack
void push(Stack* stack, Prajitura prajitura) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->prajitura = prajitura;
	newNode->next = stack->top;

	stack->top = newNode;
}

Prajitura pop(Stack* stack) {
	if (stack->top == NULL) {
		printf("Stack-ul este gol!\n");
		exit(1);
	}
	Node* temp = stack->top;
	Prajitura popped = temp->prajitura;

	stack->top = stack->top->next;
	free(temp);

	return popped;
}

Prajitura peek(Stack* stack) {
	if (stack->top == NULL) {
		printf("Stack ul este gol!\n");
		exit(1);
	}
	return stack->top->prajitura;
}

void parseStackAndPrintPrajitura(Stack* stack) {
	Node* currentNode = stack->top;
	if (currentNode != NULL) {
		while (currentNode != NULL) {
			afisarePrajituraConsola(currentNode->prajitura);
			currentNode = currentNode->next;
		}
	}
	else {
		printf("Stack ul este gol!");
	}
}


// operatii pentru queue
void enqueue(Queue* queue, Prajitura prajitura) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->prajitura = prajitura;
	newNode->next = NULL;

	if (queue->rear == NULL) {
		queue->front = queue->rear = newNode;
	}
	else {
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}

Prajitura dequeue(Queue* queue) {
	if (queue->front == NULL) {
		printf("Queue ul este gol!");
		exit(1);
	}
	Node* temp = queue->front;
	Prajitura dequeued = temp->prajitura;

	queue->front = queue->front->next;

	// cand avem un singur element in coada
	if (queue->front == NULL) {
		queue->rear = NULL;
	}

	free(temp);
	return dequeued;


}

Prajitura front(Queue* queue) {
	if (queue->front == NULL) {
		printf("Queue is empty!\n");
		exit(1);
	}
	return queue->front->prajitura;
}


void parseQueueAndPrintPrajitura(Queue* queue) {
	Node* currentNode = queue->front;
	if (currentNode != NULL) {
		while (currentNode != NULL) {
			afisarePrajituraConsola(currentNode->prajitura);
			currentNode = currentNode->next;
		}
	}
	else {
		printf("Queue ul este gol!");
	}
}

int main() {
	int numarPrajituri;
	Prajitura* vectorPrajituri;

	// citire vector de Prajituri din fisier
	citirePrajituriDinFisier("prajituri_I.txt", &vectorPrajituri, &numarPrajituri);

	for (int i = 0; i < numarPrajituri; i++) {
		afisarePrajituraConsola(vectorPrajituri[i]);
	}

	printf("\n===========  PUSH PRAJITURI IN STACK  =================\n");


	//adaugare in stack 6 prajituri
	Stack stack;
	initializareStack(&stack);
	for (int i = 0; i < numarPrajituri; i++) {
		push(&stack, vectorPrajituri[i]);
	}
	parseStackAndPrintPrajitura(&stack);


	printf("\n===========  POP PRAJITURA 1 DIN STACK  =================\n");

	Prajitura popPrajitura1 = pop(&stack);
	afisarePrajituraConsola(popPrajitura1);

	printf("\n===========  POP PRAJITURA 2 DIN STACK  =================\n");

	Prajitura popPrajitura2 = pop(&stack);
	afisarePrajituraConsola(popPrajitura2);

	printf("\n===========  PEEK PRAJITURA DIN STACK  =================\n");

	Prajitura peekPrajitura = peek(&stack);
	afisarePrajituraConsola(peekPrajitura);



	printf("\n===========  ADAUGARE PRAJITURI IN QUEUE  =================\n");

	//adaugare in queue 6 prajituri
	Queue queue;
	initializareQueue(&queue);
	for (int i = 0; i < numarPrajituri; i++) {
		enqueue(&queue, vectorPrajituri[i]);
	}
	parseQueueAndPrintPrajitura(&queue);

	printf("\n===========  STERGERE PRAJITURA 1 DIN QUEUE  =================\n");
	Prajitura prajitureDeququed1 = dequeue(&queue);
	afisarePrajituraConsola(prajitureDeququed1);

	printf("\n===========  STERGERE PRAJITURA 2 DIN QUEUE  =================\n");
	Prajitura prajitureDeququed2 = dequeue(&queue);
	afisarePrajituraConsola(prajitureDeququed2);

	printf("\n===========  FRONT - PRAJITURA DIN STACK  =================\n");
	afisarePrajituraConsola(front(&queue));

	return 0;
}
