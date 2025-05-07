package model;

public abstract class Persoana {
    protected String nume;
    protected String email;

    public Persoana(String nume, String email) {
        this.nume = nume;
        this.email = email;
    }

    public String getNume() {
        return nume;
    }

    public String getEmail() {
        return email;
    }

    @Override
    public String toString() {
        return nume + " (" + email + ")";
    }
}
