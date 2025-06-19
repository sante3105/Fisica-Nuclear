#include "Numerov.h"
#include <cmath>
#include <stdexcept>

std::vector<double> numerov(double E, const Potential& V, double rmin, double rmax, int N, double mu, double hbar) {
    std::vector<double> u(N, 0.0);
    double h = (rmax - rmin) / (N - 1);
    double h2 = h * h;

    // Precomputar el vector de posiciones y f(r) = k^2 - (2μ/ħ²)·V(r)
    std::vector<double> r(N);
    std::vector<double> f(N);

    double k2 = (2.0 * mu * E) / (hbar * hbar);

    for (int i = 0; i < N; ++i) {
        r[i] = rmin + i * h;
        f[i] = k2 - (2.0 * mu / (hbar * hbar)) * V(r[i]);
    }

    // Condiciones iniciales para l = 0
    u[0] = 0.0;
    u[1] = 1e-5;  // Valor pequeño pero no nulo (equivale a imposición de u ~ r cerca de 0)

    // Algoritmo de Numerov
    for (int i = 1; i < N - 1; ++i) {
        double f_im1 = f[i - 1];
        double f_i   = f[i];
        double f_ip1 = f[i + 1];

        u[i + 1] = (2.0 * u[i] * (1.0 - (5.0 * h2 * f_i / 12.0)) -
                    u[i - 1] * (1.0 + h2 * f_im1 / 12.0)) /
                   (1.0 + h2 * f_ip1 / 12.0);
    }

    return u;
}
