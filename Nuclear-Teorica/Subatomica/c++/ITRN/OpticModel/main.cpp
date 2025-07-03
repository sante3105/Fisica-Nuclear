#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <complex>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Numerov/Numerov.h"
#include "PhaseShift/PhaseShift.h"
#include "Potential/Potential.h"
#include "constants.h"

// Phase unwrapping function
void unwrapPhases(std::vector<double>& phases) {
    if (phases.size() < 2) return;
    
    for (size_t k = 1; k < phases.size(); ++k) {
        double diff = phases[k] - phases[k-1];
        
        if (diff > 1.5) {
            for (size_t i = 0; i < k; ++i) {
                phases[i] += M_PI;
            }
        } else if (diff < -1.5) {
            for (size_t i = k; i < phases.size(); ++i) {
                phases[i] += M_PI;
            }
        }
    }
}

int main() {
    double l, a;
    std::cout << "Ingrese valor de l (momento angular): ";
    std::cin >> l;
    std::cout << "Ingrese valor del radio del canal a [fm]: ";
    std::cin >> a;
    
    char choice;
    std::cout << "¿Desea ver resultados complejos? (y/n): ";
    std::cin >> choice;
    
    bool show_complex = (choice == 'y' || choice == 'Y');
    
    Potential pot;
    
    if (show_complex) {
        // Mostrar función de onda compleja para una energía específica
        double test_E;
        std::cout << "Ingrese energía para mostrar función de onda compleja [MeV]: ";
        std::cin >> test_E;
        
        std::ostringstream complex_fname;
        complex_fname << "wavefunction_complex_E=" << test_E << "_l=" << l << "_a=" << a << ".dat";
        
        std::ofstream complex_out(complex_fname.str());
        if (complex_out) {
            complex_out << "# r [fm]\tu_real\tu_imag\tdu_real\tdu_imag\t|u|^2\n";
            
            try {
                auto result = numerov_complex(l, test_E, a, pot);
                
                auto u_real = result.u_real();
                auto u_imag = result.u_imag();
                auto du_real = result.du_real();
                auto du_imag = result.du_imag();
                
                for (size_t i = 0; i < result.r.size(); ++i) {
                    double prob_density = u_real[i]*u_real[i] + u_imag[i]*u_imag[i];
                    
                    complex_out << std::fixed << std::setprecision(6)
                               << result.r[i] << '\t'
                               << u_real[i] << '\t'
                               << u_imag[i] << '\t';
                    
                    if (i < du_real.size()) {
                        complex_out << du_real[i] << '\t'
                                   << du_imag[i] << '\t';
                    } else {
                        complex_out << "0.0\t0.0\t";
                    }
                    
                    complex_out << prob_density << '\n';
                }
                
                std::cout << "Función de onda compleja guardada en: " << complex_fname.str() << '\n';
                
            } catch(const std::exception& e) {
                std::cerr << "Error calculando función de onda compleja: " << e.what() << '\n';
            }
        }
    }
    
    // Cálculo de phase shifts (usando compatibilidad con función real)
    std::ostringstream fname;
    fname << "delta_vs_E_l=" << l << "_a=" << a << ".dat";
    
    std::ofstream out(fname.str());
    if(!out) {
        std::cerr << "No se pudo abrir " << fname.str() << "\n";
        return 1;
    }
    
    out << "# E [MeV]\tdelta_l [deg]\n";
    
    std::vector<double> all_phases;
    std::vector<double> energies;
    
    // Calcular phase shifts
    for(double E = 0.1; E <= 2.05 + 1e-9; E += 0.01) {
        try {
            auto [u, du] = numerov(l, E, a, pot);  // Usa parte real solamente
            double delta_rad = computePhaseShift(u, du, E, a, l);
            
            all_phases.push_back(delta_rad);
            energies.push_back(E);
            
        } catch(const std::exception& e) {
            std::cerr << "Error en E = " << E
                      << " MeV: " << e.what() << '\n';
            all_phases.push_back(std::numeric_limits<double>::quiet_NaN());
            energies.push_back(E);
        }
    }
    
    // Phase unwrapping
    unwrapPhases(all_phases);
    
    // Escribir resultados
    for(size_t i = 0; i < all_phases.size(); ++i) {
        if (!std::isnan(all_phases[i])) {
            double delta_deg = all_phases[i] * 180.0 / M_PI;
            
            out << std::fixed << std::setprecision(4)
                << energies[i] << '\t' << delta_deg << '\n';
            
            if (i % 10 == 0) {  // Mostrar cada 10 valores para no saturar la consola
                std::cout << "E = " << energies[i]
                          << " MeV\tδ = " << delta_deg << "°\n";
            }
        }
    }
    
    std::cout << "Archivo de phase shifts generado: " << fname.str() << '\n';
    
    // Mostrar información sobre el potencial
    std::cout << "\nInformación del potencial:\n";
    std::cout << "V0 real = " << V_r << " MeV\n";
    std::cout << "V0 imag = " << V_i << " MeV\n";
    
    return 0;
}
