#include "Numerov.h"
#include "../Potential/HarmonicPotential.h"
#include <iostream>
#include <fstream>

int main() {
    HarmonicPotential V;
    double mu = 0.5;       // m = 1, entonces mu = m/2 = 0.5
    double hbar = 1.0;
    double E = 1.0;         // Ground state
    double rmin = -6.0;
    double rmax = 6.0;
    int N = 1000;

    auto u = numerov(E, V, rmin, rmax, N, mu, hbar);

    std::ofstream out("harmonic_wavefunction.dat");
    double h = (rmax - rmin) / (N - 1);
    for (int i = 0; i < N; ++i) {
        double r = rmin + i * h;
        out << r << "\t" << u[i] << "\n";
    }

    std::cout << "Saved to 'harmonic_wavefunction.dat'\n";
}
