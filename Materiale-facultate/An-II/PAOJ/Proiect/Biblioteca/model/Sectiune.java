package model;

import java.util.ArrayList;
import java.util.List;

// clasa Sectiune
public class Sectiune {
    private String nume;
    private List<Carte> carti;
    
    // constructor
    public Sectiune(String nume) {
        this.nume = nume;
        this.carti = new ArrayList<>();
    }

    // metode
    public String getNume() {
        return nume;
    }

    public List<Carte> getCarti() {
        return carti;
    }

    public void adaugaCarte(Carte carte) {
        carti.add(carte);
    }
}
