import model.*;
import service.*;

import java.io.Console; // pentru citirea parolelor fara sa fie afisate pe ecran
import java.sql.*;
import java.util.Arrays; // pentru stergerea parolei din memorie (Arrays.fill)
import java.util.Scanner; // pentru inserarea datelor din consola

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        BibliotecaService biblioteca = new BibliotecaService();
        UserService userService = new UserService();
        AuditService audit = new AuditService();
        Console console = System.console();

        while (true) {
            System.out.println("\n>>> BUN VENIT LA BIBLIOTECA <<<");
            System.out.println("1. Login");
            System.out.println("2. Register");
            System.out.println("0. Iesire");
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
                    System.out.print("Email: ");
                    String email = scanner.nextLine();
                    String parola;

                    if (console != null) {
                        char[] parolaChars = console.readPassword("Parola: ");
                        parola = new String(parolaChars);
                        Arrays.fill(parolaChars, '\0');
                    } else {
                        System.out.print("Parola: ");
                        parola = scanner.nextLine();
                    }

                    User user = userService.autentificare(email, parola);
                    if (user != null) {
                        audit.logActiune("autentificare", user);
                        System.out.println("Bun venit, " + user.getNume() + "!");
                        if (user.getRol().equals("admin")) {
                            MeniuAdmin.run(userService, biblioteca, audit, scanner);
                        } else {
                            MeniuUser.run(userService, biblioteca, audit, scanner);
                        }
                    } else {
                        System.out.println("Email sau parola incorecte.");
                    }
                    break;

                case 2:
                    System.out.print("Nume: ");
                    String nume = scanner.nextLine();
                    System.out.print("Email: ");
                    String newEmail = scanner.nextLine();
                    String newParola;
                    if (console != null) {
                        char[] parolaChars = console.readPassword("Parola: ");
                        newParola = new String(parolaChars);
                        Arrays.fill(parolaChars, '\0');
                    } else {
                        System.out.print("Parola: ");
                        newParola = scanner.nextLine();
                    }
                    User userNou = new User(nume, newEmail, newParola, "user");
                    userService.inregistreazaUser(userNou);
                    audit.logActiune("inregistrare_user", userNou);
                    System.out.println("Inregistrare reusita! Acum te poti autentifica.");
                    break;

                case 0:
                    System.out.println("La revedere!");
                    return;

                default:
                    System.out.println("Optiune invalida.");
            }
        }
    }
}
