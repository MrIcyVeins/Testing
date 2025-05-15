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
                initTables();
            } catch (SQLException e) {
                System.out.println("Eroare la conectare: " + e.getMessage());
            }
        }
        return connection;
    }

    private static void initTables() {
        String createUserTable =
            "CREATE TABLE IF NOT EXISTS utilizator (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "nume TEXT NOT NULL," +
            "email TEXT UNIQUE NOT NULL," +
            "parola TEXT NOT NULL," +
            "rol TEXT CHECK(rol IN ('admin','user')) NOT NULL" +
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

        String createImprumut =
            "CREATE TABLE IF NOT EXISTS imprumut (" +
            "id INTEGER PRIMARY KEY AUTOINCREMENT," +
            "user_id INTEGER NOT NULL," +
            "carte_id INTEGER NOT NULL," +
            "data_imprumut TEXT NOT NULL," +
            "data_returnare TEXT," +
            "FOREIGN KEY(user_id) REFERENCES utilizator(id)," +
            "FOREIGN KEY(carte_id) REFERENCES carte(id)" +
            ");";

        String createFavorite =
            "CREATE TABLE IF NOT EXISTS favorite (" +
            "user_id INTEGER," +
            "carte_id INTEGER," +
            "FOREIGN KEY(user_id) REFERENCES user(id)," +
            "FOREIGN KEY(carte_id) REFERENCES carte(id)" +
            ");";

        try (Statement stmt = connection.createStatement()) {
            stmt.execute(createUserTable);
            stmt.execute(createAutor);
            stmt.execute(createCarte);
            stmt.execute(createImprumut);
            stmt.execute(createFavorite);
        } catch (SQLException e) {
            System.out.println("Eroare la crearea tabelelor: " + e.getMessage());
        }
    }
}
