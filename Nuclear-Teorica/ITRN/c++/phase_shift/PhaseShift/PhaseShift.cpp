#include "PhaseShift.h"
#include <cmath>
#include <stdexcept>
#include <gsl/gsl_sf_coulomb.h>
#include <gsl/gsl_errno.h>
#include "../constants.h"

double Sommerfield(double E){
    return alpha * Z_P * Z_T * std::sqrt(mu / (2 * E));
}

double computePhaseShift(const std::vector<double>& u, const std::vector<double>& du, double E, double a, double l) {
    double h = 0.001;
    double k = std::sqrt(2 * mu * E) / hbar;
    double eta = Sommerfield(E);
    double rho = k * a;

    int index = static_cast<int>(a / h) - 1; // más seguro
    if (index < 0 || index >= static_cast<int>(du.size())){
        throw std::runtime_error("Índice inválido para derivada logarítmica.");
	}
    if (std::abs(u[index +1]) < 1e-10){
        throw std::runtime_error("Valor de u demasiado cercano a cero para derivada logarítmica.");
		}
    double L = a * du[index] / u[index + 1];

    gsl_sf_result F, Fp, G, Gp;
    double exp_F, exp_G;
    int ell = static_cast<int>(std::lround(l));
    int status = gsl_sf_coulomb_wave_FG_e(
        eta, rho, ell, 0,
        &F, &Fp, &G, &Gp, &exp_F, &exp_G
    );

    if (status != GSL_SUCCESS)
        throw std::runtime_error("Error al evaluar funciones de Coulomb.");

    double numerator = -(rho * Fp.val - F.val * L);
    double denominator = rho * Gp.val - G.val * L;

    double Sl = numerator / denominator;
    double delta_rad = std::atan(Sl);
    
    return delta_rad; // en radianes
}
