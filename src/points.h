#pragma once
#include <Windows.h>

struct point {
	FLOAT x, y, z;
};

void rotate(FLOAT &a, FLOAT &b, FLOAT angle);