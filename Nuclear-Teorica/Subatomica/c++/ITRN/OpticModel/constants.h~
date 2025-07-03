#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once

// Cargas
inline constexpr int Z_P = 2;   // Z del proyectil (proton)
inline constexpr int Z_T = 2;   // Z del blanco (carbono-12)

// Masas (en unidades de masa atómica)
inline constexpr int A_P = 4;   // A del proyectil
inline constexpr int A_T = 4;  // A del blanco

// Constantes físicas
inline constexpr double uma_to_MeV = 931.494;      // 1 uma = 931.494 MeV/c²
inline constexpr double hbar = 197.326980;         // MeV·fm
inline constexpr double e2 = 1.44;                 // e² en MeV·fm
inline constexpr double alpha = 0.0072973525693;   // Fine structure constant



// Masa reducida μ = (A_P * A_T / (A_P + A_T)) * uma_to_MeV
inline constexpr double mu = (A_P * A_T * uma_to_MeV) / (A_P + A_T); // MeV/c²

// Potencial nuclear
inline constexpr double V0 = -122.6225;  // MeV
inline constexpr double a_p = 2.132;   // fm

#endif
