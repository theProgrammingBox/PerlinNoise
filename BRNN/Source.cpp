#define OLC_PGE_APPLICATION
#include "Randoms.h"
#include "OpenSimplex.h"
#include "olcPixelGameEngine.h"

using olc::Key;
using olc::vi2d;
using olc::Pixel;

using std::cout;
using std::endl;
using std::max;
using std::min;

const int screenSize = 500;
const int screenResolution = 2;
const double halfScreenSize = screenSize / 2;

class Example : public olc::PixelGameEngine
{
public:
	Random random;
	OpenSimplexNoise noise;
	double z;

	Pixel mapToWeight(double d) { // -1 - 1
		double r = (d < 0) ? min(1.0, -2 * d) : 0;
		double g = (d >= 0) ? min(1.0, 2 * d) : 0;

		return Pixel(r * 0xff, g * 0xff, 0);
	}

	Pixel mapToRainbow(double d) { // 0 - 1
		d *= 6;
		double r = (d > 4) ? max(0.0, min(1.0, 6 - d)) : max(0.0, min(1.0, d - 2));
		double g = (d > 2) ? max(0.0, min(1.0, 4 - d)) : max(0.0, min(1.0, d));
		double b = (d > 3) ? max(0.0, min(1.0, d - 4)) : max(0.0, min(1.0, 2 - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
	}

	Pixel mapToBAndW(double d) { return Pixel(d * 0xff, d * 0xff, d * 0xff); }

	Example()
	{
		sAppName = "PerlinRandom";
	}

	bool OnUserCreate() override
	{
		random = Random();
		noise = OpenSimplexNoise(random.ULongRandom());
		z = 0;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(Key::SPACE).bPressed) { noise = OpenSimplexNoise(random.ULongRandom()); }

		z += 0.05;
		for (int x = 0; x < screenSize; x++)
			for (int y = 0; y < screenSize; y++)
			{
				double mx = x - halfScreenSize;
				double my = y - halfScreenSize;
				double layeredNoise =
					noise.Evaluate(x * 0.02, y * 0.02, z + 100) * (3.0 / 6.0) +
					noise.Evaluate(x * 0.04, y * 0.04, z + 050) * (2.0 / 6.0) +
					noise.Evaluate(x * 0.08, y * 0.08, z + 000) * (1.0 / 6.0);
				double value = layeredNoise * max(0.0, 1 - (mx * mx + my * my) / (halfScreenSize * halfScreenSize));

				FillRect(vi2d(x, y), vi2d(1, 1), mapToWeight(value));
				//FillRect(vi2d(x, y), vi2d(1, 1), mapToBAndW(value * 0.5 + 0.5)); // covert from -1 - 1 to 0 - 1
				//FillRect(vi2d(x, y), vi2d(1, 1), mapToRainbow(value * 0.5 + 0.5)); // covert from -1 - 1 to 0 - 1
			}

		return true;
	}
};

int main()
{
	Example demo;

	if (demo.Construct(screenSize, screenSize, screenResolution, screenResolution))
		demo.Start();

	return 0;
}