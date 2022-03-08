#define OLC_PGE_APPLICATION
#include "Randoms.h"
#include "olcPixelGameEngine.h"

using olc::Key;
using olc::vd2d;
using olc::vi2d;
using olc::Pixel;
using std::to_string;

using std::cout;
using std::endl;
using std::max;
using std::min;

using std::chrono::seconds;
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

#define friction 0.1
#define screenSize 1000
#define screenResolution 1
#define halfScreenSize screenSize / 2

class Example : public olc::PixelGameEngine
{
public:
	unsigned int seed;
	double* screen;
	double zoom;
	double frequencyC = 0.5;
	double weightC = 2;
	vd2d pos;
	vd2d posv;

	Pixel mapToRainbow(double d) {
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
		seed = IntRandom();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(Key::SPACE).bPressed) { seed = IntRandom(); }

		for (int x = 0; x < screenSize; x++)
			for (int y = 0; y < screenSize; y++)
			{
				double mx = x - halfScreenSize;
				double my = y - halfScreenSize;
				double value = (PerlinRandom2D(seed, x, y, 0.01, 1.8, 0.6, 4) + 1) / 2 * max(0.0, 1 - sqrt(mx * mx + my * my) / (halfScreenSize));

				FillRect(vi2d(x, y), vi2d(1, 1), mapToBAndW(value));
				//FillRect(vi2d(x, y), vi2d(1, 1), mapToRainbow(value));
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