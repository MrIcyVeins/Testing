package model;

public class User extends Persoana {
    private String parola;
    private String rol;

    public User(String nume, String email, String parola, String rol) {
        super(nume, email);
        this.parola = parola;
        this.rol = rol;
    }

    public String getParola() {
        return parola;
    }

    public void setParola(String parola) {
        this.parola = parola;
    }

    public String getRol() {
        return rol;
    }

    public void setRol(String rol) {
        this.rol = rol;
    }
}
