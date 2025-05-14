
import model.*;
import service.*;

import java.io.Console;
import java.sql.*;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        BibliotecaService biblioteca = new BibliotecaService();
        UserService userService = new UserService();
        AuditService audit = new AuditService();
        Console console = System.console();

        // ✅ Inserare admin implicit în tabela `admin` dacă nu există
        try {
            Statement stmt = DBConnection.getConnection().createStatement();
            ResultSet rs = stmt.executeQuery("SELECT COUNT(*) AS total FROM admin WHERE email = 'admin@admin.com'");
            if (rs.next() && rs.getInt("total") == 0) {
                stmt.execute("INSERT INTO admin (nume, email, parola) " +
                            "VALUES ('admin', 'admin@admin.com', 'admin123')");
                System.out.println("✅ Utilizator admin implicit creat (admin@admin.com)");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la inițializarea adminului: " + e.getMessage());
        }

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
                        parola = new String(console.readPassword("Parolă: "));
                    } else {
                        System.out.print("Parola: ");
                        parola = scanner.nextLine();
                    }

                    try {
                        String sqlAdmin = "SELECT * FROM admin WHERE email = ? AND parola = ?";
                        PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sqlAdmin);
                        stmt.setString(1, email);
                        stmt.setString(2, parola);
                        ResultSet rs = stmt.executeQuery();

                        if (rs.next()) {
                            System.out.println("Bun venit, admin!");
                            audit.logActiune("autentificare_admin");
                            MeniuAdmin.run(userService, biblioteca, audit, scanner);
                        } else {
                            User user = userService.autentificare(email, parola);
                            if (user != null) {
                                audit.logActiune("autentificare_user");
                                System.out.println("Bun venit, " + user.getNume() + "!");
                                MeniuUser.run(userService, biblioteca, audit, scanner);
                            } else {
                                System.out.println("Email sau parola incorecte.");
                            }
                        }

                    } catch (SQLException e) {
                        System.out.println("Eroare la login: " + e.getMessage());
                    }
                    break;

                case 2:
                    System.out.print("Nume: ");
                    String nume = scanner.nextLine();
                    System.out.print("Email: ");
                    String newEmail = scanner.nextLine();
                    String newParola;
                    if (console != null) {
                        newParola = new String(console.readPassword("Parolă: "));
                    } else {
                        System.out.print("Parola: ");
                        newParola = scanner.nextLine();
                    }
                    User userNou = new User(nume, newEmail, newParola);
                    userService.inregistreazaUser(userNou);
                    audit.logActiune("inregistrare_user");
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
