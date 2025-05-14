
package service;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;

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
        try (Statement stmt = connection.createStatement()) {
            String createUser =
                "CREATE TABLE IF NOT EXISTS user (" +
                "id INTEGER PRIMARY KEY AUTOINCREMENT," +
                "nume TEXT NOT NULL," +
                "email TEXT UNIQUE," +
                "parola TEXT" +
                ");";

            String createAdmin =
                "CREATE TABLE IF NOT EXISTS admin (" +
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

            String createFavorite =
                "CREATE TABLE IF NOT EXISTS favorite (" +
                "user_id INTEGER," +
                "carte_id INTEGER," +
                "FOREIGN KEY(user_id) REFERENCES user(id)," +
                "FOREIGN KEY(carte_id) REFERENCES carte(id)" +
                ");";

            String createImprumut =
                "CREATE TABLE IF NOT EXISTS imprumut (" +
                "id INTEGER PRIMARY KEY AUTOINCREMENT," +
                "user_id INTEGER," +
                "carte_id INTEGER," +
                "data_imprumut TEXT," +
                "data_returnare TEXT," +
                "FOREIGN KEY(user_id) REFERENCES user(id)," +
                "FOREIGN KEY(carte_id) REFERENCES carte(id)" +
                ");";

            stmt.execute(createUser);
            stmt.execute(createAdmin);
            stmt.execute(createAutor);
            stmt.execute(createCarte);
            stmt.execute(createFavorite);
            stmt.execute(createImprumut);

        } catch (SQLException e) {
            System.out.println("Eroare la crearea tabelelor: " + e.getMessage());
        }
    }
}
