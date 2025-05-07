package model;

import java.util.ArrayList;
import java.util.List;

public class Sectiune {
    private String nume;
    private List<Carte> carti;

    public Sectiune(String nume) {
        this.nume = nume;
        this.carti = new ArrayList<>();
    }

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
