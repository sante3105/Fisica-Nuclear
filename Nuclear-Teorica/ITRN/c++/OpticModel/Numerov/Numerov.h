#ifndef NUMEROV_H
#define NUMEROV_H
#include <vector>
#include <complex>

class Potential;
using Complex = std::complex<long double>;

// Estructura para resultados complejos
struct ComplexNumerovResult {
    std::vector<long double> r;
    std::vector<Complex> u;
    std::vector<Complex> du;
    
    // Métodos auxiliares para extraer partes reales e imaginarias
    //std::vector<double> u_real() const;
  // std::vector<double> u_imag() const;
  // std::vector<double> du_real() const;
  // std::vector<double> du_imag() const;
};

// Función principal para potenciales complejos
ComplexNumerovResult numerov_complex(long double l, long double E, long double a, const Potential& pot, long double mua = 0.0);

// Función de compatibilidad (solo parte real)
std::pair<std::vector<long double>, std::vector<long double>> numerov(long double l, long double E, long double a, const Potential& pot, long double mua = 0.0);

#endif
