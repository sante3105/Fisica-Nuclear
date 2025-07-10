#include <cmath>
#include <complex>
#include "Potential.h"
#include "../constants.h"
#include <stdexcept>

// PARÁMETRO DE CONFIGURACIÓN - CAMBIAR AQUÍ
// 1: Potencial del primer código (Gaussiano + Coulomb con erf)
// 2: Potencial del segundo código (Cuadrado + Coulomb simple)
const int POTENTIAL_TYPE = 2;

Potential::Potential() {}

Complex Potential::nuclear(long double r) const {
    if (POTENTIAL_TYPE == 1) {
        // Potencial tipo 1: Gaussiano (convertido a complejo)
        long double V_nuc = V0 * std::exp(-std::pow(r / a_p, 2));
        return Complex(V_nuc, 0.0);
    } else {
        // Potencial tipo 2: Cuadrado complejo
        if (r < R_N) {
            return Complex(V_r, V_i);
        } else {
            return Complex(0.0, 0.0);
        }
    }
}

long double Potential::coulomb(long double r) const {
    if (POTENTIAL_TYPE == 1) {
        // Potencial tipo 1: Coulomb con función de error
        if (r == 0.0) {
            throw std::invalid_argument("r must be non-zero to avoid division by zero");
        }
        return (Z_P * Z_T) * e2 / r;
    } else {
        // Potencial tipo 2: Coulomb simple fuera del núcleo
        if (r >= R_N) {
            return (Z_P * Z_T * e2) / r;
        } else {
            return 0.0;
        }
    }
}

long double Potential::angular(long double r, int l, long double mua) const {
    long double constant;
    if (mua == 0.0) {
        constant = (hbar * hbar) / (2.0 * mu);
    } else {
        constant = 20.736 / mua;
    }
    if (l == 0) return 0.0;
    return constant * l * (l + 1) / (r * r);
}

// Operador() retorna potencial complejo total
Complex Potential::operator()(long double r, int l, long double mua) const {
    Complex V_nuc = nuclear(r);
    long double V_coul = coulomb(r);
    long double V_ang = angular(r, l, mua);
    
    // Suma: parte compleja (nuclear) + partes reales (Coulomb + angular)
    return V_nuc + Complex(V_coul + V_ang, 0.0);
}


// Método adicional para obtener solo la parte real (compatibilidad)
//double Potential::real_part(double r, int l, double mua) const {
  //Complex V_total = (*this)(r, l, mua);
    //return V_total.real();
    //}

// Método adicional para obtener solo la parte imaginaria
//double Potential::imag_part(double r, int l, double mua) const {
//  Complex V_total = (*this)(r, l, mua);
//  return V_total.imag();
//}
