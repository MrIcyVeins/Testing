package model;

public class Cititor extends Persoana {
    private String parola;
    private String rol;

    public Cititor(String nume, String email, String parola, String rol) {
        super(nume, email);
        this.parola = parola;
        this.rol = rol;
    }

    public String getParola() {
        return parola;
    }

    public String getRol() {
        return rol;
    }
}
