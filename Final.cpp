// Cerinta
// 7.	Idem pentru mulţimi finite de elemente de un tip neprecizat, cu supraîncărcarea operatorilor =, ==, <, <=, (incluziunea strictă/nestrictă), >, >= (relaţiile duale); + (reuniunea); * (intersecţia); - (binar, diferenţa de mulţimi); <<, >> (adăugarea/ extragerea unui element, care este al doilea operand, la/dintr-o mulţime, reprezentată de primul operand) şi conversia unui element la mulţimea care îl are ca unic element. Multimile se reprezintă printr-un întreg fără semn care este numărul elementelor (pentru mulţimea vidă acest numar va fi 0) şi printr-un vector (adică pointer la tipul elementelor) care conţine elementele. Metodele de creare şi modificare a mulţimilor vor asigura ca fiecare element să apară o singură dată în acest vector.

#include <iostream>

class ClasaSet {
private:
    unsigned int marime_; // numarul elementelor reprezentat de variabila marime_ (cu valoarea "unsigned int" = poate stoca doar valoare pozitiva si zero)
    int* elemente_; // vector (pointer la un intreg) care va contine elementele ( zona de memorie unde sunt stocate elementele ) 

public:
    // Constructor
    // Constructorul "ClasaSet()" va fi apelat atunci cand se creaza o instanta (obiect) a clasei ClasaSet
    // Initializarea variabilelor membre (marime_ si elemente_) se face in felul urmator: marime_ va lua valoarea "0" si elemente_va lua valoarea  "nullptr" ( nu pointeaza catre nici o zona de memorie // este un pointer null )
    ClasaSet() : marime_(0), elemente_(nullptr) {} 

//    // Destructor
//   ~ClasaSet() {
//        delete[] elemente_;
//    }

///////////////////////////////////////////////    Adaugare element in set (supraincarcarea operatorului "<<")
    ClasaSet& operator<<(int valoare) {                   
        if (!contine(valoare)) {
            int* elementeNoi = new int[marime_ + 1];
            for (unsigned int i = 0; i < marime_; ++i) {
                elementeNoi[i] = elemente_[i];
            }
            elementeNoi[marime_] = valoare;
            delete[] elemente_;
            elemente_ = elementeNoi;
            ++marime_;
        }
        return *this;
    }

///////////////////////////////////////////////       Extragere element din set (supraincarcarea operatorului ">>")
    ClasaSet& operator>>(int valoare) {
        if (contine(valoare)) {
            int* elementeNoi = new int[marime_ - 1];
            unsigned int j = 0;
            for (unsigned int i = 0; i < marime_; ++i) {
                if (elemente_[i] != valoare) {
                    elementeNoi[j] = elemente_[i];
                    ++j;
                }
            }
            delete[] elemente_;
            elemente_ = elementeNoi;
            --marime_;
        }
        return *this;
    }

    // Verificare daca elementul exista deja in set sau nu 
    bool contine(int valoare) const {
        for (unsigned int i = 0; i < marime_; ++i) {
            if (elemente_[i] == valoare) {
                return true;
            }
        }
        return false;
    }

///////////////////////////////////////////////      Reuniune (supraincarcarea operatorului "+")
    ClasaSet operator+(const ClasaSet& secundar) const {
        ClasaSet rezultat;
        if (marime_ == 0 && secundar.marime_ == 0) {
            // Daca ambele seturi sunt goale atunci si rezultatul trebuie sa fie la fel
            return rezultat;
        } else if (marime_ == 0) {
            // Doar set1 este gol > copiaza elementele set2 in rezultat
            for (unsigned int i = 0; i < secundar.marime_; ++i) {
                rezultat << secundar.elemente_[i];
            }
        } else if (secundar.marime_ == 0) {
            // Doar set2 este gol > copiaza elementele set1 in rezultat
            for (unsigned int i = 0; i < marime_; ++i) {
                rezultat << elemente_[i];
            }
        } else {
            // Ambele seturi au elemente > copiaza ambele seturi in rezultat
            for (unsigned int i = 0; i < marime_; ++i) {
                rezultat << elemente_[i];
            }
            for (unsigned int i = 0; i < secundar.marime_; ++i) {
                if (!rezultat.contine(secundar.elemente_[i])) {
                    rezultat << secundar.elemente_[i];
                }
            }
        }
        rezultat.sorteaza();
        return rezultat;
    }

///////////////////////////////////////////////     Intersectie (supraincarcarea operatorului "*")
    ClasaSet operator*(const ClasaSet& secundar) const {
    ClasaSet rezultat;
    if (elemente_ == nullptr || secundar.elemente_ == nullptr) {
        // Unul sau ambele seturi sunt goale deci rezultatul ar trebui sa fie la fel 
        return  ClasaSet();
    }

    for (unsigned int i = 0; i < marime_; ++i) {
        if (secundar.contine(elemente_[i])) {
            rezultat << elemente_[i];
        }
    }

    if (rezultat.marime_ == 0) {
        rezultat << 0;
    }

    rezultat.sorteaza();
    return rezultat;
    }

// Sorteaza fiecare element din vector in ordine crescatoare
    void sorteaza() {
        for (unsigned int i = 0; i < marime_ - 1; ++i) {
            for (unsigned int j = 0; j < marime_ - i - 1; ++j) {
                if (elemente_[j] > elemente_[j + 1]) {
                    int temporar = elemente_[j];
                    elemente_[j] = elemente_[j + 1];
                    elemente_[j + 1] = temporar;
                }
            }
        }
    }

///////////////////////////////////////////////          Diferenta (supraincarcarea operatorului "-")
    ClasaSet operator-(const ClasaSet& secundar) const {
        // Dacă mulţimile sunt egale, returnează o mulţime vidă
        if (marime_ == secundar.marime_) {
            bool suntEgale = true;
            for (unsigned int i = 0; i < marime_; ++i) {
                if (elemente_[i] != secundar.elemente_[i]) {
                    suntEgale = false;
                    break;
                }
            }
            if (suntEgale) {
                ClasaSet setGol;
                return setGol;
            }
        }
        ClasaSet rezultat;
        if (marime_ == 0) {
            return rezultat;
        } else {
            for (unsigned int i = 0; i < marime_; ++i) {
                if (elemente_[i] != 0 && !secundar.contine(elemente_[i])) {
                    rezultat << elemente_[i];
                    rezultat.sorteaza();
                }
            }
        }
        return rezultat;
    }


///////////////////////////////////////////////         Incluziune stricta (supraincarcarea operatorului "<=")
    ClasaSet operator<=(const ClasaSet& secundar) const {
        if (secundar.marime_ > marime_) {
            return ClasaSet(); // Daca marimea setului secundar este mai mare decat marimea primului set > returneaza null
        }

        // Creaza un set nou (elemenete comune)
        ClasaSet setComun;

        // Verifica daca elementele din setul secundar sunt si in al doilea set
        for (unsigned int i = 0; i < secundar.marime_; ++i) {
            bool gasit = false;
            for (unsigned int j = 0; j < marime_; ++j) {
                if (elemente_[i] == secundar.elemente_[j]) {
                    setComun << 1;              // Adauga 1 (true) in setComun pentru output
                    gasit = true;
                    break;
                }
            }

            if (!gasit) {
                return ClasaSet(); // Daca nu se gasesc elemente in comun > returneaza set gol (null)
            }
        }

        // Daca toate elementele au fost gasite > returneaza set comun ( cu elementele comune )
        return setComun;
    }

///////////////////////////////////////////////         Incluziune stricta (supraincarcarea operatorului ">=")
    ClasaSet operator>=(const ClasaSet& secundar) const {
        if (marime_ > secundar.marime_) {
            return ClasaSet(); // Daca marimea primului set este mai mare decat marimea celui de-al doilea set > returneaza null
        }

        // Creaza un set nou (elemenete comune)
        ClasaSet setComun;

        // Verifica daca elementele din primul set sunt si in setul secundar
        for (unsigned int i = 0; i < marime_; ++i) {
            bool gasit = false;
            for (unsigned int j = 0; j < secundar.marime_; ++j) {
                if (elemente_[i] == secundar.elemente_[j]) {
                    setComun << 1;          // Adauga 1 (true) in setComun pentru output
                    gasit = true;
                    break;
                }
            }

            if (!gasit) {
                return ClasaSet(); // Daca nu se gasesc elemente in comun > returneaza set gol (null)
            }
        }

        // Daca toate elementele au fost gasite > returneaza set comun ( cu elementele comune )
        return setComun;
    }

///////////////////////////////////////////////         Incluziune nestricta (supraincarcarea operatorului "<")
    ClasaSet operator<(const ClasaSet& secundar) const {
        if (secundar.marime_ == marime_) {
            return ClasaSet(); // Daca marimea primului set este egala cu marimea setului secundar > returneaza null
        }
        ClasaSet setComun;
        for (unsigned int i = 0; i < secundar.marime_; ++i) {
            bool gasit = false;
            for (unsigned int j = 0; j < marime_; ++j) {
                if (secundar.elemente_[i] == elemente_[j]) {
                    setComun << 1;          // Adauga 1 (true) in setComun pentru output
                    gasit = true;
                    break;
                }
            }
            if (!gasit) {
                return ClasaSet(); // Daca nu se gaseste element comun > returneaza null
            }
        }
        return setComun; // Returneaza 1 (= true)
    }
///////////////////////////////////////////////         Incluziune nestricta (supraincarcarea operatorului ">")
    ClasaSet operator>(const ClasaSet& secundar) const {
        if (marime_ == secundar.marime_) {
            return ClasaSet(); // Daca marimea primului set este egala cu marimea setului secundar > returneaza null
        }
        ClasaSet setComun;
        for (unsigned int i = 0; i < marime_; ++i) {
            bool gasit = false;
            for (unsigned int j = 0; j < secundar.marime_; ++j) {
                if (elemente_[i] == secundar.elemente_[j]) {
                    setComun << 1;              // Adauga 1 (true) in setComun pentru output
                    gasit = true;
                    break;
                }
            }
            if (!gasit) {
                return ClasaSet(); // Daca nu se gaseste element comun > returneaza null
            }
        }
        return setComun; // Returneaza 1 (= true)
    }
///////////////////////////////////////////////          Conversie (supraincarcarea operatorului "=")
    ClasaSet& operator=(const ClasaSet& secundar) {
        if (this != &secundar) {             // verificare daca s1 != set2
            delete[] elemente_;              // Sterge alocarea memoriei din set
            marime_ = secundar.marime_;      // Copiaza marimea noua
            elemente_ = new int[marime_];    // Aloca memoria noua 
            for (unsigned int i = 0; i < marime_; ++i) {
                elemente_[i] = secundar.elemente_[i];   // Copiaza elementele
            }
        }
        return *this; // Returneaza referinta la setul modificat
    }

///////////////////////////////////////////////         Egalitate (supraincarcarea operatorului "==")
    ClasaSet operator==(const ClasaSet& secundar) const {
        ClasaSet setComun;
        if (marime_ != secundar.marime_) {    // Daca marimea set1 != set2  > returneaza null
            return ClasaSet();
        }

        for (unsigned int i = 0; i < marime_; ++i) {
            bool gasit = false;
            for (unsigned int j = 0; j < secundar.marime_; ++j) {
                if (elemente_[i] == secundar.elemente_[j]) {            // Verifica daca elementele din s1 sunt egale cu cele din s2
                    setComun << 1;                                      // Adauga 1 (true) in setComun pentru output
                    gasit = true;
                    break;
                }
            }
            if (!gasit) {
                return ClasaSet();   // Daca nu se gaseste element comun > returneaza null
            }
        }

        return setComun;   // Returneaza 1 (true) 
    }

    void print() const {
        // sorteazaeaza elementele inainte de printare
        for (unsigned int i = 0; i < marime_ - 1 && i; ++i) {
            for (unsigned int j = 0; j < marime_ - i - 1; ++j) {
                if (elemente_[j] > elemente_[j + 1]) {
                    int temporar = elemente_[j];
                    elemente_[j] = elemente_[j + 1];
                    elemente_[j + 1] = temporar;
                }
            }
        }

        // Printeaza elementele
        for (unsigned int i = 0; i < marime_; ++i) {
                std::cout << elemente_[i] << " ";
            }
        if (elemente_ == nullptr) {
            std::cout << "null" << std::endl;
        } else {
            std::cout << std::endl;
        }
    }
};

int main() {
    ClasaSet set1;
    set1 << -4 << 0 << 1 << 2 << -2 << 99; // Adaugarea elementelor in set1 ( operatorul "<<" )
    set1 >> -4; // Eliminarea elementelor din set1 ( operatorul ">>" )
    std::cout << "Set1: ";
    set1.print(); 

    ClasaSet set2;
    set2 << 1 << 0 << -4; // Adaugarea elementelor in set2 ( operatorul "<<" )
    set2 >> -4; // Eliminarea elementelor din set2 ( operatorul ">>" )
    std::cout << "Set2: ";
    set2.print();

    ClasaSet setReuniune1 = set1 + set2; // Reuniune ( set1 + set2 )   // operatorul "+"
    std::cout << "Reuniune: "; 
    setReuniune1.print();

    ClasaSet setReuniune2 = set2 + set1; // Reuniune ( set2 + set1 )
    std::cout << "Reuniune2: "; 
    setReuniune2.print();

    ClasaSet setIntersectie1 = set1 * set2; // Intersectie ( set1 cu set2 )  // operatorul "*"
    std::cout << "Intersectie: ";
    setIntersectie1.print();

    ClasaSet setIntersectie2 = set2 * set1; // Intersectie ( set2 cu set1 )
    std::cout << "Intersectie2: ";
    setIntersectie2.print();

    ClasaSet setDiferenta1 = set1 - set2; // Diferenta ( ce este in set1 si nu este in set2 )   // operatorul "-"
    std::cout << "Diferenta: ";
    setDiferenta1.print();

    ClasaSet setDiferenta2 = set2 - set1; // Diferenta ( ce este in set2 si nu este in set1 )
    std::cout << "Diferenta2: ";
    setDiferenta2.print();

    ClasaSet setIncluziune1 = set1 >= set2; // Incluziune nestricta  ( set1 in set2 )    // operatorul ">="
    std::cout << "Incluziune nestricta set1 >= set2: ";
    setIncluziune1.print();

    ClasaSet setIncluziune2 = set2 <= set1; // Incluziune nestricta ( set1 in set2 - inversa )  // operatorul "<="
    std::cout << "Incluziune nestricta set2 <= set1: ";
    setIncluziune2.print();

    ClasaSet setIncluziune3 = set2 >= set1; // Inlcuziune nestricta ( set2 in set1 )
    std::cout << "Incluziune nestricta set2 >= set1: ";
    setIncluziune3.print();

    ClasaSet setIncluziune4 = set1 <= set2; // Inlcuziune nestricta ( set2 in set1 - invers )
    std::cout << "Incluziune nestricta set1 <= set2: ";
    setIncluziune4.print();

    ClasaSet setIncluziuneStricta1 = set1 > set2; // Incluziune stricta ( set1 in set2 )   // operatorul ">"
    std::cout << "Incluziune stricta set1 > set2: ";
    setIncluziuneStricta1.print();

    ClasaSet setIncluziuneStricta2 = set2 < set1; // Incluziune stricta ( set1 in set2 - invers )  // operatorul "<"
    std::cout << "Incluziune stricta set2 < set1: ";
    setIncluziuneStricta2.print();

    ClasaSet setIncluziuneStricta3 = set2 > set1; // Incluziune stricta ( set2 in set1 )
    std::cout << "Incluziune stricta set2 > set1: ";
    setIncluziuneStricta3.print();

    ClasaSet setIncluziuneStricta4 = set1 < set2; // Incluziune stricta ( set2 in set1 - invers )
    std::cout << "Incluziune stricta set1 < set2: ";
    setIncluziuneStricta4.print();

    ClasaSet verificare = set1 == set2; // Verificare egalitate ( daca set1 este egal cu set2 )  // operatorul "=="
    std::cout << "VERIFICARE: ";
    verificare.print();

    ClasaSet egalitate1 = set1 = set2; // Seteaza set1 sa fie egal cu set2 ( elementele lui set1 vor fi cele din set2 ) // operatorul "="
    std::cout << "Noul set1 este: ";
    egalitate1.print();

    // ClasaSet egalitate = set2 = set1; // Seteaza set2 sa fie egal cu set1 ( elementele lui set2 vor fi cele din set1 ) // operatorul "="
    // std::cout << "Noul set2 este: ";
    // egalitate.print();

    return 0;
}
