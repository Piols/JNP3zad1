#include "points.h"
#include <cmath>

void rotate(FLOAT& a, FLOAT &b, FLOAT angle) {
	FLOAT new_a = a * std::cos(angle) - b * std::sin(angle);
	b = a * std::sin(angle) + b * std::cos(angle);
	a = new_a;
}