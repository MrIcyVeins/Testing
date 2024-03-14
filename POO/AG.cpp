// Cerinta
// 7.	Idem pentru mulţimi finite de elemente de un tip neprecizat, cu supraîncărcarea operatorilor =, ==, <, <=, (incluziunea strictă/nestrictă), >, >= (relaţiile duale); + (reuniunea); * (intersecţia); - (binar, diferenţa de mulţimi); <<, >> (adăugarea/ extragerea unui element, care este al doilea operand, la/dintr-o mulţime, reprezentată de primul operand) şi conversia unui element la mulţimea care îl are ca unic element. Multimile se reprezintă printr-un întreg fără semn care este numărul elementelor (pentru mulţimea vidă acest numar va fi 0) şi printr-un vector (adică pointer la tipul elementelor) care conţine elementele. Metodele de creare şi modificare a mulţimilor vor asigura ca fiecare element să apară o singură dată în acest vector.

#include <iostream>
#include <vector>
#include <algorithm> // Pentru std::find

class Multime {
private: // tipul de mostenire privata (signatura)
    std::vector<int> elemente; // Vector pentru a stoca elementele

public: // tipul de mostenire publica (signatura)
    Multime() {}

    // Metoda pentru adăugarea unui element în mulțime
    void adaugaElement(int elem) {
        // Verificăm dacă elementul nu există deja în mulțime
        if (std::find(elemente.begin(), elemente.end(), elem) == elemente.end()) {
            elemente.push_back(elem);
        }
    }

    // Supraincarcarea operatorului de atribuire (=)
    Multime& operator=(const Multime& other) {
        if (this != &other) {
            elemente = other.elemente;
        }
        return *this;
    }

    // Supraincarcarea operatorului de egalitate (==)
    bool operator==(const Multime& other) const {
        return elemente == other.elemente;
    }

    // Supraincarcarea operatorilor de comparație (<, <=, >, >=)
    bool operator<(const Multime& other) const {
        return elemente.size() < other.elemente.size();
    }

    bool operator<=(const Multime& other) const {
        return elemente.size() <= other.elemente.size();
    }

    bool operator>(const Multime& other) const {
        return elemente.size() > other.elemente.size();
    }

    bool operator>=(const Multime& other) const {
        return elemente.size() >= other.elemente.size();
    }

    // Supraincarcarea operatorului de reuniune (+)
    Multime operator+(const Multime& other) const {
        Multime rezultat = *this;
        for (int elem : other.elemente) {
            rezultat.adaugaElement(elem);
        }
        return rezultat;
    }

    // Supraincarcarea operatorului de intersectie (*)
    Multime operator*(const Multime& other) const {
        Multime rezultat;
        for (int elem : elemente) {
            if (std::find(other.elemente.begin(), other.elemente.end(), elem) != other.elemente.end()) {
                rezultat.adaugaElement(elem);
            }
        }
        return rezultat;
    }

    // Supraincarcarea operatorului de diferenta (-)
    Multime operator-(const Multime& other) const {
        Multime rezultat;
        for (int elem : elemente) {
            if (std::find(other.elemente.begin(), other.elemente.end(), elem) == other.elemente.end()) {
                rezultat.adaugaElement(elem);
            }
        }
        return rezultat;
    }

    // Supraincarcarea operatorului de afișare (<<)
    friend std::ostream& operator<<(std::ostream& os, const Multime& m) {
        os << "{ ";
        for (int elem : m.elemente) {
            os << elem << " ";
        }
        os << "}";
        return os;
    }
};

int main() {
    Multime A, B;
    A.adaugaElement(1);
    A.adaugaElement(2);
    A.adaugaElement(3);

    B.adaugaElement(2);
    B.adaugaElement(3);
    B.adaugaElement(4);

    Multime reuniune = A + B; // Reuniunea multimilor A si B
    Multime intersectie = A * B; // Intersectia multimilor A si B
    Multime diferenta = A - B; // Diferenta multimilor A si B

    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "Reuniune: " << reuniune << std::endl;
    std::cout << "Intersectie: " << intersectie << std::endl;
    std::cout << "Diferenta: " << diferenta << std::endl;

    return 0;
}
