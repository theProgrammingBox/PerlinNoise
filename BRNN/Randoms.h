#pragma once

#include <chrono>
#include <math.h>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

using std::max;
using std::min;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

const unsigned int second = duration_cast<seconds>(high_resolution_clock::now().time_since_epoch()).count();
const unsigned int millisecond = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
const unsigned int microsecond = duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
const unsigned int nanosecond = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();

int state = millisecond ^ microsecond ^ second ^ nanosecond;

const unsigned int UIntRandom()
{
	state = (state ^ 2747636419u) * 2654435769u;
	state = (state ^ (state >> 16u)) * 2654435769u;

	return state;
}
const int IntRandom()
{
	return UIntRandom();
}

const double UDoubleRandom()	// 0 through 1
{
	return UIntRandom() * 2.32830643654e-10;
}

const double DoubleRandom()		// -1 through 1
{
	return IntRandom() * 4.65661287308e-10;
}

const double NormalRandom(double mean, double standardDeviation)
{
	double x, y, radius;
	do
	{
		x = DoubleRandom();
		y = DoubleRandom();

		radius = x * x + y * y;
	} while (radius > 1.0 || radius == 0.0);

	return x * sqrt(-2.0 * log(radius) / radius) * standardDeviation + mean;
}

const uint64_t UIntRandom2D(uint64_t seeded, double x, double y)
{
	uint64_t seed = (seeded ^ 2747636419u) * 2654435769u;
	//cout << seed << endl;
	seed = (seed ^ (seed >> 16u)) * 2654435769u;
	seed = (seed ^ *(uint64_t*)&x) * 2654435769u;
	//cout << seed << endl;
	seed = (seed ^ (seed >> 16u)) * 2654435769u;
	seed = (seed ^ *(uint64_t*)&y) * 2654435769u;
	//cout << seed << "\n\n";
	return (seed ^ (seed >> 16u)) * 2654435769u;
}

const int64_t IntRandom2D(uint64_t seed, double x, double y)
{
	return UIntRandom2D(seed, x, y);
}

const double UDoubleRandom2D(uint64_t seed, double x, double y) // 0 through 1
{
	return UIntRandom2D(seed, x, y) * 5.42101086243e-20;
}

const double DoubleRandom2D(uint64_t seed, double x, double y) // -1 through 1
{
	return IntRandom2D(seed, x, y) * 1.08420217249e-19;
}

//double Interpolate(double a, double b, double x) { return (b - a) * (x * x * x * (x * (x * 6 - 15) + 10)) + a; }

double Interpolate(double a, double b, double x) { return (b - a) * (x * x * (3 - x - x)) + a; }

//double Interpolate(double a, double b, double x) { return (b - a) * x + a; }

double Interpolate2D(uint64_t seed, double x, double y)
{
	int integer_X = x - (x < 0),
		integer_Y = y - (y < 0);

	double fractional_X = x - integer_X,
		fractional_Y = y - integer_Y;

	double v1 = DoubleRandom2D(seed, integer_X, integer_Y),
		v2 = DoubleRandom2D(seed, integer_X + 1, integer_Y),
		v3 = DoubleRandom2D(seed, integer_X, integer_Y + 1),
		v4 = DoubleRandom2D(seed, integer_X + 1, integer_Y + 1),
		i1 = Interpolate(v1, v2, fractional_X),
		i2 = Interpolate(v3, v4, fractional_X);

	return Interpolate(i1, i2, fractional_Y);
}

double PerlinRandom2D(uint64_t seed, double x, double y, double initalStep = 0.1, double stepC = 2, double weightC = 0.5, int numOctaves = 4)
{
	double total = 0;
	double weight = 1;
	double step = initalStep;
	double sum = 0;

	for (int i = 0; i < numOctaves; i++)
	{
		seed ^= UIntRandom2D(seed, seed, seed);
		total += Interpolate2D(seed, x * step, y * step) * weight;

		step *= stepC;
		sum += weight;
		weight *= weightC;
	}

	return total / sum;
}