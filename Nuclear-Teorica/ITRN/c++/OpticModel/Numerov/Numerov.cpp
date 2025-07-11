#include "Numerov.h"
#include <cmath>
#include <stdexcept>
#include <complex>
#include "../Potential/Potential.h"
#include "../constants.h"

// Función f ahora retorna complejo
Complex f_complex(long double r, long double l, long double E, const Potential& pot, long double mua = 0.0);

Complex f_complex(long double r, long double l, long double E, const Potential& pot, long double mua) {
    Complex Veff = pot(r, l, 0.0);  // Potencial complejo
    return -(2.0 * mu / (hbar * hbar)) * (Veff - Complex(E, 0.0));
}

ComplexNumerovResult numerov_complex(long double l, long double E, long double a, const Potential& pot, long double mua) {
    long double h = 0.001L;  // Use long double for consistency
    
    // Crear arreglo r
    std::vector<long double> r;
    for (long double val = h; val < a + 2*h + 1e-12L; val += h) {
        r.push_back(val);
    }
    
    std::vector<Complex> u(r.size());
    std::vector<Complex> du(r.size() - 1);
    
    // Algoritmo de Numerov para números complejos
    for (size_t i = 0; i < r.size(); ++i) {
        if (i == 0) {
            u[i] = Complex(0.0L, 0.0L);
        } else if (i == 1) {
            u[i] = Complex(h, 0.0L);  // Condición inicial real
        } else {
            Complex f_i = f_complex(r[i], l, E, pot, mua);
            Complex f_im1 = f_complex(r[i - 1], l, E, pot, mua);
            Complex f_im2 = f_complex(r[i - 2], l, E, pot, mua);
            
            Complex denom = Complex(1.0L, 0.0L) + Complex(h*h / 12.0L, 0.0L) * f_i;
            Complex num = Complex(2.0L, 0.0L) * u[i - 1] * (Complex(1.0L, 0.0L) - Complex(5.0L * h*h / 12.0L, 0.0L) * f_im1) -
                         u[i - 2] * (Complex(1.0L, 0.0L) + Complex(h*h / 12.0L, 0.0L) * f_im2);
            u[i] = num / denom;
        }
    }
    
    for (size_t i = 0; i < du.size(); ++i) {
        if (i == 0) {
            // Forward difference
	  du[i] = (u[i + 1] - u[i]) / Complex(h , 0);
        } else if (i == du.size() - 1) {
            // Backward difference
	  du[i] = (u[i] - u[i - 1]) / Complex(h ,  0);
        } else {
            // Central difference
	  du[i] = (u[i + 1] - u[i - 1]) / Complex((2.0L * h) , 0);
        }
    }
    
    return {r, u, du};
}
// Función de compatibilidad que retorna solo las partes reales
//std::pair<std::vector<double>, std::vector<double>> numerov(double l, double E, double a, const Potential& pot, double mua) {
//    auto result = numerov_complex(l, E, a, pot, mua);
//  return {result.u_real(), result.du_real()};
//}
