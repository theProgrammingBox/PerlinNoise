#pragma once

#include "Environment.h"
#include "BRNNGradients.h"

class BRNNTrainer
{
public:
	Environment environment;
	BRNNParameters parameters;
	BRNNValues valuesArray[SEQUENCE_LENGTH + 1];
	BRNNGradients gradients;

	float input[NETWORK_LOOPS][INPUT_SIZE];
	float output[NETWORK_LOOPS][OUTPUT_SIZE];
	float networkOutput[NETWORK_LOOPS][OUTPUT_SIZE];

	BRNNTrainer()
	{
		parameters.Reset();
	}

	void Train(int iterations)
	{
		int iteration, batch, sequence;
		int node;
		float AverageError;

		AverageError = 0;

		for (iteration = 0; iteration < iterations; iteration++)
		{

			for (batch = 0; batch < BATCH_SIZE; batch++)
			{
				environment.Reset();
				valuesArray[0].Reset();
				gradients.Reset();

				for (sequence = 0; sequence < SEQUENCE_LENGTH; sequence++)
				{
					environment.GetInput(input[sequence]);
					environment.GetOutput(output[sequence]);
					valuesArray[sequence + 1] = valuesArray[sequence];
					valuesArray[sequence + 1].ForwardPropagate(input[sequence], networkOutput[sequence]);
					environment.ForwardPropagate();

					for (node = 0; node < OUTPUT_SIZE; node++)
					{
						AverageError += abs(output[sequence][node] - networkOutput[sequence][node]); //Cost(networkOutput[sequence][node], output[sequence][node]); // Using the cost function I am using the gradient of for training
					}

					//										cout << "input:\n";
					//										for (node = 0; node < OUTPUT_NODES; node++)
					//										{
					//											cout << input[sequence][node] << " ";
					//										}
					//										cout << endl;
					//
					//										cout << "networkOutput:\n";
					//										for (node = 0; node < OUTPUT_NODES; node++)
					//										{
					//											cout << networkOutput[sequence][node] << " ";
					//										}
					//										cout << endl;
					//
					//										cout << "output:\n";
					//										for (node = 0; node < OUTPUT_NODES; node++)
					//										{
					//											cout << output[sequence][node] << " ";
					//										}
					//										cout << endl;
					//										cout << endl;
				}

				for (sequence = SEQUENCE_LENGTH; sequence > 0; sequence--)
				{
					gradients.Assign(&valuesArray[sequence]);
					gradients.BackPropagate(input[sequence], output[sequence]);
				}
			}

			gradients.UpdateParameters(LEARNING_RATE / BATCH_SIZE);
		}

		cout << "AVERAGE ERROR: " << AverageError / (iterations * BATCH_SIZE * SEQUENCE_LENGTH * OUTPUT_SIZE) << endl;
	}
};