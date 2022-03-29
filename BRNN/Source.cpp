#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "BRNNTrainer.h"

using olc::Key;
using olc::vi2d;
using olc::Pixel;

using std::cout;
using std::cin;
using std::endl;
using std::max;
using std::min;

double FalseActivation(double x)
{
	return x < -1 ? -1 : (x > 1 ? 1 : 1.5 * x - 0.5 * x * x * x);
}

double FalseActivationGradient(double x, double g)
{
	return x < -1 ? (g < -1 ? 0 : 1) : (x > 1 ? (g > 1 ? 0 : 1) : 1.5 - 1.5 * x * x);
}

class Example : public olc::PixelGameEngine
{
public:
	Random random;
	int count;
	double a;
	double b;
	double c;

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
		sAppName = "Visualizing";
	}

	bool OnUserCreate() override
	{
		random = Random();
		count = 0;

		a = random.DoubleRandom();
		b = random.DoubleRandom();
		c = random.DoubleRandom();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		double i1 = random.DoubleRandom();
		double i2 = random.DoubleRandom();

		double i = (a * i1 + b * i2 + c);
		double o = i * i * i;
		double e = (i1 + i2) / 2.0;
		double dO = e - o;
		double di = 3 * i * i * dO;
		double da = i1 * di;
		double db = i2 * di;
		double dc = di;

		a += da * 0.005;
		b += db * 0.005;
		c += dc * 0.005;
		if (count++ == 1000)
		{
			if (abs(dO) < 0.1)
			{
				cout << "a: " << a << endl;
				cout << "b: " << b << endl;
				cout << "c: " << c << "\n\n";
			}

			count = 0;
			FillRect(vi2d(0, 0), vi2d(1000, 1000), olc::BLACK);
			FillRect(vi2d(0, 0), vi2d(100, 100 * abs(dO)));
		}

		return true;
	}
};

int main()
{
	/*Example demo;

	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();*/

	BRNNTrainer networkTrainer;

	networkTrainer = BRNNTrainer();
	//networkTrainer.ResetNetwork(); // resets the parameter
//	networkTrainer.LoadNetwork("Network.txt"); // loads the parameter

	while (true)
	{
		networkTrainer.Train(1000);
		//networkTrainer.SaveNetwork("Network.txt");
	}
	//networkTrainer.Train(1);

	//	NetworkParameters networkParameters;
	//	NetworkValues networkValues;
	//
	//	networkParameters.Reset();
	//	networkValues.Assign(&networkParameters);

	return 0;
}