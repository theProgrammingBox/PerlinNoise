#pragma once

#include "BRNNConfig.h"

const int memoryLag = 3;

class Environment
{
public:
	Random random;
	bool value;
	bool memory[memoryLag];
	int memoryIndex;

	Environment()
	{
		random = Random();

		Reset();
	}

	void Reset()
	{
		memoryIndex = 0;

		for (int i = 0; i < memoryLag; i++)
		{
			memory[i] = 0;
		}
	}

	void ForwardPropagate()
	{
		memory[memoryIndex] = value;
		memoryIndex = memoryIndex + 1 == memoryLag ? 0 : memoryIndex + 1;
	}

	void GetInput(float* input)
	{
		value = random.UIntRandom() & 1;

		for (int node = 0; node < INPUT_SIZE; node++)
		{
			input[node] = value == node;
		}
	}

	void GetOutput(float* output)
	{
		for (int node = 0; node < OUTPUT_SIZE; node++)
		{
			output[node] = memory[memoryIndex] == node;
		}
	}
};