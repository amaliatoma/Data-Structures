#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128

typedef struct Comanda Comanda;
typedef struct Node Node;
typedef struct ListaDubla ListaDubla;

struct Comanda {
	unsigned int id;
	char* status;
	char* nume_client;
	unsigned char nrProduse;
	float suma;
};

struct Node {
	Comanda comanda;
	Node* next;
	Node* prev;
};

struct ListaDubla {
	Node* head;
	Node* tail;
};

void printComandaToConsola(Comanda comanda) {
	printf("Id: %d\n", comanda.id);
	printf("Status: %s\n", comanda.status);
	printf("Nume client: %s\n", comanda.nume_client);
	printf("Numar produse: %d\n", comanda.nrProduse);
	printf("Suma: %5.2f\n", comanda.suma);
	printf("-----------------------------\n");
}

void inserareInListaDublaLaInceput(ListaDubla* lista, Comanda comanda) {
	Node* nodNou = malloc(sizeof(Node));
	nodNou->comanda = comanda;

	nodNou->prev = NULL;
	nodNou->next = lista->head;

	if (lista->head != NULL) {
		lista->head->prev = nodNou;
	}
	else
	{
		lista->tail = nodNou;
	}

	lista->head = nodNou;
}

void inserareInListaDublaLaSfarsit(ListaDubla* lista, Comanda comanda) {
	Node* nodNou = malloc(sizeof(Node));
	nodNou->comanda = comanda;

	nodNou->next = NULL;
	nodNou->prev = lista->tail;

	if (lista->tail != NULL) {
		lista->tail->next = nodNou;
	}
	else {
		lista->head = nodNou;
	}

	lista->tail = nodNou;
}

void parseListaBtoE(ListaDubla lista) {
	if (lista.head) {
		while (lista.head !=NULL) {
			printComandaToConsola(lista.head->comanda);
			lista.head = lista.head->next;
		}
	}
	else {
		printf("Lista este goala!");
	}
}


void parseListEtoB(ListaDubla lista) {
	if (lista.tail)
	{
		while (lista.tail != NULL) {
			printComandaToConsola(lista.tail->comanda);
			lista.tail = lista.tail->prev;
		}
	}
	else {
		printf("Lista este goala!");
	}
}


//stergere dupa id si returnare comenzii sterse
ListaDubla stergereNodeDupaId(ListaDubla lista, int id) {
	if (id == NULL) {
		return lista;
	}

	Node* nod = lista.head;

	while (nod && nod->comanda.id != id) {
		nod = nod->next;
	}

	if (nod != NULL) {
		if (nod->next == NULL && nod->prev == NULL) {
			lista.head = NULL;
			lista.tail = NULL;
		}
		else {
			if (nod->next == NULL) {
				nod->prev->next = NULL;
				lista.tail = nod->prev;
			}
			else if (nod->prev == NULL) {
				nod->next->prev = NULL;
				lista.head = nod->next;
			}
			else {
				nod->next->prev = nod->prev;
				nod->prev->next = nod->next;
			}
		}
		//free(nod->comanda.id);
		free(nod);
	}

	return lista;
}
Comanda readOneFromFile(FILE* fisier) {
	Comanda comanda;

	char* buffer[LINESIZE];
	char* token;

	fgets(buffer, LINESIZE, fisier);

	token = strtok(buffer, ",");
	comanda.id = atoi(token);

	token = strtok(NULL, ",");
	comanda.status = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(comanda.status, token);

	token = strtok(NULL, ",");
	comanda.nume_client = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(comanda.nume_client, token);

	token = strtok(NULL, ",");
	comanda.nrProduse = (int)atoi(token);

	token = strtok(NULL, ",");
	comanda.suma = atof(token);

	return comanda;
}

void readAllFromFile(const char* numeFisier, Comanda** vectorComenzi, int* nrComenzi) {
	FILE* f;
	f = fopen(numeFisier, "r");

	int nrComenziCitite = 0;
	*vectorComenzi = malloc(nrComenziCitite * sizeof(Comanda));

	while (!feof(f)) {
		Comanda comanda = readOneFromFile(f);
		nrComenziCitite++;
		*vectorComenzi = realloc(*vectorComenzi, nrComenziCitite * sizeof(Comanda));
		(*vectorComenzi)[nrComenziCitite - 1] = comanda;
	}

	*nrComenzi = nrComenziCitite;

	fclose(f);
}

float valoareaMedieComenziFinalizate(ListaDubla lista) {
	float valoare = 0;
	int nrComenziFinalizate = 0;
	if (lista.head != NULL) {
		while (lista.head != NULL) {
			if (strcmp(lista.head->comanda.status,"LIVRATA")==0) {
				valoare += lista.head->comanda.suma;
				nrComenziFinalizate++;
			}
			lista.head = lista.head->next;
		}

		return valoare/nrComenziFinalizate;
	}
	else {
		return valoare;
	}
}

int existaValoriDuplicateDupaId(ListaDubla lista) {
	int gasit = 0;
	if (lista.head != NULL) {
		if (lista.head == lista.tail) {
			return 0;
		}
		else {
			do {
				Node* nod = lista.head->next;
				while (nod != NULL) {
					if (lista.head->comanda.id == nod->comanda.id) {
						gasit = 1;
					}
					nod = nod->next;
				}
				lista.head = lista.head->next;
			} while (gasit == 0 && lista.head->next != NULL );

			if (gasit == 1) {
				return 1;
			}
			else return 0;
		}
	}
	else {
		return 0;
	}
}


ListaDubla stergereaListei(ListaDubla lista) {
	while (lista.head != NULL) {
		free(lista.head->comanda.status);
		free(lista.head->comanda.nume_client);

		Node* temp = lista.head;
		lista.head = lista.head->next;
		free(temp);
	}

	lista.head = NULL;
	lista.tail = NULL;

	return lista;
}

int main() {
	Comanda* vectorComenzi;
	int nrComenzi;

	readAllFromFile("comenzi.txt", &vectorComenzi, &nrComenzi);

	for (int i = 0; i < nrComenzi; i++) {
		printComandaToConsola(vectorComenzi[i]);
	}

	ListaDubla listaDubla;
	listaDubla.head = NULL;
	listaDubla.tail = NULL;

	inserareInListaDublaLaInceput(&listaDubla, vectorComenzi[0]);
	inserareInListaDublaLaInceput(&listaDubla, vectorComenzi[1]);
	inserareInListaDublaLaSfarsit(&listaDubla, vectorComenzi[2]);
	inserareInListaDublaLaSfarsit(&listaDubla, vectorComenzi[3]);
	inserareInListaDublaLaSfarsit(&listaDubla, vectorComenzi[4]);


	printf("===== PARSARE LISTA B TO E =====\n");
	parseListaBtoE(listaDubla);
	printf("\n\n");

	printf("===== PARSARE LISTA E TO B =====\n");
	parseListEtoB(listaDubla);
	printf("\n\n");


	printf("===== STERGERE ID 90 ======\n");
	parseListaBtoE(stergereNodeDupaId(listaDubla, 89));

	printf("===== Val medie comenzi finalizate ======\n");
	float val = valoareaMedieComenziFinalizate(listaDubla);
	printf("Valoare medie : %5.2f\n", val);


	printf("===== Exista valori duplicate? ======\n");
	int exista = existaValoriDuplicateDupaId(listaDubla);
	if (exista == 1) {
		printf("Da!\n");
	}
	else printf("Nu!\n");

	inserareInListaDublaLaSfarsit(&listaDubla, vectorComenzi[1]);
	printf("===== PARSARE LISTA B TO E =====\n");
	parseListaBtoE(listaDubla);
	printf("\n\n");

	printf("===== Exista valori duplicate? ======\n");
	int exista2 = existaValoriDuplicateDupaId(listaDubla);
	if (exista2 == 1) {
		printf("Da!\n");
	}
	else printf("Nu!\n");


	return 0;
}