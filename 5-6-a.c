#include <stdio.h>		 /* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <pthread.h>
#include <semaphore.h>
// #include <dispatch/dispatch.h>


#define SHARED 0

sem_t sem[6];

// ****************************  STRUCT ***********************


// En datastruktur til å gi info til trådene
struct threadargs {
	int id;         // Trådens ID
	int sec;        // Hvor lenge skal tråden kjøres
	int signal[6];  // Hvem skal tråden gi signal til
                  // når den er ferdig?
};

// ****************************  FUNKSJONER  ***********************

void* tfunc(void *arg) {
	struct threadargs *targs=arg;
  // Vent på lov til å kjøre (tell ned egen semafor)
    sem_wait(&sem[targs->id]);
  // Print at tråden starter
  printf("Hei. Tråd %d har startet\n", targs->id);
  printf("Den skal kjøre i %d sekund\n", targs->sec);

  // Vent noen sekunder
  sleep(targs->sec);
  // Print at tråden er ferdig
  printf("Tråd %d kjørte i %d sekunder\n", targs->id, targs->sec);

  // For i in 0->5:
  for (int i = 0; i <= 5; i++) {
    if (targs->signal[i] == 1) {  // Hvis "jeg" skal signalisere i:
      sem_post(&sem[i]);      // Tell opp semafor til tråd i
    }
  }

  return 0;
}

// ****************************  MAIN  ***********************

int main(void) {
  int i, j;                    // Tellere til løkker
  struct threadargs* targs[6]; // Pekere til argumentene vi gir til trådene.
  pthread_t tid[6];            // Variabler til å holde trådinformasjon

  // Initialiser argumentene vi gir til trådene.
  for(j = 0; j < 6; j++) {
    targs[j] = malloc(sizeof(struct threadargs)); // Sett av minne, og
    targs[j]->id = 0;                             // sett alle variablene
    targs[j]->sec = 0;                            // til 0.
    for(i = 0; i < 6; i++) {
      targs[j]->signal[i] = 0;
  	}
  }

  // START T0
  // Sett opp data til T0
  targs[0]->id = 0;
  targs[0]->sec = 1;
  targs[0]->signal[1] = 1;
  targs[0]->signal[4] = 1;
  // Initialiser semafor til T0
  sem_init(&sem[0], SHARED, 1);
  // Start T0
  pthread_create(&tid[0], NULL, tfunc, targs[0]);

  // START T1
  // Sett opp data til T1
  targs[1]->id = 1;
  targs[1]->sec = 2;
  targs[1]->signal[3] = 1;
  // Initialiser semafor til T1
 // int ret;
  sem_init(&sem[1], SHARED, 0);
  // Start T1
  pthread_create(&tid[1], NULL, tfunc, targs[1]);

  // START T2
  // Sett opp data til T2
  targs[2]->id = 2;
  targs[2]->sec = 3;
  // Initialiser semafor til T2
  sem_init(&sem[2], SHARED, 1);
  // Start T2
  pthread_create(&tid[2], NULL, tfunc, targs[2]);

  // START T3
  // Sett opp data til T3
  targs[3]->id = 3;
  targs[3]->sec = 2;
  // Initialiser semafor til T3
  sem_init(&sem[3], SHARED, 0);
  // Start T3
  pthread_create(&tid[3], NULL, tfunc, targs[3]);

  // START T4
  // Sett opp data til T4
  targs[4]->id = 4;
  targs[4]->sec = 3;
  targs[4]->signal[5] = 1;
  // Initialiser semafor til T4
  sem_init(&sem[4], SHARED, 0);
  // Start T4
  pthread_create(&tid[4], NULL, tfunc, targs[4]);

  // START T5
  // Sett opp data til T5
  targs[5]->id = 5;
  targs[5]->sec = 3;
  // Initialiser semafor til T5
  sem_init(&sem[5], SHARED, 0);
  // Start T5
  pthread_create(&tid[5], NULL, tfunc, targs[5]);

  // Vent på alle tråder
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  pthread_join(tid[3], NULL);
  pthread_join(tid[4], NULL);
  pthread_join(tid[5], NULL);


	return 0;
}
