
package model;

public class User extends Persoana {
    private String parola;

    public User(String nume, String email, String parola) {
        super(nume, email);
        this.parola = parola;
    }

    public String getParola() {
        return parola;
    }

    public void setParola(String parola) {
        this.parola = parola;
    }
}
