#pragma once
#include <math.h>
#include <algorithm>
#include <iostream>
#include "Randoms.h"
#include "OpenSimplex.h"

using std::max;
using std::cout;
using std::endl;

const int NETWORK_SIZE = 10;
const int NETWORK_LOOPS = 10;

const int INPUT_SIZE = 1;
const int OUTPUT_SIZE = 2;

const int SEQUENCE_LENGTH = 4;
const int BATCH_SIZE = 10;
const int LEARNING_RATE = 0.01;

float RELU(float x)
{
	return x < 0 ? 0 : x;
}

float RELUGradient(float x)
{
	return x < 0 ? 0 : 1;
}

const void Softmax(float* input, float* output)
{
	int node;
	float largestValue;
	float total;

	largestValue = input[0];

	for (node = 1; node < INPUT_SIZE; node++)
	{
		if (largestValue < input[node])
		{
			largestValue = input[node];
		}
	}

	total = 0;

	for (node = 0; node < OUTPUT_SIZE; node++)
	{
		output[node] = exp(input[node] - largestValue);
		total += output[node];
	}

	for (node = 0; node < OUTPUT_SIZE; node++)
	{
		output[node] /= total;
	}
}

const void SoftmaxGradient(float* input, float* output)
{
	int parentNode, childNode;
	float largestValue;
	float total;
	float numerator[INPUT_SIZE]{};

	largestValue = input[0];

	for (parentNode = 1; parentNode < INPUT_SIZE; parentNode++)
	{
		if (largestValue < input[parentNode])
		{
			largestValue = input[parentNode];
		}
	}

	total = 0;

	for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
	{
		output[parentNode] = input[parentNode] - largestValue;
		total += exp(output[parentNode]);
	}

	total *= total;

	for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
	{
		for (childNode = 0; childNode < OUTPUT_SIZE; childNode++)
		{
			if (parentNode != childNode)
			{
				numerator[parentNode] += exp(output[parentNode] + output[childNode]);
			}
		}
	}

	for (parentNode = 0; parentNode < OUTPUT_SIZE; parentNode++)
	{
		output[parentNode] = numerator[parentNode] / total;
	}
}

const float LogCost(float output, float expected) // Expected must be 1 or 0
{
	if (expected == 1)
	{
		if (output == 1)
		{
			return 0;
		}
		else if (output < 0.001)
		{
			return 5.90875527898;
		}
		else
		{
			return output - log(output) - 1;
		}
	}
	else if (expected == 0)
	{
		if (output == 0)
		{
			return 0;
		}
		else if (output > 0.999)
		{
			return 5.90875527898;
		}
		else
		{
			return -output - log(1 - output);
		}
	}
}

const float LogCostGradient(float output, float expected) // Expected must be 1 or 0
{
	if (expected == 1)
	{
		if (output == 1)
		{
			return 0;
		}
		else if (output < 0.001)
		{
			return 999;
		}
		else
		{
			return 1 / output - 1;
		}
	}
	else if (expected == 0)
	{
		if (output == 0)
		{
			return 0;
		}
		else if (output > 0.999)
		{
			return -999;
		}
		else
		{
			return output / (output - 1);
		}
	}
}