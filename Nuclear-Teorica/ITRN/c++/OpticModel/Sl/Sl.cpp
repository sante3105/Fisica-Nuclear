#include "Sl.h"
#include "../constants.h"          // ← your file that defines mu, hbar, Sommerfeld
#include <cmath>
#include <stdexcept>
#include <complex>
#include <gsl/gsl_sf_coulomb.h>
#include <gsl/gsl_errno.h>

namespace
{
    constexpr double h = 0.001;    // radial mesh step (same step you used to build u & du)
}

// Función corregida: nombre y parámetro Sommerfeld
double Sommerfeld(double E){
    return alpha * Z_P * Z_T * std::sqrt(mu / (2 * E));
}

std::complex<double>
computeSl(const std::vector<std::complex<double>>& u,
          const std::vector<std::complex<double>>& du,
          double E, double a, double l)
{
    if (E <= 0.0)  throw std::runtime_error("computeSl: E must be > 0");
    if (a <= 0.0)  throw std::runtime_error("computeSl: a must be > 0");
    
    /* ----------- kinematics & Coulomb parameters ----------- */
    const double k   = std::sqrt(2.0 * mu * E) / hbar;   // wave number
    const double rho = k * a;
    const double eta = Sommerfeld(E);  // Nombre corregido
    
    /* ----------- locate mesh index closest to r = a -------- */
    const int idx = static_cast<int>(std::round(a / h)) - 1;  // Usar round() para mejor precisión
    if (idx < 0 || idx >= static_cast<int>(u.size()) - 1)
        throw std::runtime_error("computeSl: radius a lies outside the radial mesh");
    
    if (std::abs(u[idx + 1]) < 1e-10)
        throw std::runtime_error("computeSl: |u| too small at r = a");
    
     const std::complex<double> L = a * du[idx + 1] / u[idx + 1];   // logarithmic derivative
    
    /* ----------- Coulomb wave functions F, G and derivatives -------- */
    gsl_sf_result F, Fp, G, Gp;
    double exp_F, exp_G;
    int ell = static_cast<int>(std::lround(l));
    
    int status = gsl_sf_coulomb_wave_FG_e(
        eta, rho, ell, 0,
        &F, &Fp, &G, &Gp, &exp_F, &exp_G
    );
    
    if (status != GSL_SUCCESS)
        throw std::runtime_error("Error al evaluar funciones de Coulomb.");
    
    /* ----------- incoming / outgoing combinations H± -------- */
    const std::complex<double> i(0.0, 1.0);
    const std::complex<double> Hplus   =  i * F.val + G.val;
    const std::complex<double> Hminus  = -i * F.val + G.val;
    const std::complex<double> dHplus  =  i * Fp.val + Gp.val;
    const std::complex<double> dHminus = -i * Fp.val + Gp.val;
    
    /* ----------- single–channel S-matrix -------- */
    const std::complex<double> den = Hplus  - (k * a / L) * dHplus;
    const std::complex<double> num = Hminus - (k * a / L) * dHminus;
    
    if (std::abs(den) < 1e-30)
        throw std::runtime_error("computeSl: denominator ~ 0 – S_l undefined");
    
    return num / den;
}
