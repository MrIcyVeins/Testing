#include <iostream>

#include "multime.hpp"

using std::cout;
using std::endl;

class complex
{
public:
    complex(double re_, double im_):
        re{re_},
        im{im_}
    {}
    complex(double re_):
        re{re_},
        im{0}
    {}
    complex():
        complex{0,0}
    {}
    bool operator==(const complex& other) const {
        return re == other.re && im == other.im;
    }
    friend std::ostream& operator<<(std::ostream& os, const complex& c);
private:
    double re;
    double im;
};

std::ostream& operator<<(std::ostream& os, const complex& c)
{
    os << c.re;
    if(c.im != 0) {
        os << " + i" << c.im;
    }
    return os;
}

int main()
{
    multime<int> A;
    A.input();
    multime c_mul{complex{7,13}};
    cout << A << endl;
    c_mul << complex{19};
    cout << c_mul << endl;

    return EXIT_SUCCESS;
}

