
package service;

import model.Carte;
import model.User;

import java.util.*;

public class MeniuUser {
    private static final FavoriteService favoriteService = new FavoriteService();
    private static final ImprumutService imprumutService = new ImprumutService();

    public static void run(UserService userService, BibliotecaService biblioteca, AuditService audit, Scanner scanner) {
        User user = userService.getUserAutentificat();

        while (true) {
            System.out.println("\n>>> MENIU UTILIZATOR <<<");
            System.out.println("1. Afiseaza toate cartile");
            System.out.println("2. Cauta carte - dupa titlu");
            System.out.println("3. Cauta carte - dupa gen");
            System.out.println("4. Cauta carte - dupa autor");
            System.out.println("5. Adauga carte la favorite");
            System.out.println("6. Afiseaza carti favorite");
            System.out.println("7. Sterge carte din favorite");
            System.out.println("8. Reseteaza parola");
            System.out.println("9. Imprumuta carte");
            System.out.println("10. Returneaza carte");
            System.out.println("11. Vezi imprumuturi");
            System.out.println("0. Logout");
            System.out.print("Alege optiunea: ");

            int opt;
            try {
                opt = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Optiune invalida.");
                continue;
            }

            switch (opt) {
                case 1:
                    biblioteca.afiseazaCarti();
                    audit.logActiune("user_afiseaza_carti", user);
                    break;
                case 2:
                    System.out.print("Titlu carte: ");
                    biblioteca.cautaCartePartialTitlu(scanner.nextLine());
                    audit.logActiune("user_cauta_partial_titlu", user);
                    break;
                case 3:
                    System.out.print("Genul dorit: ");
                    biblioteca.cautaCartiDupaGen(scanner.nextLine());
                    audit.logActiune("user_cauta_dupa_gen", user);
                    break;
                case 4:
                    System.out.print("Nume autor: ");
                    biblioteca.cautaCartiDupaAutor(scanner.nextLine());
                    audit.logActiune("user_cauta_dupa_autor", user);
                    break;
                case 5:
                    System.out.print("Titlu carte: ");
                    Carte favAdd = biblioteca.getCarteByTitlu(scanner.nextLine());
                    if (favAdd != null) {
                        favoriteService.adaugaFavorite(user, favAdd);
                    } else {
                        System.out.println("❌ Carte negasita.");
                    }
                    audit.logActiune("user_adauga_favorite", user);
                    break;
                case 6:
                    favoriteService.afiseazaFavorite(user);
                    audit.logActiune("user_afiseaza_favorite", user);
                    break;
                case 7:
                    System.out.print("Titlu carte de eliminat din favorite: ");
                    favoriteService.stergeFavorite(user, scanner.nextLine());
                    audit.logActiune("user_sterge_favorite", user);
                    break;
                case 8:
                    System.out.print("Parola noua: ");
                    userService.schimbaParola(user.getEmail(), scanner.nextLine());
                    System.out.println("✅ Parola a fost actualizata.");
                    audit.logActiune("user_reseteaza_parola", user);
                    break;
                case 9:
                    System.out.print("Titlu carte de imprumutat: ");
                    imprumutService.imprumutaCarte(user, scanner.nextLine());
                    audit.logActiune("user_imprumuta_carte", user);
                    break;
                case 10:
                    System.out.print("Titlu carte de returnat: ");
                    imprumutService.returneazaCarte(user, scanner.nextLine());
                    audit.logActiune("user_returneaza_carte", user);
                    break;
                case 11:
                    imprumutService.veziImprumuturi(user);
                    audit.logActiune("user_vezi_imprumuturi", user);
                    break;
                case 0:
                    System.out.println("Logout...");
                    return;
                default:
                    System.out.println("Optiune invalida.");
            }
        }
    }
}
