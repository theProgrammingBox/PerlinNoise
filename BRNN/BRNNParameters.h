#pragma once
#include "BRNNConfig.h"


class BRNNParameters
{
private:
	Random random;
	OpenSimplexNoise noise;

public:
	float initialState[NETWORK_SIZE]{};

	float stateBiases[NETWORK_SIZE]{};
	float outputBiases[OUTPUT_SIZE]{};

	float inputWeights[NETWORK_SIZE][INPUT_SIZE]{};
	float stateWeights[NETWORK_SIZE][NETWORK_SIZE]{};
	float outputWeights[OUTPUT_SIZE][NETWORK_SIZE]{};

	BRNNParameters()
	{
		random = Random();

		Reset();
	}

	void Reset()
	{
		int parentNode, childNode;

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			initialState[parentNode] = noise.Evaluate(parentNode * 0.31579 + 37.951, -95.173);
		}

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			initialState[parentNode] *= noise.Evaluate(parentNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
		}

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			stateBiases[parentNode] = noise.Evaluate(parentNode * 0.31579 + 37.951, -95.173);
		}

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			stateBiases[parentNode] *= noise.Evaluate(parentNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
		}

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
		{
			outputBiases[parentNode] = noise.Evaluate(parentNode * 0.31579 + 37.951, -95.173);
		}

		noise = OpenSimplexNoise(random.ULongRandom());

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
		{
			outputBiases[parentNode] *= 0.5 * noise.Evaluate(parentNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
		}

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < INPUT_SIZE; childNode++)
			{
				inputWeights[parentNode][childNode] = noise.Evaluate(childNode * 0.31579 + 37.951, -95.173);
			}

			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < INPUT_SIZE; childNode++)
			{
				inputWeights[parentNode][childNode] *= noise.Evaluate(childNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
			}
		}

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
		{
			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < NETWORK_SIZE; childNode++)
			{
				stateWeights[parentNode][childNode] = noise.Evaluate(childNode * 0.31579 + 37.951, -95.173);
			}

			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < NETWORK_SIZE; childNode++)
			{
				stateWeights[parentNode][childNode] *= noise.Evaluate(childNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
			}
		}

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
		{
			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < NETWORK_SIZE; childNode++)
			{
				outputWeights[parentNode][childNode] = noise.Evaluate(childNode * 0.31579 + 37.951, -95.173);
			}

			noise = OpenSimplexNoise(random.ULongRandom());

			for (childNode = 0; childNode < NETWORK_SIZE; childNode++)
			{
				outputWeights[parentNode][childNode] *= noise.Evaluate(childNode * 0.31579 - 51.937, 19.537) * 0.5 + 0.5;
			}
		}
	}
};