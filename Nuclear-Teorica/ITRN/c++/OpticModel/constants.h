#ifndef CONSTANTS_H
#define CONSTANTS_H
#pragma once
#include <complex>

using Complex = std::complex< long double>;

// Cargas
inline constexpr int Z_P = 1;   // Z del proyectil (Deuterio)
inline constexpr int Z_T = 1; //1;   // Z del blanco (Tritio)

// Masas (en unidades de masa atómica)
inline constexpr int A_P = 2; // 2;   // A del proyectil
inline constexpr int A_T = 3; // 3;  // A del blanco

// Constantes físicas
inline constexpr  long double uma_to_MeV = 931.494;      // 1 uma = 931.494 MeV/c²
inline constexpr  long double hbar = 197.326980;         // MeV·fm
inline constexpr  long double e2 = 1.44;                 // e² en MeV·fm

// Constante de estructura fina (exacta como Python)
inline constexpr  long double alpha = 0.0072973525693;

// Masa reducida μ = (A_P * A_T / (A_P + A_T)) * uma_to_MeV
inline constexpr  long double mu = (A_P * A_T * uma_to_MeV) / (A_P + A_T); // MeV/c²

// Potencial nuclear (real)
inline constexpr  long double V0 = -73.8;
inline constexpr  long double V_r = -30.0;  // MeV (parte real)
inline constexpr  long double V_i = -0.04964;   // MeV (parte imaginaria, inicialmente cero)
inline constexpr  long double a_p =1.338; // 2.70; // fm
inline constexpr  long double R_N = a_p*(std::pow(A_P, 1/3) + std::pow(A_T, 1/3));;

// Potencial complejo
inline const Complex V0_complex(V_r, V_i); // Negativo para aplicar en potential

#endif
