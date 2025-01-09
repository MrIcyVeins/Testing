// 19. Scrieţi un program care genereaza un proces copil iar acesta trimite
// părintelui un număr aleator de semnale SIGUSR1; copilul număra câte a trimis,
// părintele numără câte a primit, apoi fiecare afişază numarul respectiv. Părintele
// se va termina la primirea SIGCHLD şi va culege codul de retur al copilului
// (’wait()’ sau ’waitpid()’). Se va asigura protecţia la pierderea unor semnale.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

volatile sig_atomic_t semnale_primite = 0; // Contor pentru semnalele SIGUSR1 primite de părinte
volatile sig_atomic_t copil_terminat = 0; // Indicator pentru terminarea copilului

// Funcție pentru gestionarea semnalelor SIGUSR1
void gestioneaza_SIGUSR1(int semnal) {
    semnale_primite++;
    printf("Părintele a primit semnalul SIGUSR1 (Total: %d).\n", semnale_primite);
}

// Funcție pentru gestionarea semnalului SIGCHLD
void gestioneaza_SIGCHLD(int semnal) {
    copil_terminat = 1;
    printf("Părintele a primit SIGCHLD - copilul s-a terminat.\n");
}

int main() {
    pid_t pid;

    // Măsurăm timpul de execuție
    clock_t inceput = clock();

    // Setăm handler-ele pentru semnale
    struct sigaction act_SIGUSR1, act_SIGCHLD;

    act_SIGUSR1.sa_handler = gestioneaza_SIGUSR1;
    sigemptyset(&act_SIGUSR1.sa_mask);
    act_SIGUSR1.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &act_SIGUSR1, NULL);

    act_SIGCHLD.sa_handler = gestioneaza_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);

    pid = fork();

    if (pid < 0) {
        perror("Eroare la fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Cod pentru procesul copil
        srand(time(NULL)); // Inițializează generatorul de numere aleatoare
        int semnale_trimise = 0;
        int numar_semnale = rand() % 200 + 1; // Generează un număr aleator între 1 și 100

        printf("Copilul va trimite %d semnale SIGUSR1.\n", numar_semnale);

        for (int i = 0; i < numar_semnale; i++) {
            kill(getppid(), SIGUSR1); // Trimite semnalul SIGUSR1 către părinte
            semnale_trimise++;
            usleep(10000); // Pauză mică pentru a evita suprasolicitarea
        }

        printf("Copilul a trimis %d semnale SIGUSR1.\n", semnale_trimise);
        exit(0); // Returnează 0 pentru succes
    } else {
        // Cod pentru procesul părinte
        while (!copil_terminat) {
            pause(); // Așteaptă semnale (SIGUSR1 sau SIGCHLD)
        }

        // Afișează numărul de semnale primite
        printf("Părintele a primit %d semnale SIGUSR1.\n", semnale_primite);

        // Așteaptă copilul și culege codul de retur
        int status;
        if (waitpid(pid, &status, 0) > 0) {
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    printf("Copilul s-a terminat cu succes (cod de retur 0).\n");
                } else {
                    printf("Copilul s-a terminat cu un cod de retur neașteptat: %d.\n", WEXITSTATUS(status));
                }
            }
        } else {
            perror("Eroare la waitpid");
        }

        printf("Părintele se va termina după o pauză de 10 secunde...\n");
        sleep(10); // Pauză de 10 secunde

        // Timpul de execuție
        clock_t sfarsit = clock();
        printf("Timp de execuție: %lf secunde.\n", (double)(sfarsit - inceput) / CLOCKS_PER_SEC);

        return EXIT_SUCCESS;
    }
}



