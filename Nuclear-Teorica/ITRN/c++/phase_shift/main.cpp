#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Numerov/Numerov.h"
#include "PhaseShift/PhaseShift.h"
#include "Potential/Potential.h"

void unwrapPhases(std::vector<double>& phases) {
    if (phases.size() < 2) return;
    
    for (size_t k = 1; k < phases.size(); ++k) {
        double diff = phases[k] - phases[k-1];
        
        if (diff > 1.5) {
            // Apply π to all previous phases
            for (size_t i = 0; i < k; ++i) {
                phases[i] += M_PI;
            }
        } else if (diff < -1.5) {
            // Apply π to all subsequent phases
            for (size_t i = k; i < phases.size(); ++i) {
                phases[i] += M_PI;
            }
        }
    }
}

int main() {
    double l, a;
    std::cout << "Ingrese valor de l (momento angular): ";
    std::cin  >> l;
    std::cout << "Ingrese valor del radio del canal a [fm]: ";
    std::cin  >> a;

    // Construir nombre de archivo
    std::ostringstream fname;
   fname << "data/delta_vs_E_l=" << l << "_a=" << a << ".dat";



    Potential pot;
    std::ofstream out(fname.str());
    if(!out) {
        std::cerr << "No se pudo abrir " << fname.str() << "\n";
        return 1;
    }
    out << "# E [MeV]\tdelta_l [deg]\n";
    
    std::vector<double> all_phases;
    std::vector<double> energies;

    for(double E = 0.1; E <= 20 + 1e-9; E += 0.01) {
        try {
            auto [u, du] = numerov(l, E, a, pot);  // ahora con pot
            double delta_rad = computePhaseShift(u, du, E, a, l);

	    all_phases.push_back(delta_rad);
            energies.push_back(E);
	    

        } catch(const std::exception& e) {
            std::cerr << "Error en E = " << E
                      << " MeV: " << e.what() << '\n';
	               // Push NaN or skip this energy
            all_phases.push_back(std::numeric_limits<double>::quiet_NaN());
            energies.push_back(E);
        }
    }

    // Apply phase unwrapping
    unwrapPhases(all_phases);
    
    // Second pass: write results to file
    for(size_t i = 0; i < all_phases.size(); ++i) {
        if (!std::isnan(all_phases[i])) {
            double delta_deg = all_phases[i] * 180.0 / M_PI;
            
            out << std::fixed << std::setprecision(4)
                << energies[i] << '\t' << delta_deg << '\n';
            
            std::cout << "E = " << energies[i]
                      << " MeV\tδ = " << delta_deg << "°\n";
        }
    }
    
    std::cout << "Archivo generado: " << fname.str() << '\n';
    return 0;
}
