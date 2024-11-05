#ifndef MULTIME_H
#define MULTIME_H

#include <iostream>

using std::cout;
using std::endl;
using std::cin;

template<typename T>
class multime
{
public:
    multime();
    multime(const T& t);
    multime(const T&& t);
    multime(const multime&);
    multime(const multime&&);
    multime<T> operator =(const multime<T>& other);

    // functii necesare ca multime să fie folosită în range-based for
    T* begin() {
        return elemente;
    }
    T* end() {
        return elemente + nr_elemente;
    }

    void input();
    // Adăugare element în mulţime
    multime& operator <<(const T& el);
    
    // comparaţie între mulţimi
    bool operator==(const T& other) const;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, multime<U>& m);
private:
    void muta_n_spatiu_nou();
    T* elemente;
    size_t nr_elemente;
    size_t nr_spatiu_alocat;
};

template<typename U>
std::ostream& operator<< (std::ostream& os, multime<U>& m)
{
    // TODO: nicer display ...
    for(size_t i{0}; i < m.nr_elemente; ++i) {
        cout << '"' << m.elemente[i] << '"';
        if(i < m.nr_elemente - 1) {
            cout << ' ';
        }
    }
    return os;
}

// constructor fără niciun argument
template<typename T>
multime<T>::multime():
    nr_elemente{0},
    nr_spatiu_alocat{0},
    elemente{nullptr}
{}

// constructor de copirer
template<typename T>
multime<T>::multime(const multime<T>& other):
    elemente{new T[other.nr_spatiu_alocat]},
    nr_elemente{other.nr_elemente},
    nr_spatiu_alocat{other.nr_spatiu_alocat}
{
    size_t i{};
    for(const T& el: other) {
        elemente[i] = el;
        i++;
    }
}

// constructor de mutare
template<typename T>
multime<T>::multime(const multime<T>&& other):
    elemente{other.elemente},
    nr_elemente{other.nr_elemente},
    nr_spatiu_alocat{other.nr_spatiu_alocat}
{
}

// constructor cu un singur element
template<typename T>
multime<T>::multime(const T& t):
    elemente{new T[1]},
    nr_elemente{1},
    nr_spatiu_alocat{1}
{
    elemente[0] = t;
}

// constructor care foloseşte un singur element
// când acesta nu mai poate fi folosit în program
template<typename T>
multime<T>::multime(const T&& t):
    elemente{new T[1]},
    nr_elemente{1},
    nr_spatiu_alocat{1}
{
    elemente[0] = t;
}

// operator de atribuire
template<typename T>
multime<T> multime<T>::operator=(const multime<T>& other)
{
    T* nou_spatiu = new T[other.nr_spatiu_alocat]; // ridică excepţie în caz de incapacitate
    delete elemente;
    elemente = nou_spatiu;
    size_t i{};
    for(const T& el: other) {
        elemente[i] = el;
        i++;
    }
    nr_elemente = other.nr_elemente;
    nr_spatiu_alocat = other.nr_spatiu_alocat;

    return this;
}

// comparaţie între mulţimi
template<typename T>
bool multime<T>::operator==(const T& other) const
{
    if(nr_elemente != other.nr_elemente) {
        return false;
    }

    for(size_t i{}; i < nr_elemente; ++i) {
        if(elemente[i] != other.elemente[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
void multime<T>::input()
{
    T el;
    cin >> el;
    while (el) {
        if(nr_elemente == nr_spatiu_alocat) {
            muta_n_spatiu_nou();
        }
        elemente[nr_elemente] = el;
        nr_elemente++;
    }
}

// adăugare element
template<typename T>
multime<T>& multime<T>::operator <<(const T& el_nou)
{
    for(size_t i{}; i < nr_elemente; ++i) {
        if(elemente[i] == el_nou) {
            return *this;
        }
    }

    if(nr_elemente == nr_spatiu_alocat) {
        muta_n_spatiu_nou();
    }

    elemente[nr_elemente] = el_nou;
    nr_elemente++;

    return *this;
}

// functie privată...
template<typename T>
void multime<T>::muta_n_spatiu_nou()
{
    if(nr_spatiu_alocat == 0) {
        nr_spatiu_alocat = 1;
    }

    T* nou_spatiu = new T[nr_spatiu_alocat * 2];
    for(size_t i {0}; i < nr_elemente; i++) {
        nou_spatiu[i] = std::move(elemente[i]);
    }

    delete[] elemente;
    elemente = nou_spatiu;
    nr_spatiu_alocat *= 2;
}

#endif // MULTIME_H

