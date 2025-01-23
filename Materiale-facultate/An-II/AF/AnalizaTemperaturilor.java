/* 
    Metoda Divide et impera
    Program - Analiza temperaturii minime dintr-un set de date
*/


public class AnalizaTemperaturilor {

    // Functie pentru a gasi cea mai mica temperatura folosind Divide et Impera
    public static double gasesteTemperaturaMinima(double[] temperaturi, int start, int end) {
        // Caz de baza: un singur element
        if (start == end) {
            return temperaturi[start];
        }

        // Divide: Imparte intervalul în doua jumatati
        int mijloc = (start + end) / 2;

        // Conquer: Gaseste temperatura minima in fiecare jumatate
        double minimaStanga = gasesteTemperaturaMinima(temperaturi, start, mijloc);
        double minimaDreapta = gasesteTemperaturaMinima(temperaturi, mijloc + 1, end);

        // Combine: Returneaza cea mai mica temperatura dintre cele doua
        return Math.min(minimaStanga, minimaDreapta);
    }

    public static void main(String[] args) {
        // Exemplu: Temperaturi zilnice înregistrate în diferite locatii timp de o luna
        // Valorile reprezinta temperaturile medii zilnice în grade Celsius
        double[] temperaturi = {
            10.5, 12.3, 9.8, 14.2, 8.1, 11.0, 7.5, 0, -1,
            10.2, 13.5, 6.7, 12.1, 15.0, 5.5, 11.8,
            9.3, 10.0, 8.8, 13.2, 7.1, 12.6, 6.0,
            10.9, 14.1, 5.2, 9.7, 11.3, 7.8, 8.5, 1.5, -1
        };

        // Gaseste cea mai mica temperatura folosind metoda Divide et Impera
        double temperaturaMinima = gasesteTemperaturaMinima(temperaturi, 0, temperaturi.length - 1);

        // Afiseaza rezultatul
        System.out.println("Cea mai mică temperatură înregistrată este: " + temperaturaMinima + " °C");
    }
}
