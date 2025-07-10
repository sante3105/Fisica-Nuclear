#include "PhaseShift.h"
#include <cmath>
#include <stdexcept>
#include <complex>
#include <gsl/gsl_sf_coulomb.h>
#include <gsl/gsl_errno.h>
#include "../constants.h"

 long double Sommerfield( long double E){
    return alpha * Z_P * Z_T * std::sqrt(mu / (2 * E));
}

std::complex< long double> computeWrWi(const std::vector<std::complex<long double>>& u, 
                                const std::vector<std::complex<long double>>& du, 
                                 long double E,  long double a,  long double l) {
     long double h = 0.001;
     long double k = std::sqrt(2 * mu * E) / hbar;
     long double eta = Sommerfield(E);
     long double rho = k * a;
      int index = static_cast<int>(a / h) - 1;
    
    if (index < 0 || index >= static_cast<int>(du.size())){
        throw std::runtime_error("Índice inválido para derivada logarítmica.");
    }
    
    if (std::abs(u[index + 1]) < 1e-10){
        throw std::runtime_error("Valor de u demasiado cercano a cero para derivada logarítmica.");
    }
    
    // Complex logarithmic derivative
    std::complex<long double> L = static_cast<long double>(a) * 
                                 std::complex<long double>(du[index].real(), du[index].imag()) / 
      std::complex<long double>(u[index + 1].real(), u[index + 1].imag());
    
    // Coulomb functions (still real)
    gsl_sf_result F, Fp, G, Gp;
    double exp_F, exp_G;
    int ell = static_cast<int>(std::lround(l));
   int status = gsl_sf_coulomb_wave_FG_e(
        static_cast<double>(eta), static_cast<double>(rho), ell, 0,
        &F, &Fp, &G, &Gp, &exp_F, &exp_G
    );
    
    if (status != GSL_SUCCESS)
        throw std::runtime_error("Error al evaluar funciones de Coulomb.");
    
    // Complex calculation with long double precision
    std::complex<long double> F_val(static_cast<long double>(F.val), 0.0L);
    std::complex<long double> Fp_val(static_cast<long double>(Fp.val), 0.0L);
    std::complex<long double> G_val(static_cast<long double>(G.val), 0.0L);
    std::complex<long double> Gp_val(static_cast<long double>(Gp.val), 0.0L);
    
    std::complex<long double> numerator = -(rho * Fp_val - F_val * L);
    std::complex<long double> denominator = rho * Gp_val - G_val * L;
    std::complex<long double> cot_delta = denominator / numerator;

    return cot_delta;
}
