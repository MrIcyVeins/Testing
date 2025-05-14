
package service;

import model.Carte;
import model.User;

import java.sql.*;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class ImprumutService {

    public void imprumutaCarte(User user, String titluCarte) {
        int userId = getUserId(user.getEmail());
        int carteId = getCarteId(titluCarte);

        if (userId == -1 || carteId == -1) {
            System.out.println("❌ Utilizator sau carte inexistenta.");
            return;
        }

        if (carteEsteImprumutata(carteId)) {
            System.out.println("❌ Cartea este deja împrumutata.");
            return;
        }

        String sql = "INSERT INTO imprumut (user_id, carte_id, data_imprumut) VALUES (?, ?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.setInt(2, carteId);
            stmt.setString(3, LocalDate.now().toString());
            stmt.executeUpdate();
            System.out.println("✅ Carte imprumutata cu succes.");
        } catch (SQLException e) {
            System.out.println("Eroare la imprumut: " + e.getMessage());
        }
    }

    public void returneazaCarte(User user, String titluCarte) {
        int userId = getUserId(user.getEmail());
        int carteId = getCarteId(titluCarte);

        String sql = "UPDATE imprumut SET data_returnare = ? WHERE user_id = ? AND carte_id = ? AND data_returnare IS NULL";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, LocalDate.now().toString());
            stmt.setInt(2, userId);
            stmt.setInt(3, carteId);
            int updated = stmt.executeUpdate();
            if (updated > 0) {
                System.out.println("✅ Carte returnata.");
            } else {
                System.out.println("❌ Nu ai aceasta carte împrumutata.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la returnare: " + e.getMessage());
        }
    }

    public void veziImprumuturi(User user) {
        int userId = getUserId(user.getEmail());
        String sql = "SELECT carte.titlu, imprumut.data_imprumut, imprumut.data_returnare " +
                     "FROM imprumut JOIN carte ON imprumut.carte_id = carte.id " +
                     "WHERE imprumut.user_id = ?";

        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            ResultSet rs = stmt.executeQuery();
            System.out.println(">>> Carti împrumutate <<<");
            while (rs.next()) {
                String titlu = rs.getString("titlu");
                String dataImprumut = rs.getString("data_imprumut");
                String dataReturnare = rs.getString("data_returnare");
                System.out.println("- " + titlu + " | Imprumutat la: " + dataImprumut +
                                   (dataReturnare != null ? " | Returnat: " + dataReturnare : " | ACTIV"));
            }
        } catch (SQLException e) {
            System.out.println("Eroare la afisarea imprumuturilor: " + e.getMessage());
        }
    }

    private boolean carteEsteImprumutata(int carteId) {
        String sql = "SELECT COUNT(*) FROM imprumut WHERE carte_id = ? AND data_returnare IS NULL";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, carteId);
            ResultSet rs = stmt.executeQuery();
            return rs.next() && rs.getInt(1) > 0;
        } catch (SQLException e) {
            System.out.println("Eroare la verificare stare carte: " + e.getMessage());
            return false;
        }
    }

    private int getUserId(String email) {
        String sql = "SELECT id FROM user WHERE email = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            ResultSet rs = stmt.executeQuery();
            return rs.next() ? rs.getInt("id") : -1;
        } catch (SQLException e) {
            return -1;
        }
    }

    private int getCarteId(String titlu) {
        String sql = "SELECT id FROM carte WHERE LOWER(titlu) = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, titlu.toLowerCase());
            ResultSet rs = stmt.executeQuery();
            return rs.next() ? rs.getInt("id") : -1;
        } catch (SQLException e) {
            return -1;
        }
    }
}
