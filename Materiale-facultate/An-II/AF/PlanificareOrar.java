/* 
    Metoda Backtracking
    Program - Planificator orar in functie de timpul disponibil
*/

// Importuri pentru gestionarea listelor
import java.util.ArrayList;
import java.util.List;


// Clasa principala - Backtracking
public class PlanificareOrar {

    // Clasa pentru o activitate
    static class Activitate {
        String name;
        int duration; // Durata in minute

        // constructor pentru obiectul "Activitate"
        Activitate(String name, int duration) {
            this.name = name;
            this.duration = duration;
        }

        // suprascriere metoda toString() pentru a afisa activitatea
        public String toString() {
            return name + " (" + duration + " min)";
        }
    }

    // Lista orarOptim - cu elemente de tip Activitate
    static List<Activitate> orarOptim = new ArrayList<>();
    static int timpMax;
    static int timpOptim = 0;

    public static void main(String[] args) {
        // Activitati posibile - lista din fisier (de facut)
        List<Activitate> ListaActivitati = new ArrayList<>();
        ListaActivitati.add(new Activitate("Citit", 60));
        ListaActivitati.add(new Activitate("Sport", 30));
        ListaActivitati.add(new Activitate("Meditatie", 20));
        ListaActivitati.add(new Activitate("Gatit", 90));
        ListaActivitati.add(new Activitate("Serial", 40));

        // Timp total disponibil (in minute) - de introdus de la tastatura (de facut)
        timpMax = 200;

        // Lista temporara pentru backtracking
        List<Activitate> orarCurent = new ArrayList<>();

        // Start backtracking
        planOrar(ListaActivitati, orarCurent, 0, 0);

        // Afiseaza cel mai bun orar gasit
        System.out.println("Cel mai bun orar (Timp folosit: " + timpOptim + " min):");
        for (Activitate element : orarOptim) {
            System.out.println(element);
        }
    }

    // Metoda de backtracking
    private static void planOrar(List<Activitate> ListaActivitati, List<Activitate> orarCurent, int timpCurent, int index) {
        // Actualizeaza solutia cea mai buna daca timpul folosit este mai mare
        if (timpCurent > timpOptim) {
            timpOptim = timpCurent;
            orarOptim = new ArrayList<>(orarCurent);
        }

        // Itereaza prin activitati pentru a incerca diverse combinatii
        for (int i = index; i < ListaActivitati.size(); i++) {
            Activitate activitate = ListaActivitati.get(i);

            // Verifica daca activitatea curenta se potrivește in timp
            if (timpCurent + activitate.duration <= timpMax) {
                // Adauga activitatea la orarul curent
                orarCurent.add(activitate);

                // Continua cu urmatoarele activitati
                planOrar(ListaActivitati, orarCurent, timpCurent + activitate.duration, i + 1);

                // Elimina activitatea adaugata (backtracking)
                orarCurent.remove(orarCurent.size() - 1);
            }
        }
    }
}
