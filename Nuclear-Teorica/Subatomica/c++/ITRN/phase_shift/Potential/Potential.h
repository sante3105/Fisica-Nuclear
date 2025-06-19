#ifndef POTENTIAL_H
#define POTENTIAL_H

class Potential {
 public:
  Potential();
  // devuelve V(r) = VN(r) +VC(r)
  double operator()(double r) const;

  // componentes separadas
  double nuclear(double r) const;
  double coulomb(double r) const;


 private:
  const double V0 = -73.8; //MEV
  const double a = 2.70; //fm
  const double Z1Z2e2 = 6.0 * 1.44;// MeV·fm (Z1 = 1, Z2 = 6, e^2 = 1.44) values for C¹² +p
};

#endif
