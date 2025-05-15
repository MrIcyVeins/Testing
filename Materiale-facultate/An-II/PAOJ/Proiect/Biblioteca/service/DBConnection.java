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

        try (Statement stmt = connection.createStatement()) {
            stmt.execute(createUserTable);
            stmt.execute(createAutor);
            stmt.execute(createCarte);

            String checkAdmin = "SELECT COUNT(*) AS total FROM utilizator WHERE email = 'admin@admin.com' AND rol = 'admin'";
            var rs = stmt.executeQuery(checkAdmin);
            if (rs.next() && rs.getInt("total") == 0) {
                stmt.execute("INSERT INTO utilizator (nume, email, parola, rol) VALUES ('admin', 'admin@admin.com', 'admin123', 'admin')");
                System.out.println("✅ Utilizator admin implicit creat (admin@admin.com)");
            }
        } catch (SQLException e) {
            System.out.println("Eroare la crearea tabelelor: " + e.getMessage());
        }
    }
}
