
import model.*;
import service.*;

import java.io.Console;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        BibliotecaService biblioteca = new BibliotecaService();
        UserService userService = new UserService();
        AuditService audit = new AuditService();

        Cititor user = null;

        while (true) {
            System.out.println("\n>>> BUN VENIT LA BIBLIOTECA <<<");
            System.out.println("1. Login");
            System.out.println("2. Register");
            System.out.println("0. Ieșire");
            System.out.print("Alege opțiunea: ");

            int opt;
            try {
                opt = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Opțiune invalidă.");
                continue;
            }

            Console console = System.console();

            switch (opt) {
                case 1:
                    System.out.print("Email: ");
                    String e = scanner.nextLine();
                    String p;
                    if (console != null) {
                        char[] passwd = console.readPassword("Parolă: ");
                        p = new String(passwd);
                    } else {
                        System.out.print("Parolă: ");
                        p = scanner.nextLine(); // fallback în IDE
                    }
                    user = userService.autentificare(e, p);
                    if (user != null) {
                        audit.logActiune("autentificare_" + user.getRol());
                        System.out.println("Bun venit, " + user.getNume() + "!");
                    } else {
                        System.out.println("Email sau parolă incorecte.");
                    }
                    break;

                case 2:
                    System.out.print("Nume: ");
                    String nume = scanner.nextLine();
                    System.out.print("Email: ");
                    String email = scanner.nextLine();
                    String parola;
                    if (console != null) {
                        char[] pw = console.readPassword("Parolă: ");
                        parola = new String(pw);
                    } else {
                        System.out.print("Parolă: ");
                        parola = scanner.nextLine();
                    }
                    user = new Cititor(nume, email, parola, "user");
                    userService.inregistreazaCititor(user, "user");
                    audit.logActiune("inregistrare_user");
                    System.out.println("Înregistrare reușită! Utilizator autentificat automat.");
                    break;

                case 0:
                    System.out.println("La revedere!");
                    return;

                default:
                    System.out.println("Opțiune invalidă.");
            }

            if (user != null) {
                if (userService.esteAdmin(user)) {
                    MeniuAdmin.run(userService, biblioteca, audit, scanner);
                } else {
                    MeniuUser.run(userService, biblioteca, audit, scanner);
                }
                user = null; // după logout, revine la meniu principal
            }
        }
    }
}
