#pragma once
#include "BRNNParameters.h"

class BRNNValues
{
public:
	BRNNParameters* parameters;

	float prestate[NETWORK_LOOPS][NETWORK_SIZE]{};
	float state[NETWORK_LOOPS + 1][NETWORK_SIZE]{}; // including input state

	float preoutput[OUTPUT_SIZE]{};
	float output[OUTPUT_SIZE]{};

	BRNNValues()
	{
		Reset();
	}

	BRNNValues(BRNNParameters* Parameters)
	{
		parameters = Parameters;

		Reset();
	}

	void Reset()
	{
		for (int i = 0; i < NETWORK_SIZE; i++) state[NETWORK_LOOPS][i] = parameters->initialState[i];
	}

	void ForwardPropagate(float* Input, float* Output)
	{
		int layer, parentNode, childNode;

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++) state[0][parentNode] = state[NETWORK_LOOPS][parentNode];

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++) preoutput[parentNode] = parameters->outputBiases[parentNode];

		for (layer = 0; layer < NETWORK_LOOPS; layer++)
		{
			for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
			{
				prestate[layer][parentNode] = parameters->stateBiases[parentNode];

				for (childNode = 0; childNode < INPUT_SIZE; childNode++) prestate[layer][parentNode] += Input[childNode] * parameters->inputWeights[parentNode][childNode];

				for (childNode = 0; childNode < NETWORK_SIZE; childNode++) prestate[layer][parentNode] += state[layer][childNode] * parameters->stateWeights[parentNode][childNode];

				state[layer + 1][parentNode] = RELU(prestate[layer][parentNode]);

				for (childNode = 0; childNode < OUTPUT_SIZE; childNode++) preoutput[childNode] = state[layer + 1][parentNode] * parameters->outputWeights[childNode][parentNode];
			}
		}

		Softmax(preoutput, output);

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++) Output[parentNode] = output[parentNode];
	}
};