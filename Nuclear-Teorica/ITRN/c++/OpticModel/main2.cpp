#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <limits>
#define _USE_MATH_DEFINES
#include <cmath>
#include <filesystem>

#include "Numerov/Numerov.h"
#include "Sl/Sl.h"
#include "Potential/Potential.h"
#include "constants.h"

namespace fs = std::filesystem;

int main()
{
    // ---------------------------------------------------------------------
    // 1) Parámetros de entrada
    double l, a;
    std::cout << "Ingrese valor de l (momento angular): ";
    std::cin  >> l;

    std::cout << "Canal de radio recomendado: " << R_N << " fm\n";
    std::cout << "Ingrese valor del radio del canal a [fm]: ";
    std::cin  >> a;

    // ---------------------------------------------------------------------
    // 2) Directorio de salida:  data/datfiles_l=<l>_a=<a>
    std::ostringstream dir_stream;
    dir_stream << "data/datfiles_l=" << l << "_a=" << a;
    const std::string data_dir = dir_stream.str();
    fs::create_directories(data_dir);

    const std::string filename = data_dir + "/delta_vs_Elab.dat";

    Potential pot;
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "No se pudo abrir " << filename << '\n';
        return 1;
    }

    out << "# E_lab [MeV]\t"
        << "delta_real [deg]\t"
        << "delta_imag [deg]\t"
        << "cross_section [b]\t"
        << "S_factor [MeV*b]\n";

    std::vector<double> phases_re, phases_im,
                        cross_sections, s_factors, energies_lab;

    // ---------------------------------------------------------------------
    // 3) Bucle en energía (E = CM, Elab = laboratorio)
    for (double E = 0.00012; E <= 0.168 + 1e-9; E += 0.00001)
    {
        const double Elab = E * (A_P + A_T) / A_T;      // CM → Lab

        try {
            auto [r, u, du] = numerov_complex(l, E, a, pot);

            std::complex<double> Sl = computeSl(u, du, E, a, l);

            /* ------------------------------------------------------------
               S_l  =  η_l · exp( 2 i δ_l )
               ⇒   δ_l       = ½·arg(S_l)
               ⇒   ρ_l (absorción) = −½·ln|S_l|   (si |S_l|≠1)
            ------------------------------------------------------------ */
            const double eta_l  = std::abs(Sl);
            const double delta  = 0.5 * std::arg(Sl);         // radianes
            const double rho_l  = -0.5 * std::log(eta_l);     // radianes

            const double k  = std::sqrt(2.0 * mu * E) / hbar; // fm⁻¹ (unidades coherentes)
            const double sigma_reac = (M_PI / (k*k)) * (2.0*l + 1.0)*(1.0 - eta_l*eta_l); // fm²

            /* Conversión  fm² → b (1 fm² = 10⁻² b) */
            const double sigma_b = sigma_reac * 1.0e-2;

            /* Astrofísica: S-factor (Breit-Wigner‐Gamow)              */
            const double BG = Z_P * Z_T * e2 * M_PI * std::sqrt(2.0 * mu) / hbar; // MeV¹⁄²
            const double S_factor = (sigma_b * E) / std::exp(-BG / std::sqrt(E)); // MeV·b

            phases_re   .push_back(delta);              // rad
            phases_im   .push_back(rho_l);              // rad
            cross_sections.push_back(sigma_b);          // b
            s_factors   .push_back(S_factor);           // MeV·b
            energies_lab.push_back(Elab);               // MeV
        }
        catch (const std::exception& e) {
            std::cerr << "Error en E = " << E << " MeV: " << e.what() << '\n';
            phases_re   .push_back(std::numeric_limits<double>::quiet_NaN());
            phases_im   .push_back(std::numeric_limits<double>::quiet_NaN());
            cross_sections.push_back(std::numeric_limits<double>::quiet_NaN());
            s_factors   .push_back(std::numeric_limits<double>::quiet_NaN());
            energies_lab.push_back(Elab);
        }
    }

    // ---------------------------------------------------------------------
    // 4) Escritura de resultados
    const std::size_t N = phases_re.size();
    for (std::size_t i = 0; i < N; ++i)
    {
        if (std::isnan(phases_re[i])) continue;   // punto fallido → lo saltamos

        const double delta_real_deg =  phases_re[i] * 180.0 / M_PI;
        const double delta_imag_deg =  phases_im[i] * 180.0 / M_PI;

        out << std::fixed << std::setprecision(6)
            << energies_lab[i]      << '\t'
            << delta_real_deg       << '\t'
            << delta_imag_deg       << '\t'
            << cross_sections[i]    << '\t'
            << s_factors[i]         << '\n';

        std::cout << "Elab = " << std::setw(9) << energies_lab[i] << " MeV  "
                  << "δ_re = "  << std::setw(9) << delta_real_deg << "°  "
                  << "δ_im = "  << std::setw(9) << delta_imag_deg << "°  "
                  << "σ_R = "   << std::setw(9) << cross_sections[i] << " b  "
                  << "S = "     << std::setw(11)<< s_factors[i] << " MeV·b\n";
    }

    std::cout << "\nArchivo generado: " << filename << '\n';
    return 0;
}
