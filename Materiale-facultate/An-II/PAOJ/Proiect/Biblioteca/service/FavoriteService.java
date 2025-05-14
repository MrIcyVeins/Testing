
package service;

import model.Carte;
import model.User;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class FavoriteService {

    public void adaugaFavorite(User user, Carte carte) {
        int userId = getUserId(user.getEmail());
        int carteId = getCarteId(carte.getTitlu());

        if (userId == -1 || carteId == -1) {
            System.out.println("❌ Utilizator sau carte inexistentă.");
            return;
        }

        if (existaDeja(userId, carteId)) {
            System.out.println("❗ Cartea este deja în favorite.");
            return;
        }

        String sql = "INSERT INTO favorite (user_id, carte_id) VALUES (?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.setInt(2, carteId);
            stmt.executeUpdate();
            System.out.println("✅ Carte adăugată la favorite.");
        } catch (SQLException e) {
            System.out.println("Eroare la adăugare favorite: " + e.getMessage());
        }
    }

    public void afiseazaFavorite(User user) {
        int userId = getUserId(user.getEmail());

        String sql = "SELECT carte.titlu, carte.gen, carte.an FROM favorite " +
                     "JOIN carte ON favorite.carte_id = carte.id WHERE favorite.user_id = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            ResultSet rs = stmt.executeQuery();
            System.out.println(">>> Cărțile tale favorite <<<");
            while (rs.next()) {
                String titlu = rs.getString("titlu");
                String gen = rs.getString("gen");
                int an = rs.getInt("an");
                System.out.println("- " + titlu + " | Gen: " + gen + " | An: " + an);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la afișarea favoritelor: " + e.getMessage());
        }
    }

    private boolean existaDeja(int userId, int carteId) {
        String sql = "SELECT COUNT(*) FROM favorite WHERE user_id = ? AND carte_id = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.setInt(2, carteId);
            ResultSet rs = stmt.executeQuery();
            return rs.next() && rs.getInt(1) > 0;
        } catch (SQLException e) {
            return false;
        }
    }

    private int getUserId(String email) {
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement("SELECT id FROM user WHERE email = ?")) {
            stmt.setString(1, email);
            ResultSet rs = stmt.executeQuery();
            return rs.next() ? rs.getInt("id") : -1;
        } catch (SQLException e) {
            return -1;
        }
    }

    private int getCarteId(String titlu) {
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement("SELECT id FROM carte WHERE LOWER(titlu) = ?")) {
            stmt.setString(1, titlu.toLowerCase());
            ResultSet rs = stmt.executeQuery();
            return rs.next() ? rs.getInt("id") : -1;
        } catch (SQLException e) {
            return -1;
        }
    }
    
    public void stergeFavorite(User user, String titluCarte) {
        int userId = getUserId(user.getEmail());
        int carteId = getCarteId(titluCarte);
    
        String sql = "DELETE FROM favorite WHERE user_id = ? AND carte_id = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.setInt(2, carteId);
            int rows = stmt.executeUpdate();
            if (rows > 0) {
                System.out.println("Carte eliminata din favorite.");
            } else {
                System.out.println("Cartea nu a fost gasita in favorite.");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la stergerea din favorite: " + e.getMessage());
        }
    }
    
}


