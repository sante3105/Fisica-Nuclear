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
#include "PhaseShift/PhaseShift.h"
#include "Potential/Potential.h"
#include "constants.h"

namespace fs = std::filesystem;

void unwrapPhases(std::vector<double>& phases) {
    if (phases.size() < 2) return;
    for (size_t k = 1; k < phases.size(); ++k) {
        double diff = phases[k] - phases[k-1];
        if (diff > 1.5) {
            for (size_t i = 0; i < k; ++i) phases[i] += M_PI;
        } else if (diff < -1.5) {
            for (size_t i = k; i < phases.size(); ++i) phases[i] += M_PI;
        }
    }
}

int main() {
    // ---------------------------------------------------------------------
    // Input parameters
    double l, a;
    std::cout << "Ingrese valor de l (momento angular): ";
    std::cin >> l;

    // Mostrar radio de canal recomendado
    std::cout << "Canal de radio recomendado: " << R_N << " fm\n";
    std::cout << "Ingrese valor del radio del canal a [fm]: ";
    std::cin >> a;

    // ---------------------------------------------------------------------
    // Prepare directory: data/datfiles_l=<l>_a=<a>
    std::ostringstream dir_stream;
    dir_stream << "data/datfiles_l=" << l << "_a=" << a;
    std::string data_dir = dir_stream.str();
    fs::create_directories(data_dir);

    // Output filename (without l, a)
    const std::string filename = data_dir + "/delta_vs_Elab.dat";

    Potential pot;
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "No se pudo abrir " << filename << "\n";
        return 1;
    }

    out << "# E_lab [MeV]\tdelta_real [deg]\tdelta_imag [deg]\tcross_section [mb]\tS_factor [MeV*mb]\n";

    std::vector<double> all_phases_real;
    std::vector<double> all_phases_imag;
    std::vector<double> all_cross_sections;
    std::vector<double> all_s_factors;
    std::vector<double> energies_lab;

    for (double E = 0.00012; E <= 0.168 + 1e-9; E += 0.00001) {
        double Elab = E * (A_P + A_T) / A_T; // CM ➔ Lab
        try {
            auto [r, u, du] = numerov_complex(l, E, a, pot);
            std::complex<double> delta_complex = computePhaseShift(u, du, E, a, l);

            double delta_real = delta_complex.real();
            double delta_imag = delta_complex.imag();

            double k = std::sqrt(2 * mu * E) / hbar;
            std::complex<double> exp_2i_delta = std::exp(2.0 * std::complex<double>(0, 1) * delta_complex);
            double cross_section = (M_PI / (k * k)) * (1.0 - std::norm(exp_2i_delta));

            double BG = Z_P * Z_T * e2 * M_PI * std::sqrt(2.0 * mu) / hbar;
            double s_factor = (cross_section * E) / std::exp(-BG / std::sqrt(E));

            all_phases_real.push_back(delta_real);
            all_phases_imag.push_back(delta_imag);
            all_cross_sections.push_back(cross_section);
            all_s_factors.push_back(s_factor);
            energies_lab.push_back(Elab);
        } catch (const std::exception &e) {
            std::cerr << "Error en E = " << E << " MeV: " << e.what() << '\n';
            all_phases_real.push_back(std::numeric_limits<double>::quiet_NaN());
            all_phases_imag.push_back(std::numeric_limits<double>::quiet_NaN());
            all_cross_sections.push_back(std::numeric_limits<double>::quiet_NaN());
            all_s_factors.push_back(std::numeric_limits<double>::quiet_NaN());
            energies_lab.push_back(Elab);
        }
    }

    // Desenrollar fases
    unwrapPhases(all_phases_real);

    // ---------------------------------------------------------------------
    // Write results
    for (size_t i = 0; i < all_phases_real.size(); ++i) {
        if (!std::isnan(all_phases_real[i])) {
            double delta_real_deg = all_phases_real[i] * 180.0 / M_PI;
            double delta_imag_deg = all_phases_imag[i] * 180.0 / M_PI;
            double cross_section_mb = all_cross_sections[i]; //* 1000.0;

            out << std::fixed << std::setprecision(4)
                << energies_lab[i] << '\t'
                << delta_real_deg << '\t'
                << delta_imag_deg << '\t'
                << cross_section_mb << '\t'
                << all_s_factors[i] << '\n';

            std::cout << "Elab = " << energies_lab[i] << " MeV\t"
                      << "δ_real = " << delta_real_deg << "°\t"
                      << "δ_imag = " << delta_imag_deg << "°\t"
                      << "σ = " << cross_section_mb << " mb\t"
                      << "S = " << all_s_factors[i] << " MeV*mb\n";
        }
    }

    std::cout << "Archivo generado: " << filename << '\n';
    return 0;
}
