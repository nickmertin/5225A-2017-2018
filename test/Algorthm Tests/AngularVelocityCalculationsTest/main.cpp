#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

using namespace std;

#define BEAM_R 16
#define BEAM_S 16
#define ARM_ANG_VEL_SCALAR 10

#define sinDegrees(x) sin(x * M_PI / 180)
#define cosDegrees(x) cos(x * M_PI / 180)

int main() {
  float degA, degB, vx, vy, wA, wB;
  while (true) {
    cout << "A B: ";
    cin >> degA >> degB;
    cout << "X Y: ";
    cin >> vx >> vy;
    float degAB = degA + degB;
    float sinA = sinDegrees(degA);
    float cosA = cosDegrees(degA);
    float sinAB = sinDegrees(degAB);
    float cosAB = cosDegrees(degAB);

    float den = cosA * sinAB - sinA * cosAB;
    float D = sqrt((BEAM_R * BEAM_R) + (BEAM_S * BEAM_S) - (2 * BEAM_R * BEAM_S * cosDegrees(degB)));

    float wA = (vx * sinAB - vy * cosAB) / (D * den) * ARM_ANG_VEL_SCALAR;
    float wB = (vx * sinA - vy * cosA) / (BEAM_S * den) * ARM_ANG_VEL_SCALAR;
    cout << "A B: " << wA << " " << wB << "\n\n";
  }
}
