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
                    break;
                case 2:
                    System.out.print("Titlu carte: ");
                    biblioteca.cautaCartePartialTitlu(scanner.nextLine());
                    break;
                case 3:
                    System.out.print("Genul dorit: ");
                    biblioteca.cautaCartiDupaGen(scanner.nextLine());
                    break;
                case 4:
                    System.out.print("Nume autor: ");
                    biblioteca.cautaCartiDupaAutor(scanner.nextLine());
                    break;
                case 5:
                    System.out.print("Titlu carte: ");
                    String titluFav = scanner.nextLine();
                    Carte fav = biblioteca.getCarteByTitlu(titluFav);
                    if (fav != null) {
                        favoriteService.adaugaFavorite(user, fav);
                    } else {
                        System.out.println("❌ Carte negasita.");
                    }
                    break;
                case 6:
                    favoriteService.afiseazaFavorite(user);
                    break;
                case 7:
                    System.out.print("Titlu carte de sters din favorite: ");
                    favoriteService.stergeFavorite(user, scanner.nextLine());
                    break;
                case 8:
                    System.out.print("Parola noua: ");
                    userService.schimbaParola(user.getEmail(), scanner.nextLine());
                    System.out.println("Parola a fost actualizata.");
                    break;
                case 9:
                    System.out.print("Titlu carte de imprumutat: ");
                    imprumutService.imprumutaCarte(user, scanner.nextLine());
                    break;
                case 10:
                    System.out.print("Titlu carte de returnat: ");
                    imprumutService.returneazaCarte(user, scanner.nextLine());
                    break;
                case 11:
                    imprumutService.veziImprumuturi(user);
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
