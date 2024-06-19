#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Farmacie Farmacie;
typedef struct NodABC NodABC;


struct Farmacie {
	unsigned short int id;
	char* denumire;
	float cifraDeAfaceri;
	char* numeSef;
	unsigned char nrAngajati;
	char* localitate;
};

struct NodABC {
	Farmacie farmacie;
	NodABC* dreapta;
	NodABC* stanga;
};


Farmacie readOneFromFile(FILE* f) {
	Farmacie farmacie;

	char buffer[LINESIZE];
	char* token;

	

	if (fgets(buffer, LINESIZE, f)) {
		buffer[strcspn(buffer, "\n")] = '\0';

		token = strtok(buffer, ",");
		farmacie.id = atoi(token);

		token = strtok(NULL, ",");
		farmacie.denumire = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(farmacie.denumire, token);

		token = strtok(NULL, ",");
		farmacie.cifraDeAfaceri = atof(token);

		token = strtok(NULL, ",");
		farmacie.numeSef = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(farmacie.numeSef, token);

		token = strtok(NULL, ",");
		farmacie.nrAngajati = atoi(token);

		token = strtok(NULL, ",");
		farmacie.localitate = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(farmacie.localitate, token);
	}

	return farmacie;
}

void readAllFromFile(const char* numeFisier, Farmacie** vectorFarmacii, int* nrFarmacii) {
	FILE* f;
	f = fopen(numeFisier, "r");

	int nrFarmaciiCitite = 0;
	*vectorFarmacii = malloc(nrFarmaciiCitite * sizeof(Farmacie));

	while (!feof(f)) {
		Farmacie farmacie = readOneFromFile(f);
		nrFarmaciiCitite++;
		(*vectorFarmacii) = realloc((*vectorFarmacii), nrFarmaciiCitite * sizeof(Farmacie));
		(*vectorFarmacii)[nrFarmaciiCitite - 1] = farmacie;
	}

	*nrFarmacii = nrFarmaciiCitite;

	fclose(f);
}

void printFarmacie(Farmacie farmacie) {
	printf("Id: %d\n", farmacie.id);
	printf("Denumire: %s\n", farmacie.denumire);
	printf("Cifra de afaceri: %5.2f\n", farmacie.cifraDeAfaceri);
	printf("Farmacist Sef: %s\n", farmacie.numeSef);
	printf("Numar angajati: %d\n", farmacie.nrAngajati);
	printf("Localitate: %s\n", farmacie.localitate);
	printf("---------------------\n");
}


NodABC* inserareFarmacieABC(NodABC* root, Farmacie farmacie, int* inserat) {
	//verificam daca arborele este gol sau nu
	if (root) {
		//continuam cautarea locului de inserat

		//verificam daca nodul pe care vrem sa il adaugam este mai mare / mai mic sau egal cu valoarea nodului curent
		if (farmacie.id < root->farmacie.id) {
			root->stanga = inserareFarmacieABC(root->stanga, farmacie, inserat);
		}
		if (farmacie.id > root->farmacie.id) {
			root->dreapta = inserareFarmacieABC(root->dreapta, farmacie, inserat);
		}
		if (farmacie.id == root->farmacie.id) {
			*inserat = 0;
		}
	}
	else {
		//creem nod nou care va fi root
		NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
		nodNou->farmacie = farmacie;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		
		root = nodNou;
	}

	return root;
}

void tranversareInordine(NodABC* root) {
	if (root) {
		tranversareInordine(root->stanga);
		printf(" %d ", root->farmacie.id);
		tranversareInordine(root->dreapta);
	}
}

void tranversarePreordine(NodABC* root) {
	if (root) {
		printf(" %d ", root->farmacie.id);
		tranversarePreordine(root->stanga);
		tranversarePreordine(root->dreapta);
	}
}

void tranversarePostordine(NodABC* root) {
	if (root) {
		tranversarePostordine(root->stanga);
		tranversarePostordine(root->dreapta);
		printf(" %d ", root->farmacie.id);
	}
}

//stergerea unui nod si returnare prin parametru element sters pt verificare
NodABC* stergereNodABC(NodABC* root, int id, Farmacie* farmacie) {
	//vrem sa stergem daca avem elemente in abc
	if (root) {
		// phase 1: cautam nodul prin recursivitate
		if (id < root->farmacie.id) {
			//continuam cautarea pe stanga
			stergereNodABC(root->stanga, id,farmacie);
		}
		else if (id > root->farmacie.id) {
			//continuam cautarea pe dreapta
			stergereNodABC(root->dreapta, id,farmacie);
		}
		else {
			// am gasit nodul care trebuie sters = root
			*farmacie = root->farmacie; //salvez datele
			// phase 2: stergere nod
			// avem 4 cazuri : 
			// caz 1 : nodul este frunza
			if (root->stanga == NULL && root->dreapta == NULL) {
				//dezalocam pur si simplu
				free(root);
				root = NULL;
			}
			else {
				//avem restul de 3 cazuri aici
				// caz 2:  nodul are 2 descendenti 
				if (root->dreapta != NULL && root->stanga != NULL) {
					// care dintre copii il va inlocui? 
					// alegem daca vrem sa alegem cel mai mare nod din stanga (cautam pe ramura dreapta mereu) sau
					// cel mai mic nod din dreapta (cautam pe ramura stanga mereu)

					//caut cel mai mare nod din stanga! 
					NodABC* temp;
					NodABC* parinte_temp =NULL;

					temp = root->stanga;
					//parcurg ramurile din dreapta pana dau de ultimul care nu mai are descendent pe dreapta
					while (temp->dreapta) {
						parinte_temp = temp;
						temp = temp->dreapta;
					}
					//sunt la cel mai mare nod din subarborele stang = temp
					//copiez datele din nodul gasit in nodul pe care vreau sa il sterg
					root->farmacie = temp->farmacie;
					if (parinte_temp != NULL) { // daca exista parintele nodului gasit
						parinte_temp->dreapta = temp->stanga;
					}
					else {
						// daca parinte_temp este null inseamna ca cheia maxima din stanga e radacina subarborelui
						// adica copilul din stanga nodului de sters
						root->stanga = temp->stanga;
					}
				}
				else {
					// caz 3:  nodul are un singur descendent in stanga sau in dreapta
					NodABC* temp = root->dreapta; // initializez cu root.dreapta
					if (root->stanga != NULL) {
						//inseamna ca are la stanga si nu la dreapta
						//astfel verific ambele cazuri
						temp = root->stanga;
					}

					//dezaloc root
					free(root);

					//l-am sters deci root devine descendentul sau!
					root = temp;

				}
				
			}
		}
	}
	return root;
}
//cautare farmacie dupa un id
Farmacie cautareNodDupaId(NodABC* root, int id) {
	Farmacie farmacieCautata;
	if (root) {
		//cautam nodul cu id-ul dat
		if (id < root->farmacie.id) {
			cautareNodDupaId(root->stanga, id);
		}
		else if (id > root->farmacie.id) {
			cautareNodDupaId(root->dreapta, id);
		}
		else {
			//nodul a fost gasit = root
			farmacieCautata = root->farmacie;
			return farmacieCautata;
		}
	}
	return farmacieCautata;
	
}
//calculare inaltime
int inaltimeABC(NodABC* root) {
	if (root) {
		int inaltimeS = inaltimeABC(root->stanga);
		int inaltimeD = inaltimeABC(root->dreapta);

		if (inaltimeS > inaltimeD) {
			return 1 + inaltimeS;
		}
		else
			return 1 + inaltimeD;
	}
	else
		return 0;
}
//echilibrare arbore
//numar de noduri frunza
int nrNoduriFrunza(NodABC* root) {
	int nr = 0;
	if (root) {
		
		if (root->stanga == NULL && root->dreapta == NULL) {
			nr++;
		}
		else {
			if (root->stanga != NULL) {
				nr += nrNoduriFrunza(root->stanga);
			}
			if (root->dreapta != NULL) {
				nr += nrNoduriFrunza(root->dreapta);
			}
		}
	}
	return nr;
}
//dezalocare
NodABC* dezalocareABC(NodABC* root) {
	if (root)
	{
		dezalocareABC(root->stanga);
		dezalocareABC(root->dreapta);

		free(root->farmacie.denumire);
		free(root->farmacie.numeSef);
		free(root->farmacie.localitate);
		free(root);
		root = NULL;
	}
	return root;
}

int nrAngajatiLocalitate(NodABC* root, char* localitate) {
	int nr = 0;
	int stanga = 0;
	int dreapta = 0;
	if (root) {
		if (root->stanga != NULL) {
			stanga = nrAngajatiLocalitate(root->stanga, localitate);
		}
		if (root->dreapta != NULL) {
			dreapta = nrAngajatiLocalitate(root->dreapta, localitate);
		}

		if (strcmp(root->farmacie.localitate, localitate) == 0) {
			return root->farmacie.nrAngajati + stanga + dreapta;
		}
		else
			return stanga + dreapta;
	}
	else return 0;
}

float cifraMedieLunaraFarmacie(NodABC* root, int id) {
	float cifra = 0;
	if (root) {
		//cautam nodul corespunzator
		if (id < root->farmacie.id) {
			cifra = cifraMedieLunaraFarmacie(root->stanga, id);
		}
		else if (id > root->farmacie.id) {
			cifra = cifraMedieLunaraFarmacie(root->dreapta, id);
		}
		else {
			//am gasit nodul corespunzator = root
			cifra = root->farmacie.cifraDeAfaceri / 12;
			return cifra;
		}
	}
	return cifra;
}
// stergere frunze de pe un anumit nivel specificat
NodABC* stergeFrunzeLaNivel(NodABC* root, int nivel, int curent) {
	if (!root) return NULL;

	if (curent == nivel) {
		if (!root->stanga && !root->dreapta) {
			// Nodul curent este frunz? ?i trebuie ?ters
			free(root->farmacie.denumire);
			free(root->farmacie.numeSef);
			free(root->farmacie.localitate);
			free(root);
			return NULL;
		}
		return root;
	}

	root->stanga = stergeFrunzeLaNivel(root->stanga, nivel, curent + 1);
	root->dreapta = stergeFrunzeLaNivel(root->dreapta, nivel, curent + 1);
	return root;
}
int main() {
	int nrFarmacii;
	Farmacie* vectorFarmacii;

	readAllFromFile("farmacii.txt", &vectorFarmacii, &nrFarmacii);

	for (int i = 0; i < nrFarmacii; i++) {
		printFarmacie(vectorFarmacii[i]);
	}

	//initializez un ABC
	NodABC* root = NULL;
	int inserat;

	for (int i = 0; i < nrFarmacii; i++) {
		root = inserareFarmacieABC(root, vectorFarmacii[i], &inserat);
		if (!inserat) {
			printf("Nodul %d nu a fost inserat\n", vectorFarmacii[i].id);
			free(vectorFarmacii[i].denumire);
			free(vectorFarmacii[i].numeSef);
			free(vectorFarmacii[i].localitate);
		}
	}

	
	printf("Tranversarea in inordine a ABC:\n");
	tranversareInordine(root);
	printf("\n============\n\n");

	printf("Tranversarea in preordine a ABC:\n");
	tranversarePreordine(root);
	printf("\n============\n\n");

	printf("Tranversarea in postordine a ABC:\n");
	tranversarePostordine(root);
	printf("\n============\n\n");


	/*printf("Sterg nodul 34:\n");
	Farmacie farmacieStearsa;
	root = stergereNodABC(root, 34, &farmacieStearsa);

	printf("Tranversarea in inordine a ABC:\n");
	tranversareInordine(root);
	printf("\n============\n\n");*/

	//printFarmacie(farmacieStearsa);
	//printf("\n============\n\n");

	printf("Caut farmacia cu id ul 10:\n");
	Farmacie farm = cautareNodDupaId(root, 10);
	printFarmacie(farm);
	printf("\n============\n\n");

	int inaltime = inaltimeABC(root);
	printf("Inaltime arbore: %d\n", inaltime);

	int nrFrunze = nrNoduriFrunza(root);
	printf("Nr frunze: %d\n", nrFrunze);

	int nrAng = nrAngajatiLocalitate(root, "Bucuresti");
	printf("Nr ang din buc : %d\n", nrAng);

	float cifra = cifraMedieLunaraFarmacie(root, 12);
	printf("Cifra medie lunara pt farmacia cu id u 10 este : %5.2f\n", cifra);

	root = stergeFrunzeLaNivel(root, 2, 0);
	printf("Tranversarea in inordine a ABC:\n");
	tranversareInordine(root);
	printf("\n============\n\n");

}