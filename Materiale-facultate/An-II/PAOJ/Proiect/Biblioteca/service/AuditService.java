package service;

import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;

public class AuditService {
    private static final String FILE_NAME = "audit.csv";

    public void logActiune(String actiune) {
        try (FileWriter fw = new FileWriter(FILE_NAME, true)) {
            fw.write(actiune + "," + LocalDateTime.now() + "\n");
        } catch (IOException e) {
            System.out.println("Eroare la scrierea în audit: " + e.getMessage());
        }
    }
}
