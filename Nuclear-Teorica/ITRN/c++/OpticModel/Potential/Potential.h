#ifndef POTENTIAL_H
#define POTENTIAL_H
#include <complex>

using Complex = std::complex<long double>;

class Potential {
public:
    Potential();
    
    // Potenciales individuales
    Complex nuclear(long double r) const;           // Ahora complejo
    long double coulomb(long double r) const;       // Siempre real
    long double angular(long double r, int l, long double mua = 0.0) const; // Siempre real
    
    // Potencial total complejo
    Complex operator()(long double r, int l = 0, long double mua = 0.0) const;
    
    // Métodos auxiliares para obtener partes real e imaginaria
    long double real_part(long double r, int l = 0, long double mua = 0.0) const;
    long double imag_part(long double r, int l = 0, long double mua = 0.0) const;
};

#endif
