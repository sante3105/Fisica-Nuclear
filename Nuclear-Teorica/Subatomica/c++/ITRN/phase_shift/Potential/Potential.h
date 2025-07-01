#ifndef POTENTIAL_H
#define POTENTIAL_H

class Potential {
public:
    Potential();

    double operator()(double r, int l, double mua = 0.0) const;

    double nuclear(double r) const;
    double coulomb(double r) const;
    double angular(double r, int l, double mua = 0.0) const;
};

#endif
