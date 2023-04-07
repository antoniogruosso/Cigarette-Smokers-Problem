#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define FUMATORI 3

sem_t sTabacco, sCartine, sFiammiferi;
sem_t sAgente;

int Tabacco = 0;
int Cartine = 0;
int Fiammiferi = 0;

void *fumatore(void * arg) {
	int id = *(int*)arg;

	while(true) {
		switch(id) {
      
        	case 0:
        		sem_wait(&sTabacco);
        		if(Cartine > 0 && Fiammiferi > 0) {
          			Cartine = 0;
          			Fiammiferi = 0;
          			printf("-->%d: Il fumatore(Tabacco) HA raccolto le cartine ed i fiammiferi\n", id);
          			sem_post(&sAgente);
        	    }
        		else printf("%d: ###ERRORE: Le cartine o i fiammiferi non sono stati disposti sul tavolo!\n", id);
        		break;

      		case 1:
        		sem_wait(&sCartine);
        		if(Tabacco > 0 && Fiammiferi > 0) {
          			Tabacco = 0;
          			Fiammiferi = 0;
          			printf("-->%d: Il fumatore(Cartine) HA raccolto il tabacco ed i fiammiferi\n", id);
          			sem_post(&sAgente);
        		}
        		else printf("%d: ###ERRORE: Il tabacco o i fiammiferi non sono stati disposti sul tavolo!\n", id);
        		break;

      		case 2:
        		sem_wait(&sFiammiferi);
        		if(Tabacco > 0 && Cartine > 0) {
          			Tabacco = 0;
          			Cartine = 0;
          			printf("<-->%d: Il fumatore(Fiammiferi) HA raccolto le cartine ed il tabacco\n", id);
          			sem_post(&sAgente);
        			}
        		else printf("%d: ###ERRORE: Il tabacco o le cartine non sono stati disposti sul tavolo!\n", id);
        		break;

      		default:
        		printf("\n###ERRORE: L'ID dell'argomento scelto NON e' valido: %d\n", id);
        		exit(1);
        		break;
    	}
  	}
}

void *agente(void * arg) {
	while(true) {
    	sem_wait(&sAgente);
		usleep(1000000);
		int id = rand() % 3;
		switch(id) {
      
	  		case 0:
        		puts("\n>--< L'agente posa sul tavolo le cartine ed i fiammiferi");
        		Cartine++;
        		Fiammiferi++;
        		sem_post(&sTabacco);
        		break;

      		case 1:
        		puts("\n>--< L'agente posa sul tavolo il tabacco ed i fiammiferi");
        		Tabacco++;
        		Fiammiferi++;
        		sem_post(&sCartine);
       			break;

      		case 2:
         		puts("\n>--< L'agente posa sul tavolo il tabacco e le cartine");
        		Tabacco++;
        		Cartine++;
        		sem_post(&sFiammiferi);
        		break;

      		default:
        	printf("\n###ERRORE: L'ID dell'agente NON e' valido: %d\n", id);
        	break;
    	}
  	}
}

void schermoMenuInziale() {
  	printf("\n================================================");
  	printf("\n==                                            ==");
  	printf("\n==        Il problema dei fumatori            ==");
  	printf("\n==                                            ==");
  	printf("\n==                         (  )/              ==");
  	printf("\n==                          )(/               ==");
  	printf("\n==       ________________  ( /)               ==");
  	printf("\n==      ()__)____________)))))                ==");
  	printf("\n==                                            ==");
  	printf("\n================================================\n");
}

int main(int argc, char* argv[]) {
	schermoMenuInziale();
  	
  	sem_init(&sTabacco, 0, 0);
  	sem_init(&sCartine, 0, 0);
  	sem_init(&sFiammiferi, 0, 0);
	sem_init(&sAgente, 0, 1);
	pthread_t tFumatori[FUMATORI], tAgente;
	int idFumatori[FUMATORI];
	for(int i = 0; i < FUMATORI; i++) {
    	idFumatori[i] = i;
    	pthread_create(&tFumatori[i], NULL, fumatore, &idFumatori[i]);
  	}
  	puts("***Creazione thread fumatori***");
	pthread_create(&tAgente, NULL, agente, NULL);
  	puts("***Creazione thread agente***");
	for (int i = 0; i < FUMATORI; i++) {
    	pthread_join(tFumatori[i], NULL);
  	}
	pthread_join(tAgente, NULL);
	exit(0);
}
