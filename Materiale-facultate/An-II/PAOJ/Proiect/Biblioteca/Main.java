import model.*;
import service.*;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        BibliotecaService biblioteca = new BibliotecaService();
        UserService userService = new UserService();
        AuditService audit = new AuditService();

        // Încarcă date persistente
        userService.incarcaCititoriDinDB();
        biblioteca.incarcaCartiDinDB(); // doar dacă ai implementat această funcție

        while (true) {
            System.out.println("\n=== MENIU BIBLIOTECĂ ===");
            System.out.println("1. Înregistrare cititor");
            System.out.println("2. Autentificare cititor");
            System.out.println("3. Adaugă carte");
            System.out.println("4. Afișează toate cărțile");
            System.out.println("5. Caută carte după titlu");
            System.out.println("6. Șterge carte după titlu");
            System.out.println("7. Afișează autori");
            System.out.println("0. Ieșire");
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
                    System.out.print("Nume: ");
                    String nume = scanner.nextLine();
                    System.out.print("Email: ");
                    String email = scanner.nextLine();
                    System.out.print("Parolă: ");
                    String parola = scanner.nextLine();
                    userService.inregistreazaCititor(new Cititor(nume, email, parola));
                    audit.logActiune("inregistrare_cititor");
                    break;

                case 2:
                    System.out.print("Email: ");
                    String e = scanner.nextLine();
                    System.out.print("Parolă: ");
                    String p = scanner.nextLine();
                    if (userService.autentifica(e, p)) {
                        audit.logActiune("autentificare_cititor");
                    }
                    break;

                case 3:
                    if (userService.getCititorAutentificat() == null) {
                        System.out.println("Trebuie să fii autentificat.");
                        break;
                    }
                    System.out.print("Titlu carte: ");
                    String titlu = scanner.nextLine();
                    System.out.print("Autor: ");
                    String numeAutor = scanner.nextLine();
                    System.out.print("Biografie autor: ");
                    String bio = scanner.nextLine();
                    System.out.print("Gen: ");
                    String gen = scanner.nextLine();
                    System.out.print("An publicare: ");
                    int an;
                    try {
                        an = Integer.parseInt(scanner.nextLine());
                    } catch (NumberFormatException ex) {
                        System.out.println("An invalid.");
                        break;
                    }
                    System.out.print("Secțiune: ");
                    String sectiune = scanner.nextLine();

                    Autor autor = new Autor(numeAutor, bio);
                    Carte carte = new Carte(titlu, autor, gen, an);
                    biblioteca.adaugaCarte(carte, sectiune);
                    audit.logActiune("adauga_carte");
                    break;

                case 4:
                    biblioteca.afiseazaCarti();
                    audit.logActiune("afiseaza_carti");
                    break;

                case 5:
                    System.out.print("Titlul cărții căutate: ");
                    String cautare = scanner.nextLine();
                    biblioteca.cautaCarteDupaTitlu(cautare);
                    audit.logActiune("cauta_carte");
                    break;

                case 6:
                    System.out.print("Titlul cărții de șters: ");
                    String deSters = scanner.nextLine();
                    biblioteca.stergeCarte(deSters);
                    audit.logActiune("sterge_carte");
                    break;

                case 7:
                    biblioteca.afiseazaAutori();
                    audit.logActiune("afiseaza_autori");
                    break;

                case 0:
                    System.out.println("La revedere!");
                    return;

                default:
                    System.out.println("Opțiune invalidă.");
            }
        }
    }
}
