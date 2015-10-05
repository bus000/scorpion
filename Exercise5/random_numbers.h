#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

// Random number between 0 and 1 (uniform distribution)
double randf ();

// The normal distribution.
//   Computes the value of a normal distribution with mean value 'mean'
//   and standard deviation 'sigma' at the point 'x'.
double normal_pdf (double x, double mean, double sigma);

// Normal random number generater (Box-Muller method)
// (see www.taygeta.com/random/gaussian.html)
// mean m, standard derivation s
double randn(double m, double s);

// Von Mises Distribution
double von_mises_pdf (const double x, const double mu, const double kappa);

// Von Mises random number generator
double rand_von_mises (const double mu, const double kappa);

#endif
