#include "PhaseShift.h"
#include <cmath>
#include <stdexcept>
#include <complex>
#include <gsl/gsl_sf_coulomb.h>
#include <gsl/gsl_errno.h>
#include "../constants.h"

double Sommerfield(double E){
    return alpha * Z_P * Z_T * std::sqrt(mu / (2 * E));
}

std::complex<double> computePhaseShift(const std::vector<std::complex<double>>& u, 
                                       const std::vector<std::complex<double>>& du, 
                                       double E, double a, double l) {
    double h = 0.001;
    double k = std::sqrt(2 * mu * E) / hbar;
    double eta = Sommerfield(E);
    double rho = k * a;
    int index = static_cast<int>(a / h) - 1;
    
    if (index < 0 || index >= static_cast<int>(du.size())){
        throw std::runtime_error("Índice inválido para derivada logarítmica.");
    }
    
    if (std::abs(u[index + 1]) < 1e-10){
        throw std::runtime_error("Valor de u demasiado cercano a cero para derivada logarítmica.");
    }
    
    // Complex logarithmic derivative
    std::complex<double> L = a * du[index] / u[index + 1];
    
    // Coulomb functions (still real)
    gsl_sf_result F, Fp, G, Gp;
    double exp_F, exp_G;
    int ell = static_cast<int>(std::lround(l));
    int status = gsl_sf_coulomb_wave_FG_e(
        eta, rho, ell, 0,
        &F, &Fp, &G, &Gp, &exp_F, &exp_G
    );
    
    if (status != GSL_SUCCESS)
        throw std::runtime_error("Error al evaluar funciones de Coulomb.");
    
    // Complex calculation
    std::complex<double> numerator = -(rho * Fp.val - F.val * L);
    std::complex<double> denominator = rho * Gp.val - G.val * L;
    std::complex<double> Sl = numerator / denominator;
    
    // Complex phase shift using complex atan
    std::complex<double> delta_complex = std::atan(Sl);
    
    return delta_complex;
}
