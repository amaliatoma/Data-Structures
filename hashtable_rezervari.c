#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128
#define HTABLE_SIZE 100

typedef struct Rezervare Rezervare;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Rezervare {
	unsigned int id;
	char* denumireHotel;
	unsigned char categorieHotel;
	char* numeClient;
	unsigned char nrZileRezervate;
	float suma;
};

struct Nod {
	Rezervare rezervare;
	Nod* next;
};

struct HashTable {
	Nod** vector;
	int dimensiune;
};

void printRezervare(Rezervare rezervare) {
	printf("Id: %d\n", rezervare.id);
	printf("Denumire hotel: %s\n", rezervare.denumireHotel);
	printf("Categorie hotel: %d\n", rezervare.categorieHotel);
	printf("Nume client: %s\n", rezervare.numeClient);
	printf("Nr zile rezervate: %d\n", rezervare.nrZileRezervate);
	printf("Suma de plata (RON): %5.2f\n", rezervare.suma);
	printf("----------------------------------\n");
}

//initializare hashtable
HashTable initializareHashTable() {
	HashTable ht;
	ht.dimensiune = HTABLE_SIZE;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * HTABLE_SIZE);

	for (int i = 0; i < HTABLE_SIZE; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

//implementare functie hash pentru cheie de tip char/int
int calculHash(unsigned char categorieHotel, int dimensiune) {
	if (dimensiune > 0) {
		return ((int)categorieHotel) % dimensiune;
	}
	return -1;
}

int calculHash2(char* numeClient, int dimensiune) {
	if (dimensiune > 0) {
		int rezultat=0;
		for (int i = 0; i < strlen(numeClient); i++) {
			rezultat += numeClient[i];
		}
		return (rezultat % dimensiune);
	}
	return -1;
}

//inserare in lista la final
void inserareLaFinal(Nod** head, Rezervare rezervare) {
	Nod* temp = malloc(sizeof(Nod));
	temp->rezervare = rezervare;
	temp->next = NULL;

	if (*head != NULL) {
		Nod* aux = (*head);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = temp;
	}
	else {
		(*head) = temp;
	}
}

//inserare in hashtable
void inserareInHT(HashTable ht, Rezervare rezervare) {
	if (ht.dimensiune > 0) {
		int pozitie = calculHash(rezervare.categorieHotel, ht.dimensiune);
		if (ht.vector[pozitie] != NULL) {
			inserareLaFinal(&(ht.vector[pozitie]), rezervare);
		}
		else {
			//coliziune => adaugam la finalul listei
			inserareLaFinal(&(ht.vector[pozitie]), rezervare);
		}
	}
}


void inserareInHT2(HashTable ht, Rezervare rezervare) {
	if (ht.dimensiune > 0) {
		int pozitie = calculHash2(rezervare.numeClient, ht.dimensiune);
		if (ht.vector[pozitie] != NULL) {
			inserareLaFinal(&(ht.vector[pozitie]), rezervare);
		}
		else {
			inserareLaFinal(&(ht.vector[pozitie]), rezervare);
		}
	}
}

void afisareListaRezervari(Nod* head) {
	while (head != NULL) {
		printRezervare(head->rezervare);
		head = head->next;
	}
}

void afisareTabela(HashTable ht) {
	for (int i = 0; i < ht.dimensiune; i++) {
		if (ht.vector[i] != NULL) {
			printf("Clusterul %d :\n", (i + 1));
			afisareListaRezervari(ht.vector[i]);
		}
	}
}

Rezervare readOneFromFile(FILE* f) {
	Rezervare rezervare;

	char* buffer[LINESIZE];
	char* token;

	fgets(buffer, LINESIZE, f);

	token = strtok(buffer, ",");
	rezervare.id = atoi(token);

	token = strtok(NULL, ",");
	rezervare.denumireHotel = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(rezervare.denumireHotel, token);

	token = strtok(NULL, ",");
	rezervare.categorieHotel = (char)atoi(token);

	token = strtok(NULL, ",");
	rezervare.numeClient = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(rezervare.numeClient, token);

	token = strtok(NULL, ",");
	rezervare.nrZileRezervate = (char)atoi(token);

	token = strtok(NULL, ",");
	rezervare.suma = atof(token);


	return rezervare;

}

void readAllFromFile(const char* numeFisier, Rezervare** vectorRezervari, int* nrRezervari) {
	FILE* f;
	f = fopen(numeFisier, "r");

	int nrRezervariCitite = 0;
	*vectorRezervari = malloc(nrRezervariCitite * sizeof(Rezervare));

	while (!feof(f)) {
		Rezervare rezervare = readOneFromFile(f);
		nrRezervariCitite++;
		*vectorRezervari = realloc(*vectorRezervari, nrRezervariCitite * sizeof(Rezervare));
		(*vectorRezervari)[nrRezervariCitite - 1] = rezervare;
	}

	*nrRezervari = nrRezervariCitite;
	fclose(f);
}


int numarRezervariCategorie(HashTable ht, int categorie) {
	if (ht.dimensiune > 0) {
		int nrRez = 0;
		int pozitie = calculHash(categorie, ht.dimensiune);
		Nod* head = ht.vector[pozitie];
		if (head != NULL) {
			while (head != NULL) {
				nrRez++;
				head = head->next;
			}
		}
		return nrRez;
	}
	else
		return 0;
	
}

HashTable modificareCategorieHotel(HashTable ht, char* numeHotel, int categorieNoua) {
	HashTable HTnou = initializareHashTable();
	if (ht.dimensiune > 0) {
		for (int i = 0; i < ht.dimensiune; i++) {
			Nod* head = ht.vector[i];
			if (head != NULL) {
				while (head != NULL) {
					if (strcmp(head->rezervare.denumireHotel, numeHotel) == 0) {
						head->rezervare.categorieHotel = categorieNoua;
						inserareInHT(HTnou, head->rezervare);
					}
					else {
						inserareInHT(HTnou, head->rezervare);
					}
					
					head = head->next;

					//free(temp->rezervare.denumireHotel);
					//free(temp->rezervare.numeClient);
					//free(temp);
				}
			}
		}
		//free(ht.vector);
		//ht.vector = NULL;
		return HTnou;
	}
	else
		return HTnou;
}


HashTable achizitionareHotel(HashTable ht, char* hotelCumparator, char* hotelAchizitionat) {
	HashTable HTnou = initializareHashTable();
	if (ht.dimensiune > 0) {
		for (int i = 0; i < ht.dimensiune; i++) {
			Nod* head = ht.vector[i];
			if (head != NULL) {
				while (head != NULL) {
					if (strcmp(head->rezervare.denumireHotel, hotelAchizitionat) == 0) {
						Rezervare temp;
						temp.id = head->rezervare.id;
						temp.denumireHotel = malloc((strlen(hotelCumparator) + 1) * sizeof(char));
						strcpy(temp.denumireHotel, hotelCumparator);
						temp.categorieHotel = head->rezervare.categorieHotel;
						temp.numeClient = malloc((strlen(head->rezervare.numeClient) + 1) * sizeof(char));
						strcpy(temp.numeClient, head->rezervare.numeClient);
						temp.nrZileRezervate = head->rezervare.nrZileRezervate;
						temp.suma = head->rezervare.suma;

						inserareInHT(HTnou, temp);
						
					}
					else {
						inserareInHT(HTnou, head->rezervare);
					}

					head = head->next;
				}
			}
		}

		return HTnou;
	} else
	return HTnou;
}

HashTable creareHTdupaCategorie(HashTable ht, int categorie) {
	HashTable HTnou = initializareHashTable();
	if (ht.dimensiune > 0) {
		for (int i = 0; i < ht.dimensiune; i++) {
			Nod* head = ht.vector[i];
			if (head != NULL) {
				while (head != NULL) {
					if (head->rezervare.categorieHotel == categorie) {
						inserareInHT2(HTnou, head->rezervare);
					}

					head = head->next;
				}
			}
		}
		return HTnou;
	}
	return HTnou;
}
int main() {
	Rezervare* vectorRezervari;
	int nrRezervari;

	readAllFromFile("rezervari.txt", &vectorRezervari, &nrRezervari);

	for (int i = 0; i < nrRezervari; i++) {
		printRezervare(vectorRezervari[i]);
	}


	printf("=====================================\n\n");


	HashTable tabela = initializareHashTable();
	inserareInHT(tabela, vectorRezervari[0]);
	inserareInHT(tabela, vectorRezervari[1]);
	inserareInHT(tabela, vectorRezervari[2]);
	inserareInHT(tabela, vectorRezervari[3]);
	inserareInHT(tabela, vectorRezervari[4]);
	

	afisareTabela(tabela);

	printf("=====================================\n\n");
	int nr = numarRezervariCategorie(tabela, 5);
	printf("Numarul de rezervari la hotelul cu categoria 5 este %d\n", nr);

	printf("=====================================\n\n");
	tabela = modificareCategorieHotel(tabela, "Hilton", 4);
	afisareTabela(tabela);

	printf("=====================================\n\n");
	tabela = achizitionareHotel(tabela, "Hilton", "Intercontinental");
	afisareTabela(tabela);


	inserareInHT(tabela, vectorRezervari[0]);
	printf("=====================================\n\n");
	HashTable htCategorie5 = creareHTdupaCategorie(tabela, 4);
	afisareTabela(htCategorie5);
}