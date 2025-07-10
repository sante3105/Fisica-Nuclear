#ifndef SL_H
#define SL_H

#include <complex>
#include <vector>

/**
 *  computeSl
 *  ---------
 *  Single–channel Coulomb-modified S-matrix element
 *
 *  Parameters
 *    u   – radial solution u(r) on an equally-spaced mesh
 *    du  – derivative du/dr on the same mesh
 *    E   – kinetic energy        (units consistent with mu and hbar)
 *    a   – channel radius r = a  (same length units as the mesh)
 *    l   – orbital angular momentum (integer or half-integer ≥ 0)
 *
 *  Returns
 *    S_l(E,a)  as std::complex<double>
 *
 *  Requires GSL for Coulomb wave functions.
 */
std::complex<double>
computeSl(const std::vector<std::complex<double>>& u,
          const std::vector<std::complex<double>>& du,
          double E, double a, double l);

#endif // SL_H
