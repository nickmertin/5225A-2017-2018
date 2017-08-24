#include <cmath>
#include <ctime>
#include <iostream>

#define RANDOM (static_cast<float>(rand() - RAND_MAX / 2) / static_cast<float>(RAND_MAX))

#define CUBIC(x, a, b, c, d) (x * (x * (x * a + b) + c) + d)

#define SGN_OF_DIFF(x, y) ((x > y) - (x < y))
#define sgn(x) ((x > 0) - (0 > x))

int solveCubic(float a, float b, float c, float d, float* sln)
{
  float p = -b / (3 * a);
  float q = p * p * p + (b * c - 3 * a * d) / (6 * a * a);
  float r = c / (3 * a);
  float p2 = p * p;
  float q2 = q * q;
  float r_m_p2 = r - p2;
  float s = sqrt(fabs(q2 + r_m_p2 * r_m_p2 * r_m_p2));
  sln[0] = pow(fabs(q + s), 1.0 / 3.0) * SGN_OF_DIFF(q, -s) + pow(fabs(q - s), 1.0 / 3.0) * SGN_OF_DIFF(q, -s) + p;
  float b_2 = a * sln[0] + b;
  float c_2 = b_2 * sln[0] + c;
  float dscm = b_2 * b_2 - 4 * a * c_2;
  int count = sgn(dscm) + 2;
  if (count == 2)
  {
    sln[1] = -b_2 / (2 * a);
  }
  else if (count == 3)
  {
    float sqrt_dscm = sqrt(dscm);
    float _den = 2 * a;
    sln[1] = (sqrt_dscm - b_2) / _den;
    sln[2] = (-sqrt_dscm - b_2) / _den;
  }
  return count;
}

int main() {
  srand(time(NULL));
  for (int i = 0; i < 10000; ++i) {
    float a = RANDOM;
    float b = RANDOM;
    float c = RANDOM;
    float d = RANDOM;

    std::cout << a << "x^3 + " << b << "x^2 + " << c << "x + " << d << std::endl;

    float sln[3];

    int count = solveCubic(a, b, c, d, sln);

    for (int i = 0; i < count; ++i) {
      float value = CUBIC(sln[i], a, b, c, d);
      std::cout << value << std::endl;
    }

    std::cout << std::endl;
  }
}
