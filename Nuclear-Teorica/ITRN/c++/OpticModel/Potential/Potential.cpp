#include <cmath>
#include <complex>
#include "Potential.h"
#include "../constants.h"

Potential::Potential() {}

Complex Potential::nuclear(double r) const {
    if (r < R_N) {
        return Complex(V_r, V_i);  // Potencial complejo constante
    } else {
        return Complex(0.0, 0.0);  // No hay potencial nuclear fuera del núcleo
    }
}

double Potential::coulomb(double r) const {
    if (r >= R_N) {
        return (Z_P * Z_T * e2) / r;  // Potencial de Coulomb fuera del núcleo
    } else {
        return 0.0;  // Nada dentro del núcleo
    }
}

// Potencial angular (siempre real)
double Potential::angular(double r, int l, double mua) const {
    double constant;
    if (mua == 0.0) {
        constant = (hbar * hbar) / (2.0 * mu);
    } else {
        constant = 20.736 / mua;
    }
    if (l == 0) return 0.0;
    return constant * l * (l + 1) / (r * r);
}

// Operador() retorna potencial complejo total
Complex Potential::operator()(double r, int l, double mua) const {
    Complex V_nuc = nuclear(r);
    double V_coul = coulomb(r);
    double V_ang = angular(r, l, mua);
    
    // Suma: parte compleja (nuclear) + partes reales (Coulomb + angular)
    return V_nuc + Complex(V_coul + V_ang, 0.0);
}

// Método adicional para obtener solo la parte real (compatibilidad)
double Potential::real_part(double r, int l, double mua) const {
    Complex V_total = (*this)(r, l, mua);
    return V_total.real();
}

// Método adicional para obtener solo la parte imaginaria
double Potential::imag_part(double r, int l, double mua) const {
    Complex V_total = (*this)(r, l, mua);
    return V_total.imag();
}
