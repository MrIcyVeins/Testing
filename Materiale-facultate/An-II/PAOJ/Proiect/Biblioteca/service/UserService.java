package service;

import model.Cititor;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class UserService {
    private List<Cititor> cititori = new ArrayList<>();
    private Cititor cititorAutentificat = null;

    // Înregistrare utilizator
    public void inregistreazaCititor(Cititor cititor) {
        cititori.add(cititor); // Adaugă și în memorie
        salveazaCititorInDB(cititor); // Salvează în DB
        System.out.println("Cititor înregistrat cu succes!");
    }

    // Autentificare utilizator
    public boolean autentifica(String email, String parola) {
        for (Cititor c : cititori) {
            if (c.getEmail().equals(email) && c.getParola().equals(parola)) {
                cititorAutentificat = c;
                System.out.println("Autentificare reușită! Bun venit, " + c.getNume());
                return true;
            }
        }
        System.out.println("Email sau parolă incorecte.");
        return false;
    }

    // Returnează utilizatorul autentificat
    public Cititor getCititorAutentificat() {
        return cititorAutentificat;
    }

    // Getter pentru toți cititorii (opțional, pentru debug)
    public List<Cititor> getCititori() {
        return cititori;
    }

    // Salvează un utilizator nou în baza de date
    public void salveazaCititorInDB(Cititor c) {
        String sql = "INSERT INTO cititor (nume, email, parola) VALUES (?, ?, ?)";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, c.getNume());
            stmt.setString(2, c.getEmail());
            stmt.setString(3, c.getParola());
            stmt.executeUpdate();
        } catch (SQLException e) {
            System.out.println("Eroare la salvarea cititorului: " + e.getMessage());
        }
    }

    // Încarcă toți cititorii existenți din baza de date
    public void incarcaCititoriDinDB() {
        String sql = "SELECT nume, email, parola FROM cititor";
        try (Statement stmt = DBConnection.getConnection().createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {

            while (rs.next()) {
                String nume = rs.getString("nume");
                String email = rs.getString("email");
                String parola = rs.getString("parola");

                Cititor cititor = new Cititor(nume, email, parola);
                cititori.add(cititor);
            }

            // DEBUG – Afișează utilizatorii încărcați
            System.out.println("Cititori încărcați din baza de date:");
            for (Cititor c : cititori) {
                System.out.println(" - " + c.getEmail());
            }

        } catch (SQLException e) {
            System.out.println("Eroare la încărcarea cititorilor: " + e.getMessage());
        }
    }
}
