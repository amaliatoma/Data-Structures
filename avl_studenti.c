#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define LINESIZE 128


typedef struct Student Student;
typedef struct NodAVL NodAVL;

struct Student {
	int id;
	char* nume;
	float nota;
};

struct NodAVL {
	Student student;
	NodAVL* dreapta;
	NodAVL* stanga;
};


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

// functie - calculare inaltime nod
int h(NodAVL* root) {
	if (root) {
		int inaltimeS = h(root->stanga);
		int inaltimeD = h(root->dreapta);

		if (inaltimeS > inaltimeD) {
			return 1 + inaltimeS;
		}
		else if (inaltimeS < inaltimeD) {
			return 1 + inaltimeD;
		}
	}
}
// functie calculare grad de echilibru
int gradDeEchilibru(NodAVL* root) {
	if (root) {
		return h(root->dreapta) - h(root->stanga);
	}
}

//rotire dreapta
NodAVL* rotireD(NodAVL* root) {
	if (root) {
		// fiul din stanga radacinii
		NodAVL* aux = root->stanga; 
		// schimbam pointerii
		root->stanga = aux->dreapta;
		//atribuim root-ul
		aux->dreapta = root;
		//schimbam roo-ul
		root = aux;
	}
	return root;
}

//rotire stanga
NodAVL* rotireS(NodAVL* root) {
	if (root) {
		//fiul din dreapta radacinii
		NodAVL* aux = root->dreapta;
		root->dreapta = aux->stanga;
		aux->stanga = root;
		root = aux;
	} 
	return root;
}

//inserare nod in avl
NodAVL* inserareInAVL(NodAVL* root, Student stud) {
	if (root) {
		if (stud.id > root->student.id) {
			root->dreapta = inserareInAVL(root->dreapta, stud);
		}
		else if (stud.id < root->student.id) {
			root->dreapta = inserareInAVL(root->stanga, stud);;
		}
		//calculam gradul de echilibru
		int ge = gradDeEchilibru(root);
		//avem 2 cazuri :
		//1. ge = 2 => ne uitam in dreapta
		if (ge == 2) {
			int geD = gradDeEchilibru(root->dreapta);
			if (geD != -1) {
				root->dreapta = rotireD(root->dreapta);
			}
			root = rotireS(root);
		}
		//2. ge= -2 => ne uitam in stanga
		if (ge == -2) {
			int geS = gradDeEchilibru(root->stanga);
			if (geS != 1) {
				root->stanga = rotireS(root->stanga);
			}
			root = rotireD(root);
		}
		return root;
	}
	else {
		NodAVL* nodNou = malloc(sizeof(NodAVL));
		nodNou->student = stud;
		nodNou->dreapta = NULL;
		nodNou->stanga = NULL;

		return nodNou;
	}
	
}

//tranversare inordine avl

//stergere nod din arbore avl

int main() {
	int nrStudenti;
	Student* vectorStudenti;

	readAllFromFile("studenti.txt", &vectorStudenti, &nrStudenti);

	for (int i = 0; i < nrStudenti; i++) {
		
	}
}