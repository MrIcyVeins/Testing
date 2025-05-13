
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

    public void afiseazaCarti1() {
        if (carti.isEmpty()) {
            System.out.println("Nu există cărți în bibliotecă.");
            return;
        }
        for (Carte c : carti) {
            System.out.println(c);
        }
    }

    public Carte afiseazaCarti() {
        String sql = "SELECT * FROM carte";
        try (Statement stmt = DBConnection.getConnection().createStatement()) {
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println(">>> Lista carti <<<");

            while (rs.next()) {
                int id = rs.getInt("id");
                String titlu = rs.getString("titlu");
                String gen = rs.getString("gen");
                int an = rs.getInt("an");
                System.out.println("Id: " + id + " | Titlu: " + titlu + " | Gen: " + gen + " | An: " + an);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public Autor afiseazaAutori() {
        String sql = "SELECT * FROM autor";
        try (Statement stmt = DBConnection.getConnection().createStatement()) {
            ResultSet rs = stmt.executeQuery(sql);

            System.out.println(">>> Lista autori <<<");

            while (rs.next()) {
                String nume = rs.getString("nume");
                String biografie = rs.getString("biografie");
                System.out.println("Nume: " + nume + " | Biografie: " + biografie);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
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

    public List<Carte> getCarti() {
        return carti;
    }

    public void salveazaCarteInDB(Carte carte) {
        try {
            String sqlAutor = "INSERT OR IGNORE INTO autor (nume, biografie) VALUES (?, ?)";
            try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sqlAutor)) {
                stmt.setString(1, carte.getAutor().getNume());
                stmt.setString(2, carte.getAutor().getBiografie());
                stmt.executeUpdate();
            }

            int autorId = -1;
            try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(
                    "SELECT id FROM autor WHERE nume = ?")) {
                stmt.setString(1, carte.getAutor().getNume());
                ResultSet rs = stmt.executeQuery();
                if (rs.next()) {
                    autorId = rs.getInt("id");
                }
            }

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

    public void afiseazaStatistici() {
        try {
            Connection conn = DBConnection.getConnection();

            Statement stmt1 = conn.createStatement();
            ResultSet rs1 = stmt1.executeQuery("SELECT COUNT(*) FROM carte");
            int totalCarti = rs1.next() ? rs1.getInt(1) : 0;

            Statement stmt2 = conn.createStatement();
            ResultSet rs2 = stmt2.executeQuery("SELECT COUNT(*) FROM autor");
            int totalAutori = rs2.next() ? rs2.getInt(1) : 0;

            System.out.println("📊 Statistici:");
            System.out.println(" - Total cărți: " + totalCarti);
            System.out.println(" - Total autori: " + totalAutori);

        } catch (SQLException e) {
            System.out.println("Eroare la statistici: " + e.getMessage());
        }
    }

    public void cautaCartePartialTitlu(String fragment) {
        String sql = "SELECT * FROM carte WHERE LOWER(titlu) LIKE ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, "%" + fragment.toLowerCase() + "%");
            ResultSet rs = stmt.executeQuery();
            boolean found = false;
            while (rs.next()) {
                found = true;
                String titlu = rs.getString("titlu");
                String gen = rs.getString("gen");
                int an = rs.getInt("an");
                System.out.println("- " + titlu + " | Gen: " + gen + " | An: " + an);
            }
            if (!found) {
                System.out.println("❌ Nicio carte găsită pentru: " + fragment);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la căutarea titlului: " + e.getMessage());
        }
    }

    public void cautaCartiDupaGen(String genCautat) {
        String sql = "SELECT * FROM carte WHERE LOWER(gen) = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, genCautat.toLowerCase());
            ResultSet rs = stmt.executeQuery();
            boolean found = false;
            while (rs.next()) {
                found = true;
                String titlu = rs.getString("titlu");
                int an = rs.getInt("an");
                System.out.println("- " + titlu + " | Gen: " + genCautat + " | An: " + an);
            }
            if (!found) {
                System.out.println("❌ Nicio carte găsită pentru genul: " + genCautat);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la căutarea după gen: " + e.getMessage());
        }
    }

    public void cautaCartiDupaAutor(String autor) {
        String sql = "SELECT carte.titlu, carte.gen, carte.an FROM carte " +
                     "JOIN autor ON carte.autor_id = autor.id WHERE LOWER(autor.nume) = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, autor.toLowerCase());
            ResultSet rs = stmt.executeQuery();
            boolean found = false;
            while (rs.next()) {
                found = true;
                String titlu = rs.getString("titlu");
                String gen = rs.getString("gen");
                int an = rs.getInt("an");
                System.out.println("- " + titlu + " | Gen: " + gen + " | An: " + an);
            }
            if (!found) {
                System.out.println("❌ Nicio carte găsită pentru autorul: " + autor);
            }
        } catch (SQLException e) {
            System.out.println("Eroare la căutarea după autor: " + e.getMessage());
        }
    }

    public Carte getCarteByTitlu(String titlu) {
        for (Carte c : carti) {
            if (c.getTitlu().equalsIgnoreCase(titlu)) {
                return c;
            }
        }
        return null;
    }
}
