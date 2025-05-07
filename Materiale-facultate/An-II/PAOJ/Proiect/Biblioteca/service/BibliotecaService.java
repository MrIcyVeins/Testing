package service;

import model.Autor;
import model.Carte;
import model.Sectiune;

import java.sql.*;
import java.util.*;

public class BibliotecaService {
    private List<Carte> carti = new ArrayList<>();
    private Set<Autor> autori = new TreeSet<>(Comparator.comparing(Autor::getNume));
    private Map<String, Sectiune> sectiuni = new HashMap<>();

    public void adaugaCarte(Carte carte, String numeSectiune) {
        carti.add(carte);
        autori.add(carte.getAutor());

        sectiuni.putIfAbsent(numeSectiune, new Sectiune(numeSectiune));
        sectiuni.get(numeSectiune).adaugaCarte(carte);

        salveazaCarteInDB(carte);
    }

    public void afiseazaCarti() {
        if (carti.isEmpty()) {
            System.out.println("Nu există cărți în bibliotecă.");
            return;
        }
        for (Carte c : carti) {
            System.out.println(c);
        }
    }

    public void cautaCarteDupaTitlu(String titlu) {
        for (Carte c : carti) {
            if (c.getTitlu().equalsIgnoreCase(titlu)) {
                System.out.println("Găsită: " + c);
                return;
            }
        }
        System.out.println("Carte negăsită.");
    }

    public void stergeCarte(String titlu) {
        boolean gasita = false;
        Iterator<Carte> it = carti.iterator();
        while (it.hasNext()) {
            Carte c = it.next();
            if (c.getTitlu().equalsIgnoreCase(titlu)) {
                it.remove();
                gasita = true;
                break;
            }
        }

        if (gasita) {
            System.out.println("Carte ștearsă din memorie. (Ștergerea din DB nu e implementată încă)");
        } else {
            System.out.println("Carte negăsită.");
        }
    }

    public void afiseazaCartiPeSectiune(String numeSectiune) {
        Sectiune sectiune = sectiuni.get(numeSectiune);
        if (sectiune != null) {
            for (Carte c : sectiune.getCarti()) {
                System.out.println(c);
            }
        } else {
            System.out.println("Secțiune inexistentă.");
        }
    }

    public void afiseazaAutori() {
        if (autori.isEmpty()) {
            System.out.println("Nu există autori încă.");
            return;
        }
        for (Autor a : autori) {
            System.out.println(a);
        }
    }

    public List<Carte> getCarti() {
        return carti;
    }

    // ✅ Persiste cartea și autorul în DB
    public void salveazaCarteInDB(Carte carte) {
        try {
            // 1. Inserează autorul (doar dacă nu există)
            String sqlAutor = "INSERT OR IGNORE INTO autor (nume, biografie) VALUES (?, ?)";
            try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sqlAutor)) {
                stmt.setString(1, carte.getAutor().getNume());
                stmt.setString(2, carte.getAutor().getBiografie());
                stmt.executeUpdate();
            }

            // 2. Obține ID autor
            int autorId = -1;
            try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(
                    "SELECT id FROM autor WHERE nume = ?")) {
                stmt.setString(1, carte.getAutor().getNume());
                ResultSet rs = stmt.executeQuery();
                if (rs.next()) {
                    autorId = rs.getInt("id");
                }
            }

            // 3. Inserează cartea
            String sqlCarte = "INSERT INTO carte (titlu, gen, an, autor_id) VALUES (?, ?, ?, ?)";
            try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sqlCarte)) {
                stmt.setString(1, carte.getTitlu());
                stmt.setString(2, carte.getGen());
                stmt.setInt(3, carte.getAn());
                stmt.setInt(4, autorId);
                stmt.executeUpdate();
            }

        } catch (SQLException e) {
            System.out.println("Eroare la salvarea cărții: " + e.getMessage());
        }
    }

    // ✅ Încarcă toate cărțile + autorii din DB
    public void incarcaCartiDinDB() {
        String sql = "SELECT c.titlu, c.gen, c.an, a.nume AS autor_nume, a.biografie AS autor_bio " +
                     "FROM carte c " +
                     "JOIN autor a ON c.autor_id = a.id";
        try (Statement stmt = DBConnection.getConnection().createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {

            while (rs.next()) {
                String titlu = rs.getString("titlu");
                String gen = rs.getString("gen");
                int an = rs.getInt("an");
                String autorNume = rs.getString("autor_nume");
                String autorBio = rs.getString("autor_bio");

                Autor autor = new Autor(autorNume, autorBio);
                Carte carte = new Carte(titlu, autor, gen, an);

                carti.add(carte);
                autori.add(autor); // TreeSet
            }

            System.out.println("📚 Cărțile au fost încărcate din baza de date.");

        } catch (SQLException e) {
            System.out.println("Eroare la încărcarea cărților din DB: " + e.getMessage());
        }
    }
}
