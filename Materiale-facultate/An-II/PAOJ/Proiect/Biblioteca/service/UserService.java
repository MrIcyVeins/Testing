
package service;

import model.User;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class UserService {
    private List<User> utilizatori = new ArrayList<>();
    private User userAutentificat = null;

    public void inregistreazaUser(User user) {
        utilizatori.add(user);
        salveazaUserInDB(user);
        System.out.println("Utilizator înregistrat cu succes!");
    }

    public User autentificare(String email, String parola) {
        String sql = "SELECT * FROM user WHERE email = ? AND parola = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            stmt.setString(2, parola);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                String nume = rs.getString("nume");
                String e = rs.getString("email");
                String p = rs.getString("parola");
                User u = new User(nume, e, p);
                userAutentificat = u;
                return u;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public User getUserAutentificat() {
        return userAutentificat;
    }

    public void salveazaUserInDB(User u) {
        String sql = "INSERT INTO user (nume, email, parola) VALUES (?, ?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, u.getNume());
            stmt.setString(2, u.getEmail());
            stmt.setString(3, u.getParola());
            stmt.executeUpdate();
        } catch (SQLException e) {
            System.out.println("Eroare la salvarea userului: " + e.getMessage());
        }
    }

    public void stergeUser(String email) {
        String sql = "DELETE FROM user WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            int affected = stmt.executeUpdate();
            if (affected > 0) {
                System.out.println("✅ Utilizator șters cu succes.");
            } else {
                System.out.println("❌ Utilizatorul nu a putut fi șters.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la ștergere: " + e.getMessage());
        }
    }

    public void schimbaParola(String email, String parolaNoua) {
        String sql = "UPDATE user SET parola = ? WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, parolaNoua);
            stmt.setString(2, email);
            int affected = stmt.executeUpdate();
            if (affected > 0) {
                System.out.println("✅ Parola modificată cu succes.");
            } else {
                System.out.println("❌ Eroare la modificare.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la actualizare: " + e.getMessage());
        }
    }

    public void afiseazaTotiUserii() {
        String sql = "SELECT nume, email FROM user";
        try (Statement stmt = DBConnection.getConnection().createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {

            System.out.println("📋 Lista utilizatori:");
            while (rs.next()) {
                String nume = rs.getString("nume");
                String email = rs.getString("email");
                System.out.println("- " + nume + " | " + email);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la afisarea userilor: " + e.getMessage());
        }
    }
}
