package service;

import model.User;
import java.sql.*;

public class UserService {
    private User userAutentificat = null;

    // metode din UserService
    public void inregistreazaUser(User user) {
        String sql = "INSERT INTO utilizator (nume, email, parola, rol) VALUES (?, ?, ?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, user.getNume()); // mostenit din clasa Persoana
            stmt.setString(2, user.getEmail()); // mostenit din clasa Persoana
            stmt.setString(3, user.getParola()); // mostenit din clasa User
            stmt.setString(4, user.getRol()); // mostenit din clasa User
            stmt.executeUpdate();
            System.out.println("✅ Utilizator inregistrat cu succes.");
        } catch (SQLException e) {
            System.out.println("Eroare la inregistrare: " + e.getMessage());
        }
    }

    public User autentificare(String email, String parola) {
        String sql = "SELECT * FROM utilizator WHERE email = ? AND parola = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            stmt.setString(2, parola);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                String nume = rs.getString("nume");
                String rol = rs.getString("rol");
                userAutentificat = new User(nume, email, parola, rol);
                return userAutentificat;
            }
        } catch (SQLException e) {
            System.out.println("Eroare la autentificare: " + e.getMessage());
        }
        return null;
    }

    public User getUserAutentificat() {
        return userAutentificat;
    }

    public void afiseazaTotiUserii() {
        String sql = "SELECT nume, email, rol FROM utilizator WHERE rol = 'user'";
        try (Statement stmt = DBConnection.getConnection().createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            System.out.println(">>> Lista utilizatori:");
            while (rs.next()) {
                String nume = rs.getString("nume");
                String email = rs.getString("email");
                String rol = rs.getString("rol");
                System.out.println("- " + nume + " | " + email + " | " + rol);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la afișarea utilizatorilor: " + e.getMessage());
        }
    }

    public void schimbaParola(String email, String parolaNoua) {
        String sql = "UPDATE utilizator SET parola = ? WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, parolaNoua);
            stmt.setString(2, email);
            int rows = stmt.executeUpdate();
            if (rows > 0) {
                System.out.println("✅ Parola a fost actualizata pentru " + email);
            } else {
                System.out.println("❌ Nu s-a gasit utilizatorul.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la actualizarea parolei: " + e.getMessage());
        }
    }

    public void stergeUtilizator(String email) {
        String sql = "DELETE FROM utilizator WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            int affected = stmt.executeUpdate();
            if (affected > 0) {
                System.out.println("✅ Utilizator sters.");
            } else {
                System.out.println("❌ Utilizatorul nu a fost gasit.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la stergerea utilizatorului: " + e.getMessage());
        }
    }
}
