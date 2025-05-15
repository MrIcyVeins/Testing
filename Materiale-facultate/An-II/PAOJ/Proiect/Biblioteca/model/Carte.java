package model;

import java.util.Objects;

public class Carte {
    private String titlu;
    private Autor autor;
    private String gen;
    private int an;

    // constructor Carte
    public Carte(String titlu, Autor autor, String gen, int an) {
        this.titlu = titlu;
        this.autor = autor;
        this.gen = gen;
        this.an = an;
    }

    // metode get Carte
    public String getTitlu() {
        return titlu;
    }

    public Autor getAutor() {
        return autor;
    }

    public String getGen() {
        return gen;
    }

    public int getAn() {
        return an;
    }

    // supraincarcarea metodei toString
    @Override
    public String toString() {
        return titlu + " - " + autor.getNume() + " [" + gen + ", " + an + "]";
    }

    // 
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Carte)) return false;
        Carte carte = (Carte) o;
        return Objects.equals(titlu, carte.titlu) && Objects.equals(autor, carte.autor);
    }

    // 
    @Override
    public int hashCode() {
        return Objects.hash(titlu, autor);
    }
}
