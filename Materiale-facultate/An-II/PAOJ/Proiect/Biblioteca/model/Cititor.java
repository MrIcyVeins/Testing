package model;

public class Cititor extends Persoana {
    private String parola;

    public Cititor(String nume, String email, String parola) {
        super(nume, email);
        this.parola = parola;
    }

    public String getParola() {
        return parola;
    }
}
