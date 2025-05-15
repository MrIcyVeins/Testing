package service;

import model.User;

import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;

public class AuditService {
    private static final String FILE_NAME = "audit.csv";

    public void logActiune(String actiune) {
        logActiune(actiune, null);
    }

    public void logActiune(String actiune, User user) {
        try (FileWriter fw = new FileWriter(FILE_NAME, true)) {
            String timestamp = LocalDateTime.now().toString();
            String userInfo = user != null ? user.getEmail() + " (" + user.getRol() + ")" : "anonim";
            fw.write(actiune + "," + userInfo + "," + timestamp + "\n");
        } catch (IOException e) {
            System.out.println("Eroare la scrierea in audit: " + e.getMessage());
        }
    }
}
