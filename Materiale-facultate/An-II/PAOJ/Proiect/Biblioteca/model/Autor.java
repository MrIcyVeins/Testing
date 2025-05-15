package model;

import java.util.Objects;

public class Autor {
    private String nume;
    private String biografie;

    // constructor Autor
    public Autor(String nume, String biografie) {
        this.nume = nume;
        this.biografie = biografie;
    }

    public String getNume() {
        return nume;
    }

    public String getBiografie() {
        return biografie;
    }

    // supraincarcarea metodie toString()
    @Override
    public String toString() {
        return nume + " - " + biografie;
    }

    // suprascriere - verifica daca doua obiecte de tip Autor sunt egale ( == campul nume )
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Autor)) return false;
        Autor autor = (Autor) o;
        return Objects.equals(nume, autor.nume);
    }

    // suprascriere - folosire hashCode ( obligatoriu ) 
    @Override
    public int hashCode() {
        return Objects.hash(nume);
    }
}
