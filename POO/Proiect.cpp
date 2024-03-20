// Cerinta
// 7.	Idem pentru mulţimi finite de elemente de un tip neprecizat, cu supraîncărcarea operatorilor =, ==, <, <=, (incluziunea strictă/nestrictă), >, >= (relaţiile duale); + (reuniunea); * (intersecţia); - (binar, diferenţa de mulţimi); <<, >> (adăugarea/ extragerea unui element, care este al doilea operand, la/dintr-o mulţime, reprezentată de primul operand) şi conversia unui element la mulţimea care îl are ca unic element. Multimile se reprezintă printr-un întreg fără semn care este numărul elementelor (pentru mulţimea vidă acest numar va fi 0) şi printr-un vector (adică pointer la tipul elementelor) care conţine elementele. Metodele de creare şi modificare a mulţimilor vor asigura ca fiecare element să apară o singură dată în acest vector.

#include <iostream>

class MySet {
private:
    unsigned int size_; // numarul elementelor reprezentat de variabila size_ (cu valoarea "unsigned int" = poate stoca doar valoare pozitiva si zero)
    int* elements_; // vector (pointer la un intreg) care va contine elementele ( zona de memorie unde sunt stocate elementele ) 

public:
    // Constructor
    // Constructorul "MySet()" va fi apelat atunci cand se creaza o instanta (obiect) a clasei MySet
    // Initializarea variabilelor membre (size_ si elements_) se face in felul urmator: size_ va lua valoarea "0" si elements_ "nullptr" ( nu pointeaza catre nici o zona de memorie // este un pointer null )
    MySet() : size_(0), elements_(nullptr) {} 

//    // Destructor
//   ~MySet() {
//        delete[] elements_;
//    }

    // Adaugare element in set (supraincarcarea operatorului "<<")
    //
    MySet& operator<<(int value) {
        if (!contains(value)) {
            int* newElements = new int[size_ + 1];
            for (unsigned int i = 0; i < size_; ++i) {
                newElements[i] = elements_[i];
            }
            newElements[size_] = value;
            delete[] elements_;
            elements_ = newElements;
            ++size_;
        }
        return *this;
    }

    // Verificarea daca elementul exista deja in set sau nu 
    bool contains(int value) const {
        for (unsigned int i = 0; i < size_; ++i) {
            if (elements_[i] == value) {
                return true;
            }
        }
        return false;
    }

    // Reuniunea a doua seturi (supraincarcarea operatorului "+")
    MySet operator+(const MySet& other) const {
        MySet result = *this;
        for (unsigned int i = 0; i < other.size_; ++i) {
            result << other.elements_[i];
        }
        return result;
    }

    // Intersectia a doua seturi (supraincarcarea operatorului "*")
    MySet operator*(const MySet& other) const {
        MySet result;
        for (unsigned int i = 0; i < size_; ++i) {
            if (other.contains(elements_[i])) {
                result << elements_[i];
            }
        }
        return result;
    }

    // Diferenta dintre doua seturi (supraincarcarea operatorului "-")
    MySet operator-(const MySet& other) const {
        MySet result = *this;
        for (unsigned int i = 0; i < other.size_; ++i) {
            if (result.contains(other.elements_[i])) {
                result.remove(other.elements_[i]);
            }
        }
	if (result.size_ == 0) {
             std::cout << "Nu exista diferenta.\n";
	}
        return result;
    }

    // Printeaza setul
    void print() const {
        for (unsigned int i = 0; i < size_; ++i) {
            std::cout << elements_[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    // Scoate un element din set
    void remove(int value) {
        for (unsigned int i = 0; i < size_; ++i) {
            if (elements_[i] == value) {
                for (unsigned int j = i; j < size_ - 1; ++j) {
                    elements_[j] = elements_[j + 1];
                }
                --size_;
                break;
            }
        }
    }
};

int main() {
    MySet set1;
    set1  << 0 << 1  << 2 << 3 << 4 << 5 << 6; // Adaugarea elementelor in set1
    std::cout << "Set1: ";
    set1.print(); // Printeaza set1

    MySet set2;
    set2 << 3 << 2 << 1; // Adaugarea elementelor in set2
    std::cout << "Set2: ";
    set2.print(); // Printeaza set2

    MySet unionSet = set1 + set2; // Reuniune
    std::cout << "Reuniune: "; 
    unionSet.print();

    MySet intersectionSet = set1 * set2; // Intersectie
    std::cout << "Intersectie: ";
    intersectionSet.print();

    MySet differenceSet = set1 - set2; // Diferenta
    std::cout << "Diferenta: ";
    differenceSet.print();

    return 0;
}
