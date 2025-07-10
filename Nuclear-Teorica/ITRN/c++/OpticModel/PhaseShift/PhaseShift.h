#pragma once
#include <vector>
#include <complex>

long double Sommerfield(long double E);

// Ahora retorna Wr y Wi donde cot(delta0) = Wr + i*Wi
std::complex<long double> computeWrWi(const std::vector<std::complex<long double>>& u, 
                                     const std::vector<std::complex<long double>>& du, 
                                     long double E, long double a, long double l);
