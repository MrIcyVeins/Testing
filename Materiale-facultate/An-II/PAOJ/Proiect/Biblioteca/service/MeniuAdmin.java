
package service;

import model.Autor;
import model.Carte;
import model.User;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.*;
import java.util.Scanner;

public class MeniuAdmin {
    private static final User admin = new User("admin", "admin@admin.com", "");

    public static void run(UserService userService, BibliotecaService biblioteca, AuditService audit, Scanner scanner) {
        while (true) {
            System.out.println("\n>>> MENIU ADMIN <<<");
            System.out.println("1. Afiseaza toate cartile");
            System.out.println("2. Afiseaza autori");
            System.out.println("3. Importa date din fisier csv");
            System.out.println("4. Adauga carte noua");
            System.out.println("5. Listeaza utilizatori");
            System.out.println("6. Statistici");
            System.out.println("7. Sterge carte dupa titlu");
            System.out.println("8. Schimba parola unui utilizator");
            System.out.println("9. Schimba parola proprie");
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
                    audit.logActiune("admin_afiseaza_carti", admin);
                    break;
                case 2:
                    biblioteca.afiseazaAutori();
                    audit.logActiune("admin_afiseaza_autori", admin);
                    break;
                case 3:
                    System.out.print("Numele fisierului CSV (ex: carti.csv): ");
                    String numeFisier = scanner.nextLine();
                    importaDateUnice(numeFisier, biblioteca);
                    audit.logActiune("admin_importa_date_unice", admin);
                    break;
                case 4:
                    adaugaCarteNoua(scanner, biblioteca);
                    audit.logActiune("admin_adauga_carte", admin);
                    break;
                case 5:
                    userService.afiseazaTotiUserii();
                    audit.logActiune("admin_afiseaza_utilizatori", admin);
                    break;
                case 6:
                    biblioteca.afiseazaStatistici();
                    audit.logActiune("admin_statistici", admin);
                    break;
                case 7:
                    System.out.print("Titlul cartii de sters: ");
                    String titlu = scanner.nextLine();
                    biblioteca.stergeCarteDinDB(titlu);
                    audit.logActiune("admin_sterge_carte", admin);
                    break;
                case 8:
                    System.out.print("Email utilizator: ");
                    String email = scanner.nextLine();
                    System.out.print("Noua parola: ");
                    String parola = scanner.nextLine();
                    userService.schimbaParola(email, parola);
                    audit.logActiune("admin_schimba_parola_user", admin);
                    break;
                case 9:
                    System.out.print("Noua parola admin: ");
                    String parolaNoua = scanner.nextLine();
                    try {
                        PreparedStatement stmt = DBConnection.getConnection().prepareStatement("UPDATE admin SET parola = ? WHERE email = ?");
                        stmt.setString(1, parolaNoua);
                        stmt.setString(2, "admin@admin.com");
                        stmt.executeUpdate();
                        System.out.println("Parola admin schimbata.");
                        audit.logActiune("admin_schimba_parola_proprie", admin);
                    } catch (SQLException e) {
                        System.out.println("Eroare la schimbarea parolei admin: " + e.getMessage());
                    }
                    break;
                case 0:
                    System.out.println("Logout...");
                    return;
                default:
                    System.out.println("Optiune invalida.");
            }
        }
    }

    private static void importaDateUnice(String fisier, BibliotecaService biblioteca) {
        try (BufferedReader br = new BufferedReader(new FileReader(fisier))) {
            String linie;
            int total = 0, adaugate = 0;
            boolean prima = true;

            while ((linie = br.readLine()) != null) {
                if (prima) {
                    prima = false;
                    continue;
                }

                total++;
                String[] parts = linie.split(",");
                if (parts.length != 5) continue;

                String titlu = parts[0].trim();
                String autorNume = parts[1].trim();
                String autorBio = parts[2].trim();
                String gen = parts[3].trim();
                int an = Integer.parseInt(parts[4].trim());

                if (!existaCarte(titlu, autorNume)) {
                    Autor autor = new Autor(autorNume, autorBio);
                    Carte carte = new Carte(titlu, autor, gen, an);
                    biblioteca.adaugaCarte(carte, "Import");
                    adaugate++;
                }
            }

            System.out.println("Import finalizat. " + adaugate + " carti adaugate.");

        } catch (IOException | NumberFormatException e) {
            System.out.println("Eroare la import: " + e.getMessage());
        }
    }

    private static boolean existaCarte(String titlu, String autorNume) {
        String sql = "SELECT COUNT(*) FROM carte " +
                     "JOIN autor ON carte.autor_id = autor.id " +
                     "WHERE LOWER(carte.titlu) = ? AND LOWER(autor.nume) = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, titlu.toLowerCase());
            stmt.setString(2, autorNume.toLowerCase());
            ResultSet rs = stmt.executeQuery();
            return rs.next() && rs.getInt(1) > 0;
        } catch (SQLException e) {
            System.out.println("Eroare la verificare duplicat: " + e.getMessage());
            return false;
        }
    }

    private static void adaugaCarteNoua(Scanner scanner, BibliotecaService biblioteca) {
        System.out.print("Titlu: ");
        String titlu = scanner.nextLine();
        System.out.print("Autor: ");
        String autorNume = scanner.nextLine();
        System.out.print("Biografie autor: ");
        String autorBio = scanner.nextLine();
        System.out.print("Gen: ");
        String gen = scanner.nextLine();
        System.out.print("An publicare: ");
        int an;
        try {
            an = Integer.parseInt(scanner.nextLine());
        } catch (NumberFormatException e) {
            System.out.println("An invalid.");
            return;
        }

        if (existaCarte(titlu, autorNume)) {
            System.out.println("Cartea exista deja.");
            return;
        }

        Autor autor = new Autor(autorNume, autorBio);
        Carte carte = new Carte(titlu, autor, gen, an);
        biblioteca.adaugaCarte(carte, "Manual");
        System.out.println("Carte adaugata cu succes.");
    }
}
