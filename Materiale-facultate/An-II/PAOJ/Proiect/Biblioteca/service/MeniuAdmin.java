
package service;

import model.Cititor;
import model.Autor;
import model.Carte;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class MeniuAdmin {
    public static void run(UserService userService, BibliotecaService biblioteca, AuditService audit, Scanner scanner) {
        while (true) {
            System.out.println("\n>>> MENIU ADMIN <<<");
            System.out.println("1. Afișează toate cărțile");
            System.out.println("2. Afișează autori");
            System.out.println("3. Importă date dummy din fișier");
            System.out.println("4. Șterge utilizator");
            System.out.println("5. Schimbă parolă");
            System.out.println("6. Statistici simple");
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
                    audit.logActiune("admin_afiseaza_carti");
                    break;

                case 2:
                    biblioteca.afiseazaAutori();
                    audit.logActiune("admin_afiseaza_autori");
                    break;

                case 3:
                    System.out.print("Numele fișierului CSV (ex: dummy_data.csv): ");
                    String numeFisier = scanner.nextLine();
                    importaDateDummy(numeFisier, biblioteca);
                    audit.logActiune("admin_importa_date_dummy");
                    break;

                case 4:
                    System.out.print("Emailul utilizatorului de șters: ");
                    String email = scanner.nextLine();
                    userService.stergeUtilizator(email);
                    audit.logActiune("admin_sterge_utilizator");
                    break;

                case 5:
                    System.out.print("Emailul utilizatorului pentru schimbarea parolei: ");
                    String emailParola = scanner.nextLine();
                    System.out.print("Noua parolă: ");
                    String parolaNoua = scanner.nextLine();
                    userService.schimbaParola(emailParola, parolaNoua);
                    audit.logActiune("admin_schimba_parola");
                    break;

                case 6:
                    biblioteca.afiseazaStatistici();
                    audit.logActiune("admin_statistici");
                    break;

                case 0:
                    System.out.println("Logout...");
                    return;

                default:
                    System.out.println("Opțiune invalidă.");
            }
        }
    }

    private static void importaDateDummy(String fisier, BibliotecaService biblioteca) {
        try (BufferedReader br = new BufferedReader(new FileReader(fisier))) {
            String linie;
            while ((linie = br.readLine()) != null) {
                String[] parts = linie.split(",");
                if (parts.length == 5) {
                    String titlu = parts[0];
                    String autorNume = parts[1];
                    String autorBio = parts[2];
                    String gen = parts[3];
                    int an = Integer.parseInt(parts[4]);

                    Autor autor = new Autor(autorNume, autorBio);
                    Carte carte = new Carte(titlu, autor, gen, an);
                    biblioteca.adaugaCarte(carte, "Import");
                }
            }
            System.out.println("Datele au fost importate cu succes.");
        } catch (IOException e) {
            System.out.println("Eroare la citirea fișierului: " + e.getMessage());
        }
    }
}
