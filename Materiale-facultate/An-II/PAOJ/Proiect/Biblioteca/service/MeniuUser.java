
package service;

import model.Carte;
import model.Cititor;

import java.util.*;
import java.util.stream.Collectors;

public class MeniuUser {
    private static List<Carte> favorite = new ArrayList<>();

    public static void run(UserService userService, BibliotecaService biblioteca, AuditService audit, Scanner scanner) {
        while (true) {
            System.out.println("\n>>> MENIU UTILIZATOR <<<");
            System.out.println("1. Afișează toate cărțile");
            System.out.println("2. Caută carte (titlu parțial)");
            System.out.println("3. Caută carte după gen");
            System.out.println("4. Caută carte după autor");
            System.out.println("5. Adaugă carte la favorite");
            System.out.println("6. Afișează cărți favorite");
            System.out.println("7. Resetează parola proprie");
            System.out.println("8. Sortează cărțile (alfabetic)");
            System.out.println("0. Logout");
            System.out.print("Alege opțiunea: ");

            int opt;
            try {
                opt = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Opțiune invalidă.");
                continue;
            }

            switch (opt) {
                case 1:
                    biblioteca.afiseazaCarti();
                    audit.logActiune("user_afiseaza_carti");
                    break;

                case 2:
                    System.out.print("Cuvânt cheie în titlu: ");
                    String partial = scanner.nextLine();
                    biblioteca.cautaCartePartialTitlu(partial);
                    audit.logActiune("user_cauta_partial_titlu");
                    break;

                case 3:
                    System.out.print("Genul dorit: ");
                    String gen = scanner.nextLine();
                    biblioteca.cautaCartiDupaGen(gen);
                    audit.logActiune("user_cauta_dupa_gen");
                    break;

                case 4:
                    System.out.print("Nume autor: ");
                    String autor = scanner.nextLine();
                    biblioteca.cautaCartiDupaAutor(autor);
                    audit.logActiune("user_cauta_dupa_autor");
                    break;

                case 5:
                    System.out.print("Titlu exact carte de adăugat la favorite: ");
                    String titluFav = scanner.nextLine();
                    Carte carteFav = biblioteca.getCarteByTitlu(titluFav);
                    if (carteFav != null) {
                        favorite.add(carteFav);
                        System.out.println("✅ Adăugată la favorite.");
                    } else {
                        System.out.println("❌ Carte negăsită.");
                    }
                    audit.logActiune("user_adauga_favorite");
                    break;

                case 6:
                    System.out.println(">>> Cărți favorite <<<");
                    for (Carte c : favorite) {
                        System.out.println(c);
                    }
                    audit.logActiune("user_afiseaza_favorite");
                    break;

                case 7:
                    System.out.print("Noua parolă: ");
                    String nouaParola = scanner.nextLine();
                    Cititor user = userService.getCititorAutentificat();
                    if (user != null) {
                        userService.schimbaParola(user.getEmail(), nouaParola);
                        System.out.println("✅ Parolă actualizată.");
                    }
                    audit.logActiune("user_reseteaza_parola");
                    break;

                case 8:
                    List<Carte> toate = biblioteca.getCarti();
                    List<Carte> sortate = toate.stream()
                            .sorted(Comparator.comparing(Carte::getTitlu, String.CASE_INSENSITIVE_ORDER))
                            .collect(Collectors.toList());
                    System.out.println(">>> Cărți sortate alfabetic <<<");
                    for (Carte c : sortate) {
                        System.out.println(c);
                    }
                    audit.logActiune("user_sorteaza_carti");
                    break;

                case 0:
                    System.out.println("Logout...");
                    return;

                default:
                    System.out.println("Opțiune invalidă.");
            }
        }
    }
}
