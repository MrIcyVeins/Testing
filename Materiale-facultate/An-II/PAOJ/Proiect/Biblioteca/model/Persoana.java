package model;

// clasa abstracta Persoana
public abstract class Persoana {
    protected String nume;
    protected String email;

    // constructor pentru clasa Persoana
    public Persoana(String nume, String email) {
        this.nume = nume;
        this.email = email;
    }

    // metode - get 
    public String getNume() {
        return nume;
    }

    public String getEmail() {
        return email;
    }

    // suprascrierea metodei toString pentru a afisa nume + (email) la folosirea
    @Override
    public String toString() {
        return nume + " (" + email + ")";
    }
}
