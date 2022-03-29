#pragma once
#include "BRNNValues.h"

class BRNNGradients
{
private:
	BRNNValues* values;

public:
	float initialStateGradient[NETWORK_SIZE]{};

	float stateBiasGradients[NETWORK_SIZE]{};
	float outputBiasGradients[OUTPUT_SIZE]{};

	float inputWeightGradients[NETWORK_SIZE][INPUT_SIZE]{};
	float stateWeightGradients[NETWORK_SIZE][NETWORK_SIZE]{};
	float outputWeightGradients[OUTPUT_SIZE][NETWORK_SIZE]{};

	float prestateGradient[NETWORK_LOOPS][NETWORK_SIZE]{};
	float stateGradient[NETWORK_LOOPS + 1][NETWORK_SIZE]{}; // including input state

	float preoutputGradient[OUTPUT_SIZE]{};
	float outputGradient[OUTPUT_SIZE]{};

	BRNNGradients()
	{
		Reset();
	}

	void Reset()
	{
		for (int i = 0; i < NETWORK_SIZE; i++) stateGradient[0][i] = 0;
	}

	void Assign(BRNNValues* Values)
	{
		values = Values;
	}

	void ForwardPropagate(float* Input, float* Output)
	{
		int layer, parentNode, childNode;

		for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++) stateGradient[NETWORK_LOOPS][parentNode] = stateGradient[0][parentNode];

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++) outputGradient[parentNode] = LogCostGradient(values->output[parentNode], Output[parentNode]);

		SoftmaxGradient(values->preoutput, preoutputGradient);

		for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
		{
			preoutputGradient[parentNode] *= outputGradient[parentNode];
			outputBiasGradients[parentNode] += preoutputGradient[parentNode];
		}

		for (layer = NETWORK_LOOPS - 1; layer >= 0; layer--)
		{
			for (parentNode = 0; parentNode < NETWORK_SIZE; parentNode++)
			{
				for (childNode = 0; childNode < OUTPUT_SIZE; childNode++)
				{
					stateGradient[layer][parentNode] = preoutputGradient[childNode] * values->parameters->outputWeights[childNode][parentNode];
					outputWeightGradients[childNode][parentNode] += preoutputGradient[childNode] * values->state[layer][parentNode];
				}

				prestateGradient[layer][parentNode] = RELUGradient(values->prestate[layer][parentNode]) * stateGradient[layer][parentNode];

				stateBiasGradients[parentNode] += prestateGradient[layer][parentNode];

				for (childNode = 0; childNode < NETWORK_SIZE; childNode++) prestate[layer][parentNode] += state[layer][childNode] * parameters->stateWeights[parentNode][childNode];

				for (childNode = 0; childNode < INPUT_SIZE; childNode++) prestate[layer][parentNode] += Input[childNode] * parameters->inputWeights[parentNode][childNode];

				prestate[layer][parentNode] = parameters->stateBiases[parentNode];
			}
		}
	}
};