
package service;

import model.Cititor;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class UserService {
    private List<Cititor> cititori = new ArrayList<>();
    private Cititor cititorAutentificat = null;

    public void inregistreazaCititor(Cititor cititor, String rol) {
        cititori.add(cititor);
        salveazaCititorInDB(cititor, rol);
        System.out.println("Cititor înregistrat cu succes!");
    }

    public Cititor autentificare(String email, String parola) {
        String sql = "SELECT * FROM cititor WHERE email = ? AND parola = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            stmt.setString(2, parola);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                String nume = rs.getString("nume");
                String e = rs.getString("email");
                String p = rs.getString("parola");
                String rol = rs.getString("rol");
                Cititor c = new Cititor(nume, e, p, rol);
                cititorAutentificat = c;
                return c;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public Cititor getCititorAutentificat() {
        return cititorAutentificat;
    }

    public boolean esteAdmin(Cititor c) {
        return c.getRol().equalsIgnoreCase("admin");
    }

    public void salveazaCititorInDB(Cititor c, String rol) {
        String sql = "INSERT INTO cititor (nume, email, parola, rol) VALUES (?, ?, ?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, c.getNume());
            stmt.setString(2, c.getEmail());
            stmt.setString(3, c.getParola());
            stmt.setString(4, rol);
            stmt.executeUpdate();
        } catch (SQLException e) {
            System.out.println("Eroare la salvarea cititorului: " + e.getMessage());
        }
    }

    public void stergeUtilizator(String email) {
        String checkSql = "SELECT rol FROM cititor WHERE email = ?";
        try (PreparedStatement checkStmt = DBConnection.getConnection().prepareStatement(checkSql)) {
            checkStmt.setString(1, email);
            ResultSet rs = checkStmt.executeQuery();

            if (rs.next()) {
                String rol = rs.getString("rol");
                if (rol.equalsIgnoreCase("admin")) {
                    System.out.println("❌ Nu poți șterge un administrator.");
                    return;
                }
            } else {
                System.out.println("❌ Utilizatorul nu există.");
                return;
            }
        } catch (SQLException e) {
            System.out.println("Eroare la verificare: " + e.getMessage());
            return;
        }

        String sql = "DELETE FROM cititor WHERE email = ?";
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
        String checkSql = "SELECT rol FROM cititor WHERE email = ?";
        try (PreparedStatement checkStmt = DBConnection.getConnection().prepareStatement(checkSql)) {
            checkStmt.setString(1, email);
            ResultSet rs = checkStmt.executeQuery();

            if (rs.next()) {
                String rol = rs.getString("rol");
                if (rol.equalsIgnoreCase("admin") && !email.equals(getCititorAutentificat().getEmail())) {
                    System.out.println("❌ Nu poți modifica parola altui admin.");
                    return;
                }
            } else {
                System.out.println("❌ Utilizatorul nu există.");
                return;
            }
        } catch (SQLException e) {
            System.out.println("Eroare la verificare: " + e.getMessage());
            return;
        }

        String sql = "UPDATE cititor SET parola = ? WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, parolaNoua);
            stmt.setString(2, email);
            int affected = stmt.executeUpdate();
            if (affected > 0) {
                System.out.println("✅ Parolă modificată cu succes.");
            } else {
                System.out.println("❌ Eroare la modificare.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la actualizare: " + e.getMessage());
        }
    }
}
