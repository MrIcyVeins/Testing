/*
 * ex16_pthreads.c
 *
 * Primeste ca argumente: n c k
 *   n – intervalul [0, n]
 *   c – cifra cautata (0–9)
 *   k – pragul numar de cifre (>= 1)
 *
 * Comportament thread:
 *  - daca i < 10^k (cel mult k cifre): extrage cifra cu cifra si numara c-urile
 *  - daca i >= 10^k (mai mult de k cifre): extrage bucati de k cifre, lansa
 *    cate un thread pentru fiecare bucata, asteapta sa se termine
 *
 * Compilare: gcc -o ex16 ex16_pthreads.c -lpthread
 * Rulare:    ./ex16 <n> <c> <k>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    long long number;
    int       digit;
    int       k;
    long long pow_k;  /* 10^k */
} ThreadArg;

static long long        total_digits  = 0;
static long long        total_threads = 0;
static pthread_mutex_t  mutex         = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void *arg)
{
    ThreadArg *targ  = (ThreadArg *)arg;
    long long  i     = targ->number;
    int        c     = targ->digit;
    long long  pow_k = targ->pow_k;

    /* fiecare thread isi incrementeaza contorul global */
    pthread_mutex_lock(&mutex);
    total_threads++;
    pthread_mutex_unlock(&mutex);

    if (i < pow_k) {
        /* i are cel mult k cifre – extrage cifra cu cifra cu div/mod 10 */
        long long temp  = i;
        int       count = 0;
        do {
            if (temp % 10 == c)
                count++;
            temp /= 10;
        } while (temp != 0);

        pthread_mutex_lock(&mutex);
        total_digits += count;
        pthread_mutex_unlock(&mutex);

    } else {
        /* i are mai mult de k cifre – extrage bucati de k cifre si lanseaza
         * thread-uri pentru fiecare bucata, in paralel */
        long long temp = i;

        /* pasul 1: numara bucatile pentru a sti cate thread-uri cream */
        int num_chunks = 0;
        {
            long long t = temp;
            while (t > 0) {
                num_chunks++;
                t /= pow_k;
            }
        }

        pthread_t *sub_threads = malloc(num_chunks * sizeof(pthread_t));
        ThreadArg *sub_args    = malloc(num_chunks * sizeof(ThreadArg));

        /* pasul 2: extrage bucatile si porneste thread-urile in paralel */
        int idx = 0;
        while (temp > 0) {
            sub_args[idx].number = temp % pow_k;
            sub_args[idx].digit  = targ->digit;
            sub_args[idx].k      = targ->k;
            sub_args[idx].pow_k  = pow_k;
            pthread_create(&sub_threads[idx], NULL, thread_func, &sub_args[idx]);
            idx++;
            temp /= pow_k;
        }

        /* pasul 3: asteapta toate sub-thread-urile */
        for (int j = 0; j < num_chunks; j++)
            pthread_join(sub_threads[j], NULL);

        free(sub_threads);
        free(sub_args);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Utilizare: %s n c k\n", argv[0]);
        return 1;
    }

    long long n = atoll(argv[1]);
    int       c = atoi(argv[2]);
    int       k = atoi(argv[3]);

    if (n < 0 || c < 0 || c > 9 || k < 1) {
        fprintf(stderr, "Argumente invalide: n>=0, 0<=c<=9, k>=1\n");
        return 1;
    }

    /* calculeaza 10^k */
    long long pow_k = 1;
    for (int i = 0; i < k; i++)
        pow_k *= 10;

    /* aloca vectorii pentru thread-urile initiale (cate unul pentru fiecare i
     * din [0, n]) */
    pthread_t *threads = malloc((n + 1) * sizeof(pthread_t));
    ThreadArg *args    = malloc((n + 1) * sizeof(ThreadArg));

    if (!threads || !args) {
        fprintf(stderr, "Eroare la alocarea memoriei\n");
        free(threads);
        free(args);
        return 1;
    }

    /* lanseaza n+1 thread-uri, unul per numar, toate in paralel */
    for (long long i = 0; i <= n; i++) {
        args[i].number = i;
        args[i].digit  = c;
        args[i].k      = k;
        args[i].pow_k  = pow_k;
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    /* asteapta toate thread-urile initiale (care la randul lor asteapta
     * sub-thread-urile proprii) */
    for (long long i = 0; i <= n; i++)
        pthread_join(threads[i], NULL);

    printf("Cifra '%d' apare de %lld ori in numerele din [0, %lld]\n",
           c, total_digits, n);
    printf("Numar total de thread-uri create: %lld\n", total_threads);

    free(threads);
    free(args);
    pthread_mutex_destroy(&mutex);

    return 0;
}
