#ifndef PHASESHIFT_H
#define PHASESHIFT_H

#include <vector>

double Sommerfield(double E);
double computePhaseShift(const std::vector<double>& u,const std::vector<double>& du, double E, double a, double l );

#endif
