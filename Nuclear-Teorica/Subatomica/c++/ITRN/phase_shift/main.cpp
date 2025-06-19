#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <cmath>


#include "Numerov/Numerov.h"
#include "PhaseShift/PhaseShift.h"
#include "Potential/Potential.h"

int main() {
    // Parámetros físicos
    double mu = 469.459;     // MeV/c^2
    double hbar = 197.327;   // MeV·fm
    double rmin = 0.01;
    double rmax = 20;
    double a = 8.0;          // radio de empalme
    int N = 10;

    Potential V;
    std::ofstream out("delta_vs_E.dat");
    out << "# E [MeV]\tdelta_0 [deg]\n";

    for (double E = 0.1; E <= 10; E += 0.05) {
        try {
            auto u = numerov(E, V, rmin, rmax, N, mu, hbar);
            double delta_rad = computePhaseShift(u, E, a, rmin, rmax, N, mu, hbar);
            double delta_deg = delta_rad * 180.0 / M_PI;
            out << std::fixed << std::setprecision(4)
                << E << "\t" << delta_deg << "\n";
            std::cout << "E = " << E << " MeV\tδ₀ = " << delta_deg << "°\n";
        } catch (const std::exception& e) {
            std::cerr << "Error at E = " << E << " MeV: " << e.what() << "\n";
        }
    }

    out.close();
    std::cout << "Resultado guardado en 'delta_vs_E.dat'\n";
    return 0;
}

