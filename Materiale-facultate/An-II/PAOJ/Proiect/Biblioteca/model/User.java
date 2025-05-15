package model;

// clasa User
public class User extends Persoana {
    private String parola;
    private String rol;

    // constructorul clasei User
    public User(String nume, String email, String parola, String rol) {
        super(nume, email); // apeleaza constructorul din clasa parinte Persoana
        this.parola = parola;
        this.rol = rol;
    }

    // metode - get/set
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
