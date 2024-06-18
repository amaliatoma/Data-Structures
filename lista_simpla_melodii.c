#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Melodie Melodie;
typedef struct Nod Nod;

struct Melodie {
	int cod;
	char* denumire;
	char* artist;
	int durata; // in secunde
	float nota;
	char esteRomaneasca;
};

struct Nod {
	Melodie melodie;
	Nod* next;
};

//inserare la inceput in lista simpla
void inserareLaInceput(Nod** head, Melodie melodie) {

	Nod* nodNou = malloc(sizeof(Nod));
	
	nodNou->melodie = melodie;
	nodNou->next = *head;

	*head = nodNou;

}

void dezalocareMemorieMelodie(Melodie* melodie) {
	free(*melodie->denumire);
	free(*melodie->artist);
}

void printMelodieToConsole(Melodie melodie) {
	printf("-------------\nCod: %d\n", melodie.cod);
	printf("Denumire: %s\n", melodie.denumire);
	printf("Artist: %s\n", melodie.artist);
	printf("Durata: %d\n", melodie.durata);
	printf("Nota: %5.2f\n", melodie.nota);
	printf("Este Romaneasca? %d\n", melodie.esteRomaneasca);
}


//inserare la sfarsit in lista simpla
void inserareLaSfarsit(Nod** head, Melodie melodie) {
	Nod* nodNou = malloc(sizeof(Nod));

	if (*head != NULL) { // lista nu e goala
		//trebuie sa ajungem la ultimul element
		Nod* aux = *head;

		while (aux->next != NULL) {
			aux = aux->next;
		}

		//suntem la ultimul element
		
		nodNou->melodie = melodie;
		nodNou->next = NULL;

		aux->next = nodNou;

	}
	else // lista e goala
	{
		nodNou->melodie = melodie;
		nodNou->next = NULL;

		*head = nodNou;
	}
}

// tranversare lista simpla
void tranversareLista(Nod* head) {
	
	if (head != NULL) {
		

		while (head != NULL) {
			printMelodieToConsole(head->melodie);

			head = head->next;
		}
	}
	else {
		printf("Lista este goala!");
	}
}



Melodie readOneFromFile(FILE* fisier) {

	Melodie melodie;


	char buffer[LINESIZE];
	char* token;

	fgets(buffer, LINESIZE, fisier);

	token = strtok(buffer, ",");
	melodie.cod = atoi(token);

	token = strtok(NULL, ",");
	melodie.denumire = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(melodie.denumire, token);

	token = strtok(NULL, ",");
	melodie.artist = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(melodie.artist, token);

	token = strtok(NULL, ",");
	melodie.durata = (int)atoi(token);

	token = strtok(NULL, ",");
	melodie.nota = (float)atof(token);

	token = strtok(NULL, ",");
	melodie.esteRomaneasca = (char)atoi(token);
	

	return melodie;
}

void readAllFromFile(const char* numeFisier, Melodie** vectorMelodii, int* numarMelodii) {
	FILE* f;
	f = fopen(numeFisier, "r");

	int nrMelodiiCitite = 0;
	*vectorMelodii = malloc(nrMelodiiCitite * sizeof(Melodie));

	while (!feof(f)) {
		Melodie melodie = readOneFromFile(f);
		nrMelodiiCitite++;
		*vectorMelodii = realloc(*vectorMelodii, nrMelodiiCitite * sizeof(Melodie));
		(*vectorMelodii)[nrMelodiiCitite - 1] = melodie;
	}

	*numarMelodii = nrMelodiiCitite;

	fclose(f);

}


//sterg de la sfarsit si returnez elementul sters
Melodie stergereNodDeLaSfarsit(Nod** head) {
	if (*head != NULL) {
		if ((*head)->next) {
			Nod* aux = *head;
			//ma duc la penultimul nod
			while (aux->next->next != NULL) {
				aux = aux->next;
			}

			Melodie melodie = aux->next->melodie;
			free(aux->next);
			aux->next = NULL;
			return melodie;
		}
		else {
			Melodie melodie = (*head)->melodie;
			free((*head)->next);

			(*head)->next = NULL;

			return melodie;
			
		}
	}
	Melodie melodie;
	return melodie;
	
}

//sterg nod de la inceput si returnez melodia stearsa
Melodie stergereNodDeLaInceput(Nod** head) {
	if (*head != NULL) {
		Melodie melodie = (*head)->melodie;
		*head = (*head)->next;

		return melodie;
	}
	Melodie melodie;
	return melodie;
}

//cautare nod dupa cod-ul melodiei 
Melodie cautareMelodieInLista(Nod* head, int cod) {
	if (head != NULL) {
		
		while (head->next != NULL) {
			if (head->melodie.cod == cod) {
				Melodie melodie = head->melodie;
				return melodie;
			}
			head = head->next;
		}
	}
	else {
		Melodie melodie;
		return melodie;
	}
}

//void stergereLista(Nod* head) {
//	if (head != NULL) {
//		while (head != NULL) {
//			head->melodie = stergereNodDeLaInceput(head);
//			head = (head)->next;
//		}
//		free(head);
//	}
//	else
//	{
//		free(head);
//	}
//}

int lungimeaListei(Nod* head) {
	int contor =0;
	if (head) {
		while(head)
		{
			contor++;
			head = head->next;
		}
		return contor;
	}
	else {
		return contor;
	}
}

void sortareNoduriDupaId(Nod** head) {
	int sortat;
	Nod* pParcurgere; // pointer ul care parcurge lista 
	Nod* pSortat = NULL; //pointer ul care marcheaza sfarsitul listei sortate

	if (*head == NULL) {
		return;
	}

	do {
		sortat = 0;
		pParcurgere = *head;

		while (pParcurgere->next != pSortat) {
			if (pParcurgere->melodie.cod > pParcurgere->next->melodie.cod) {
				Melodie temp = pParcurgere->melodie;
				pParcurgere->melodie = pParcurgere->next->melodie;
				pParcurgere->next->melodie = temp;

				sortat = 1;
			}
			pParcurgere = pParcurgere->next;
		}
		pSortat = pParcurgere;

	} while (sortat == 1);
}

void swapNoduriById(Nod** head, int cod1, int cod2) {
	if (cod1 == cod2) {
		return;
	}
	Nod* prevX = NULL;
	Nod* currX = *head;

	Nod* prevY = NULL;
	Nod* currY = *head;


	//cautam primul element cu codul 1
	while (currX != NULL && currX->melodie.cod != cod1) {
		prevX = currX;
		currX = currX->next;
	}
	//cautam al doilea element cu codul 2
	while (currY != NULL && currY->melodie.cod != cod2) {
		prevY = currY;
		currY = currY->next;
	}

	// daca am ajuns la sfarsit => nu exista elem.
	if (currX == NULL || currY == NULL) {
		return;
	}

	if (prevX != NULL) {
		prevX->next = currY;
	}
	else {
		*head = currY;
	}

	if (prevY != NULL) {
		prevY->next = currX;
	}
	else {
		*head = currX;
	}

	Nod* temp = currY->next;
	currY->next = currX->next;
	currX->next = temp;
}

void dezalocareLista(Nod* head) {
	while (head != NULL) {
		Nod* temp = head;
		head = head->next;
		free(temp->melodie.denumire);
		free(temp->melodie.artist);
		free(temp);
	}
}

int main() {
	int numarMelodii;
	Melodie* vectorMelodii;

	readAllFromFile("melodii.txt", &vectorMelodii, &numarMelodii);

	printf("\n===== AFISARE MELODII ======\n");
	for (int i = 0; i < numarMelodii; i++) {
		printMelodieToConsole(vectorMelodii[i]);
	}

	//initializare lista simpla, goala la inceput

	Nod* head1 = NULL;
	inserareLaInceput(&head1, vectorMelodii[0]);
	inserareLaInceput(&head1, vectorMelodii[1]);
	inserareLaInceput(&head1, vectorMelodii[2]);
	inserareLaInceput(&head1, vectorMelodii[3]);
	inserareLaInceput(&head1, vectorMelodii[4]);
	inserareLaInceput(&head1, vectorMelodii[5]);

	printf("\n===== TRANVERSARE LISTA SIMPLA (elem adaugate la inceput)======\n");


	tranversareLista(head1);

	Nod* head2 = NULL;
	inserareLaSfarsit(&head2, vectorMelodii[0]);
	inserareLaSfarsit(&head2, vectorMelodii[1]);
	inserareLaSfarsit(&head2, vectorMelodii[2]);
	inserareLaSfarsit(&head2, vectorMelodii[3]);
	inserareLaSfarsit(&head2, vectorMelodii[4]);
	inserareLaSfarsit(&head2, vectorMelodii[5]);

	printf("\n===== TRANVERSARE LISTA SIMPLA (elem adaugate la sfarsit)======\n");


	tranversareLista(head2);


	printf("\n===== Sterg de la sfarsit (head2)  ======\n");
	Melodie melStearsa = stergereNodDeLaSfarsit(&head2);
	printMelodieToConsole(melStearsa);
	printf("--------------\n");
	tranversareLista(head2);


	printf("\n===== Sterg de la inceput (head2)  ======\n");
	Melodie melStearsa2 = stergereNodDeLaInceput(&head2);
	printMelodieToConsole(melStearsa2);
	printf("--------------\n");
	tranversareLista(head2);


	printf("\n===== Cautare si afisare melodie cu id ul 65 ======\n");
	Melodie m = cautareMelodieInLista(head2, 65);
	printMelodieToConsole(m);

	printf("==================\n");
	printf("Lungimea listeai head 2 este : %d\n", lungimeaListei(head2));

	
	printf("========SORTARE HEAD2==========\n");
	sortareNoduriDupaId(&head2);
	tranversareLista(head2);

	printf("========SORTARE HEAD1==========\n\n");
	sortareNoduriDupaId(&head1);
	tranversareLista(head1);

	
	printf("========SWAP==========\n\n");

	swapNoduriById(&head1, 65, 100);
	tranversareLista(head1);

}
