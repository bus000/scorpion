#include <cstdlib>
#include <math.h>

// Auxil. functions
inline double sign (double a)
{
  if (a < 0)
    {
      return (-1);
    }
  else if (a > 0)
    {
      return (+1);
    }
  else
    {
      return (0);
    }
}

inline double square (double x)
{
  return x * x;
}

double bessi0 (double x)
{
  const double abs_x = fabs (x);
  double retval;

  if (abs_x < 3.75)
    {
      const double y = square (x / 3.75);
      retval = 1.0 + y * (3.5156229 + y * (3.0899424 + y * (1.2067492 + 
                     y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))));
    }
  else
    {
      const double y = 3.75 / abs_x;
      retval = (exp (abs_x) / sqrt(abs_x)) * (0.39894228 + y * (0.1328592e-1 + 
               y * (0.225319e-2 + y * (-0.157565e-2 + y * (0.916281e-2 +
               y * (-0.2057706e-1 + y * (0.2635537e-1 + y * (-0.1647633e-1 +
               y * 0.392377e-2))))))));
    }

  return retval;
}

// Random number between 0 and 1 (uniform distribution)
double randf ()
{
  const int r = rand();
  return ((double)r)/(double)RAND_MAX;
}

// The normal distribution.
//   Computes the value of a normal distribution with mean value 'mean'
//   and standard deviation 'sigma' at the point 'x'.
double normal_pdf (double x, double mean, double sigma)
{
    x = x - mean;
    return exp (-(x*x)/(2.0*sigma*sigma))/(sigma*sqrt(2.0*M_PI));
}


// Normal random number generater
// mean m, standard deviation s
double randn (double m, double s)
{
    double x1, x2, w, y1;

    do
      {
        x1 = 2.0 * randf () - 1.0;
        x2 = 2.0 * randf () - 1.0;
        w = x1 * x1 + x2 * x2;
      }
    while (w >= 1.0);
    
    w = sqrt ((-2.0 * log(w)) / w);
    y1 = x1 * w;
    //y2 = x2 * w;
    return (m + y1 * s);
}

double von_mises_pdf (const double x, const double mu, const double kappa)
{
  const double p = exp (kappa * cos (x - mu));
  const double Z = 2.0 * M_PI * bessi0 (kappa);
  return p / Z;
}

double rand_von_mises (const double mu, const double kappa)
{
  double theta;
  if (kappa < 1e-6)
    {
      // kappa is small: sample uniformly on circle
      theta = 2.0 * M_PI * randf () - M_PI;
    }
  else
    {
      const double a = 1.0 + sqrt (1.0 + 4.0 * kappa * kappa);
      const double b = (a - sqrt (2.0 * a)) / (2.0 * kappa);
      const double r = (1.0 + b*b) / (2.0 * b);

      bool not_done = true;
      double f;
      while (not_done)
        {
          const double u1 = randf ();
          const double u2 = randf ();
      
          const double z = cos (M_PI * u1);
          f = (1.0 + r * z) / (r + z);
          const double c = kappa * (r - f);
      
          not_done = (u2 >= c * (2.0 - c)) && (log (c) - log (u2) + 1.0 - c < 0.0);
        }
    
      const double u3 = randf ();
      theta = mu + sign (u3 - 0.5) * acos (f);
    }
  
  return theta;
}

