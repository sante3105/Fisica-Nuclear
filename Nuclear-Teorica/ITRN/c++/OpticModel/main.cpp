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

int main() {
    // ---------------------------------------------------------------------
    // Input parameters
    long double l, a;
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

    // Output filenames
    const std::string filename = data_dir + "/WrWi_vs_Elab.dat";
    const std::string L_filename = data_dir + "/L_vs_Elab.dat";

    Potential pot;
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "No se pudo abrir " << filename << "\n";
        return 1;
    }

    std::ofstream L_out(L_filename);
    if (!L_out) {
        std::cerr << "No se pudo abrir " << L_filename << "\n";
        return 1;
    }

    out << "# E_lab [MeV]\tWr\tWi\tcross_section [mb]\tS_factor [MeV*mb]\n";
    L_out << "# E_lab [MeV]\tL_real\tL_imag\t|L|\n";

    std::vector<long double> all_Wr;
    std::vector<long double> all_Wi;
    std::vector<long double> all_cross_sections;
    std::vector<long double> all_s_factors;
    std::vector<long double> energies_lab;
    std::vector<std::complex<long double>> all_L_values;

    for (long double E = 0.0001L; E <= 0.168L + 1e-9L; E += 0.0001L) {
        long double Elab = E * (A_P + A_T) / A_T; // CM ➔ Lab
        try {
            auto [r, u, du] = numerov_complex(l, E, a, pot);
            std::complex<long double> WrWi_complex = computeWrWi(u, du, E, a, l);

            long double Wr = WrWi_complex.real();
            long double Wi = WrWi_complex.imag();

            long double k = std::sqrt(2 * mu * E) / hbar;
            
            // Nueva fórmula para la sección eficaz: π/k² * [4Wi/(Wr² + Wi²)]
            long double W_squared = Wr * Wr + (Wi -1) * (Wi -1);
            long double cross_section = 0.0L;
            
            if (W_squared > 1e-40L) {  // Evitar división por cero con threshold más bajo
                cross_section = (M_PI / (k * k)) * (-4.0L * Wi / W_squared);
            }
	    std::cout<<M_PI<<'\t';
	    std::cout<<k*k<<'\n';
            
            // Para valores muy pequeños, usar valor absoluto
            if (cross_section < 0.0L) {
                cross_section = std::abs(cross_section);
            }

            long double BG = Z_P * Z_T * e2 * M_PI * std::sqrt(2.0L * mu) / hbar;
            long double s_factor = (cross_section * E) / std::exp(-BG / std::sqrt(E));

            // Encontrar el índice correspondiente a r = a
            size_t idx_a = 0;
            long double min_diff = std::abs(r[0] - static_cast<double>(a));
            for (size_t i = 1; i < r.size(); ++i) {
                long double diff = std::abs(r[i] - static_cast<double>(a));
                if (diff < min_diff) {
                    min_diff = diff;
                    idx_a = i;
                }
            }

            // Calcular L = a * du(a) / u(a) usando el índice correcto
            std::complex<long double> u_at_a = u[idx_a];
            std::complex<long double> du_at_a = du[idx_a];
            std::complex<long double> L_complex = static_cast<long double>(a) * du_at_a / u_at_a;

            all_Wr.push_back(Wr);
            all_Wi.push_back(Wi);
            all_cross_sections.push_back(cross_section);
            all_s_factors.push_back(s_factor);
            all_L_values.push_back(L_complex);
            energies_lab.push_back(Elab);

        } catch (const std::exception &e) {
            std::cerr << "Error en E = " << E << " MeV: " << e.what() << '\n';
            all_Wr.push_back(std::numeric_limits<long double>::quiet_NaN());
            all_Wi.push_back(std::numeric_limits<long double>::quiet_NaN());
            all_cross_sections.push_back(std::numeric_limits<long double>::quiet_NaN());
            all_s_factors.push_back(std::numeric_limits<long double>::quiet_NaN());
            all_L_values.push_back(std::complex<double>(std::numeric_limits<double>::quiet_NaN(), 
                                                       std::numeric_limits<double>::quiet_NaN()));
            energies_lab.push_back(Elab);
        }
    }

    // ---------------------------------------------------------------------
    // Write results
    for (size_t i = 0; i < all_Wr.size(); ++i) {
        if (!std::isnan(all_Wr[i])) {
	  long double cross_section_mb = all_cross_sections[i]* 10000; // / 100.0L; // Conversión a mb

            out << std::fixed << std::setprecision(15)  // Increased precision
                << energies_lab[i] << '\t'
                << all_Wr[i] << '\t'
                << all_Wi[i] << '\t'
                << std::scientific << cross_section_mb << '\t'  // Scientific notation for small values
                << all_s_factors[i] << '\n';

            // Escribir datos de L
            long double L_real = all_L_values[i].real();
            long double L_imag = all_L_values[i].imag();
            long double L_magnitude = std::abs(all_L_values[i]);

            L_out << std::fixed << std::setprecision(15)
                  << energies_lab[i] << '\t'
                  << L_real << '\t'
                  << L_imag << '\t'
                  << L_magnitude << '\n';

            // Console output
            std::cout << "Elab = " << energies_lab[i] << " MeV\t"
                      << "Wr = " << all_Wr[i] << "\t"
                      << "Wi = " << all_Wi[i] << "\t"
                      << "σ = " << std::scientific << cross_section_mb << " mb\t"
                      << "S = " << all_s_factors[i] << " MeV*mb\t"
                      << "L_real = " << L_real << "\t"
                      << "L_imag = " << L_imag << "\t"
                      << "|L| = " << L_magnitude << "\n";
        }
    }

    std::cout << "Archivos generados:\n";
    std::cout << "- " << filename << '\n';
    std::cout << "- " << L_filename << '\n';
    return 0;
}
