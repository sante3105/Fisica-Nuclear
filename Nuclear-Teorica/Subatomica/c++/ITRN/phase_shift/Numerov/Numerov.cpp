#include "Numerov.h"
#include <cmath>
#include <stdexcept>
#include "../Potential/Potential.h"
#include "../constants.h"

// Aunque l es un número cuántico entero, se deja como double
// porque la librería GSL (y otras funciones especiales de Coulomb) lo requieren así.
double f(double r, double l, double E, const Potential& pot, double mua = 0.0);

double f(double r, double l, double E, const Potential& pot, double mua) {
    double Veff = pot(r, l, 0.0);
    return -(2.0 * mu / (hbar * hbar)) * (Veff - E);
}

std::pair<std::vector<double>, std::vector<double>> numerov(double l, double E, double a, const Potential& pot, double mua) {
    double h = 0.001;

    std::vector<double> r;
    for (double val = h; val < a + 2*h + 1e-12; val += h) {
        r.push_back(val);
    }

    std::vector<double> u(r.size());
   std::vector<double> du(r.size() - 1);  // du tiene un punto menos que u (discretización de derivadas)

   for (size_t i =0; i < r.size(); ++i){
        if (i == 0) {
            u[i] = 0.0;
        } else if (i == 1) {
            u[i] = h;
        } else {
            double f_i = f(r[i], l, E, pot);
            double f_im1 = f(r[i - 1], l, E, pot);
            double f_im2 = f(r[i - 2], l, E, pot);

            double denom = 1.0 + (h*h / 12.0) * f_i;
            double num = 2.0 * u[i - 1] * (1 - (5 * h*h / 12.0) * f_im1) -
                         u[i - 2] * (1 + (h*h / 12.0) * f_im2);

            u[i] = num / denom;
        }
    }


    // Derivadas centradas (du de tamaño r.size() - 1)
    for (size_t i = 1; i < du.size(); ++i) {
      if (i == 0){
	du[i] = (u[i + 1] - u[i])/h;
	  }else {
      
        du[i] = (u[i + 1] - u[i - 1]) / (2 * h);
    }
   }
      

    // du queda con N-1 elementos, así que du[0] = (u[2] - u[0])/2h
    // y du[N-2] = (u[N-1] - u[N-3])/2h

    return {u, du};
}
