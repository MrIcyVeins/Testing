
package service;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class AdminService {
    public boolean autentificare(String email, String parola) {
        String sql = "SELECT * FROM admin WHERE email = ? AND parola = ?";
        try (PreparedStatement stmt = DBConnection.getConnection().prepareStatement(sql)) {
            stmt.setString(1, email);
            stmt.setString(2, parola);
            ResultSet rs = stmt.executeQuery();
            return rs.next();
        } catch (SQLException e) {
            System.out.println("Eroare autentificare admin: " + e.getMessage());
            return false;
        }
    }
}
