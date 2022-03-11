#pragma once

#include <chrono>

class Random
{
private:
	uint64_t seed;

public:
	Random()
	{
		seed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		seed = seed * 6364136223846793005L + 1442695040888963407L;
		seed ^= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		seed = seed * 6364136223846793005L + 1442695040888963407L;
		seed ^= std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		seed = seed * 6364136223846793005L + 1442695040888963407L;
		seed ^= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		seed = seed * 6364136223846793005L + 1442695040888963407L;
	}

	uint64_t ULongRandom()
	{
		seed = seed * 6364136223846793005L + 1442695040888963407L;
		seed = seed * 6364136223846793005L + 1442695040888963407L;
		seed = seed * 6364136223846793005L + 1442695040888963407L;

		return seed;
	}

	int64_t LongRandom()
	{
		return ULongRandom();
	}

	uint32_t UIntRandom()
	{
		return ULongRandom();
	}

	int32_t IntRandom()
	{
		return UIntRandom();
	}

	double UDoubleRandom()	// 0 through 1
	{
		return ULongRandom() * 5.42101086243e-20;
	}

	double DoubleRandom()	// -1 through 1
	{
		return LongRandom() * 1.08420217249e-19;
	}

	float UFloatRandom()	// 0 through 1
	{
		return UDoubleRandom();
	}

	float FloatRandom()	// -1 through 1
	{
		return DoubleRandom();
	}

	const double NormalRandom(double mean, double standardDeviation)
	{
		double x, y, radius;
		do
		{
			x = DoubleRandom();
			y = DoubleRandom();

			radius = x * x + y * y;
		} while (radius >= 1.0);

		return x * sqrt(-2.0 * log(radius) / radius) * standardDeviation + mean;
	}
};