#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

#define OBRADA_DATOTEKA "obrada.txt"
#define STATUS_DATOTEKA "status.txt"

int broj = 1;

// Funkcija za čitanje broja iz status.txt
int procitaj_status() {
	FILE *f = fopen(STATUS_DATOTEKA, "r");
	if (f == NULL) {
		perror("Greška pri otvaranju datoteke status.txt");
		exit(EXIT_FAILURE);
	}
	int status;
	fscanf(f, "%d", &status);
	fclose(f);
	return status;
}

// Funckija za zapisivanje broja u status.txt
void zapisi_status(int vrijednost) {
	FILE *f = fopen(STATUS_DATOTEKA, "w");
	if (f == NULL) {
		perror("Greška pri otvaranju datoteke status.txt");
		exit(EXIT_FAILURE);
	}
	fprintf(f, "%d\n", vrijednost);
	fclose(f);
	
}

// Funkcija za čitanje zadnje broja iz obrade.txt 
int procitaj_zadnji_broj_iz_obrade() {
	FILE *f = fopen(OBRADA_DATOTEKA, "r");
	if (f == NULL) {
		perror("Greška pri otvaranju datoteke obrada.txt");
		exit(EXIT_FAILURE);
	}
	int zadnji_broj = 0, broj;
	while (fscanf(f, "%d", &broj) != EOF) {
		zadnji_broj = broj;
	}
	fclose(f);
	return (int)sqrt(zadnji_broj);
}

// Funkcija za dodavanje novog broja u obrada.txt
void dodaj_u_obradu(int vrijednost) {
	FILE *f = fopen(OBRADA_DATOTEKA, "a");
	if (f == NULL) {
		perror("Greška pri otvaranju datoteke obrada.txt");
		exit(EXIT_FAILURE);
	}
	fprintf(f, "%d\n", vrijednost * vrijednost);
	fclose(f);
}

// Handler za SIGUSR1
void handle_sigusr1(int signum) {
	printf("Trenutni broj: %d\n", broj);
}

// Handler za SIGTERM
void handle_sigterm(int signum) {
	printf("Primljen SIGTERM, spremanje statusa i izlazak...\n");
	zapisi_status(broj);
	exit(0);
}

// Handler za SIGINT
void handle_sigint(int signum) {
	printf("Primljen SIGINT, izlazak...\n");
	exit(0);
}

int main(){
	// Privremeno blokiranje signala
	signal(SIGUSR1, handle_sigusr1);
	signal(SIGTERM, handle_sigterm);
	signal(SIGINT, handle_sigint);
	
	broj = procitaj_status();
	
	if (broj == 0) {
		broj = procitaj_zadnji_broj_iz_obrade();
	}
	
	zapisi_status(0);
	
	while (1) {
		broj++;
		dodaj_u_obradu(broj);
		sleep(5);
	}
	
	return 0;
}
