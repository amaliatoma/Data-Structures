#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Student Student;
typedef struct Heap Heap;

struct Student {
	int id;
	char* nume;
	float nota;
};

struct Heap {
	Student* vector;
	int dimensiune;
};

Student initializareStudent(int id, char* nume, float nota) {
	Student student;
	student.id = id;
	student.nume = malloc((strlen(nume) + 1) * sizeof(char));
	strcpy(student.nume, nume);
	student.nota = nota;

	return student;
}
Student readOneFromFile(FILE* f) {
	Student student;

	char* buffer[LINESIZE];
	char* token;

	if (fgets(buffer, LINESIZE, f)) {

		token = strtok(buffer, ",");
		student.id = atoi(token);

		token = strtok(NULL, ",");
		student.nume = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(student.nume, token);

		token = strtok(NULL, ",");
		student.nota = atof(token);
	}

	return student;
}

void readAllFromFile(const char* numeFisier, Student** vectorStudent, int* nrStudenti) {
	FILE* f;
	f = fopen(numeFisier, "r");

	int nrStudentiCititi = 0;
	(*vectorStudent) = malloc(nrStudentiCititi * sizeof(Student));

	while (!feof(f)) {
		Student student = readOneFromFile(f);
		nrStudentiCititi++;
		(*vectorStudent) = realloc(*vectorStudent, nrStudentiCititi * sizeof(Student));
		(*vectorStudent)[nrStudentiCititi - 1] = student;
	}

	*nrStudenti = nrStudentiCititi;

	fclose(f);
}

void swapStudenti(Student* student1, Student* student2) {
	Student aux = *student1;
	(*student1) = (*student2);
	(*student2) = aux;
}

void printStudent(Student student) {
	printf("Id: %d\nNume: %s\nNota: %5.2f\n-----------\n", student.id, student.nume, student.nota);
}

Heap initializareHeap() {
	Heap heap;
	heap.dimensiune = 0;
	heap.vector = malloc(heap.dimensiune * sizeof(Student));
	return heap;
}

void heapify(Heap heap, int pozRadacina) {
	int pozMax = pozRadacina;
	int left = 2 * pozRadacina + 1;
	int right = 2 * pozRadacina + 2;

	if (left < heap.dimensiune && heap.vector[left].id > heap.vector[pozMax].id) {
		pozMax = left;
	}

	if (right < heap.dimensiune && heap.vector[right].id > heap.vector[pozMax].id) {
		pozMax = right;
	}

	if (pozMax != pozRadacina) {
		swapStudenti(&heap.vector[pozMax], &heap.vector[pozRadacina]);
		if (pozMax * 2 + 1 < heap.dimensiune - 1) {
			heapify(heap, pozMax);
		}
	}
}

Heap creareHeap(Student* vectorStudenti, int dimensiune) {
	Heap heap;
	heap.dimensiune = dimensiune;
	heap.vector = malloc(heap.dimensiune * sizeof(Student));

	for (int i = 0; i < dimensiune; i++) {
		// heap.vector[i] = vectorStudenti[i]; -- shallow copy
		heap.vector[i] = initializareStudent(vectorStudenti[i].id, vectorStudenti[i].nume, vectorStudenti[i].nota);
	}

	int indexStart = (heap.dimensiune) / 2 - 1;

	for (int i = indexStart; i >= 0; i--) {
		heapify(heap, i);
	}

	return heap;
}

//inserare in Heap
void insertStudentInHeap(Heap* heap, Student student) {
	heap->dimensiune++;
	heap->vector = realloc(heap->vector, heap->dimensiune * sizeof(Student));

	int pozitie = heap->dimensiune - 1;
	int pozitieParinte = (pozitie - 1) / 2;

	heap->vector[pozitie].id = student.id;
	heap->vector[pozitie].nume = malloc((strlen(student.nume) + 1) * sizeof(char));
	strcpy(heap->vector[pozitie].nume, student.nume);
	heap->vector[pozitie].nota = student.nota;

	while (heap->vector[pozitie].id > heap->vector[pozitieParinte].id) {
		swapStudenti(&heap->vector[pozitie], &heap->vector[pozitieParinte]);

		pozitie = pozitieParinte;
		pozitieParinte = (pozitie - 1) / 2;
	}

	for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
		heapify(*heap, i);
	}
}

//extragere max din Heap

Student extragereMax(Heap* heap) {
	if (heap->dimensiune > 0) {
		Student student = heap->vector[0];
		// fac swap primul cu ultimul element din heap
		swapStudenti(&heap->vector[0], &heap->vector[heap->dimensiune - 1]);

		//ascundem ultimul element
		heap->dimensiune--;

		//refiltrare
		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			heapify(*heap, i);
		}

		return student;
	}
	else
	{
		return initializareStudent(0, "Anonim", 0);
	}

}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.dimensiune; i++) {
		printf("%d ", heap.vector[i].id);
	}

	printf("\n");
}

int main() {
	int nrStudenti;
	Student* vectorStudenti;

	readAllFromFile("studenti.txt", &vectorStudenti, &nrStudenti);

	for (int i = 0; i < nrStudenti; i++) {
		printStudent(vectorStudenti[i]);
	}

	Heap heap = creareHeap(vectorStudenti, nrStudenti);

	printf("==================\n");
	afisareHeap(heap);


	printf("==================\n");


	insertStudentInHeap(&heap, initializareStudent(34, "Lmao", 6.44));
	afisareHeap(heap);

	printf("==================\n");
	Student studentMax = extragereMax(&heap);
	printStudent(studentMax);


	return 0;
}