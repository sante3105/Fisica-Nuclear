#ifndef POTENTIAL_H
#define POTENTIAL_H
#include <complex>

using Complex = std::complex<double>;

class Potential {
public:
    Potential();
    
    // Potenciales individuales
    Complex nuclear(double r) const;           // Ahora complejo
    double coulomb(double r) const;            // Siempre real
    double angular(double r, int l, double mua = 0.0) const; // Siempre real
    
    // Potencial total complejo
    Complex operator()(double r, int l = 0, double mua = 0.0) const;
    
    // Métodos auxiliares para obtener partes real e imaginaria
    double real_part(double r, int l = 0, double mua = 0.0) const;
    double imag_part(double r, int l = 0, double mua = 0.0) const;
};

#endif
