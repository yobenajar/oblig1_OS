#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* fork */
#include <sys/wait.h> /* waitpid */
#include <sys/types.h> /* pid_t */

// ***********************  FUNKSJONER ***********************


void process(int number, int time) {
     printf("Prosess %d kjører\n", number);
     sleep(time);
     printf("Prosess %d kjørte i %d sekunder\n", number, time);
}

// ************************  MAIN  ***********************

int main (void) {
  pid_t pid1;         // 
  pid_t pid2;       
  pid_t pid3;

  pid1 = fork();          // Lager to nye proseser

  if (pid1 == 0) {        // Hvis child
    process(pid1, 1);     // Kjører funksjon
    exit(0);
    pid2 = fork();        // Lager to nye prosesser

    if (pid2 == 0) {      // Hvis child
      process(1, 2);      // Kjører funksjon
      exit(0);
    }
    else{
      process(4, 3);           // Kjører funksjon
      waitpid(pid2, NULL, 0);  // Venter
    }
  }
  else {
    process(2, 3);             // Kjører funksjon
    waitpid(pid1, NULL, 0);    // Venter
    
    pid3 = fork();             // Lager to nye prosesser

    if (pid3 == 0) {      // Hvis child
      process(3,2);       // Kjører funksjon
      exit(0);
    }
    else {
      process(5,3);               // Kjører funksjon
      waitpid(pid3, NULL, 0);     // Venter
    }
  }

  return 0;
}