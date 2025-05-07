package service;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class DBConnection {
    private static final String URL = "jdbc:sqlite:db/biblioteca.db";
    private static Connection connection;

    private DBConnection() {}

    public static Connection getConnection() {
        if (connection == null) {
            try {
                connection = DriverManager.getConnection(URL);
                initTables(); // Creează tabele dacă lipsesc
            } catch (SQLException e) {
                System.out.println("Eroare la conectare: " + e.getMessage());
            }
        }
        return connection;
    }

    private static void initTables() {
        String createCititor =
            "CREATE TABLE IF NOT EXISTS cititor (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "nume TEXT NOT NULL," +
            "email TEXT UNIQUE," +
            "parola TEXT" +
            ");";

        String createAutor =
            "CREATE TABLE IF NOT EXISTS autor (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "nume TEXT," +
            "biografie TEXT" +
            ");";

        String createCarte =
            "CREATE TABLE IF NOT EXISTS carte (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "titlu TEXT," +
            "gen TEXT," +
            "an INTEGER," +
            "autor_id INTEGER," +
            "FOREIGN KEY(autor_id) REFERENCES autor(id)" +
            ");";

        try (Statement stmt = connection.createStatement()) {
            stmt.execute(createCititor);
            stmt.execute(createAutor);
            stmt.execute(createCarte);
        } catch (SQLException e) {
            System.out.println("Eroare la crearea tabelelor: " + e.getMessage());
        }
    }
}
