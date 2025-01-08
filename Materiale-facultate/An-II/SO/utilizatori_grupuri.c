#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

// Implementarea funcției custom getpwnam
struct passwd *my_getpwnam(const char *username) {
    struct passwd *pwd;

    // Deschide fișierul /etc/passwd pentru citire
    setpwent();

    // Iterează prin fiecare înregistrare din /etc/passwd
    while ((pwd = getpwent()) != NULL) {
        // Compară numele de utilizator curent cu cel dorit
        if (strcmp(pwd->pw_name, username) == 0) {
            // Închide fluxul și returnează înregistrarea găsită
            endpwent();
            return pwd;
        }
    }

    // Închide fluxul dacă utilizatorul nu a fost găsit
    endpwent();
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s <username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *username = argv[1];
    struct passwd *user_info = my_getpwnam(username);

    if (user_info == NULL) {
        fprintf(stderr, "Utilizatorul '%s' nu a fost găsit.\n", username);
        return EXIT_FAILURE;
    }

    // Afișează informația 'gecos' asociată utilizatorului
    printf("Informația GECOS pentru utilizatorul '%s': %s\n", username, user_info->pw_gecos);

    return EXIT_SUCCESS;
}