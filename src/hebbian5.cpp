#include <iostream>
#include <vector>
#include <initializer_list>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdint>
#include "tensor.h"
#include "container.h"

inline double sigmoid(double x);

class NeuralNetwork {
private:
	ten::Container _outputs;
	ten::Container _weights;
	ten::Container _derivatives;
	ten::Container _deltas;
	//Maybe redundant, but it might be nice just having layer sizes tracked here
	std::vector<int> _sizes;
	//One less than network_size because it's just the number of output layers
	int input_size;
	int num_layers;
public:
	NeuralNetwork(std::initializer_list<int> sizes);
	void forward(ten::Tensor& inputs);
	void backprop(ten::Tensor& targets);
};

NeuralNetwork::NeuralNetwork(std::initializer_list<int> sizes) {
	std::initializer_list<int>::iterator it;
	for(it = sizes.begin(); it != sizes.end(); it = std::next(it)) {
		_sizes.push_back(*it);
		_outputs.add({1, *it});
		_derivatives.add({1, *it});
	}
	num_layers = _sizes.size();
	for(int i = 1; i < _sizes.size(); i++) {
		_weights.add({_sizes[i - 1], _sizes[i]});
		_deltas.add({_sizes[i - 1], _sizes[i]});
	}
	std::cout << "Created network of shape: (";
	for(int i = 0; i < _sizes.size(); i++) std::cout << _sizes[i] << (i < _sizes.size() - 1 ? ", " : "");
	std::cout << ")" << std::endl;
}

void NeuralNetwork::forward(ten::Tensor& inputs) {
	//Checks that should throw exceptions but I'm too lazy
	if(inputs.shape(1) != _sizes[0]) std::cout << "ERROR::NEURAL_NETWORK::FORWARD: inputs.shape(1) mismatch!" << std::endl;
	if(inputs.shape(0) != 1) std::cout << "ERROR::NEURAL_NETWORK::FORWARD: inputs.shape(0) != 1" << std::endl;

	//Copy because I'm lazy writing this algorithm with no reference to the input tensor
	//TODO: Rewrite to get rid of copy at some point
	ten::copy(inputs, _outputs[0]);
	std::cout << "Finished copying" << std::endl;

	for(int i = 1; num_layers; i++) {
		std::cout << "Forward: " << (i - 1) << ", " << i << std::endl;
		_outputs[i - 1].print();
		_outputs[i].print();
		ten::multiply(_outputs[i - 1], _weights[i], _outputs[i]);
		std::cout << "Finished multiplying" << std::endl;
		for(int j = 0; j < _outputs[i].shape(0); j++) _outputs[i][j] = sigmoid(_outputs[i][j]);
	}
}

void NeuralNetwork::backprop(ten::Tensor& targets) {
	const double eta = 0.3;
	//Compute sigmoid derivatives for each neuron
	for(int i = 0; i < num_layers; i++) {
		for(int j = 0; j < _outputs[0].shape(0); j++) _derivatives[i][j] = _outputs[i][j] * (1 - _outputs[i][j]);
	}
	//Calculate the weight adjustments
	//This actually seems incorrect, so I will need to figure out why
	//- Update: It's correct if you consider weights are all updated after this step
	//- Do the math. It makes sense!
	//TODO: Add negative indexing to container to make it more succinct
	//TODO: Decide if you want to copy inputs to initial output layer
	for(int i = 0; i < _sizes[-1]; i++) _deltas[-1][i] = (targets[i] - _outputs[-1][i]) * _derivatives[-1][i];
	//TODO: Check the indices on this
	for(int i = num_layers - 2; i >= 0; i--) {
		for(int j = 0; j < _sizes[i + 1]; j++) {
			double sum = 0;
			for(int k = 0; k < _sizes[i]; k++) sum += _weights[i + 1][j][k] * _deltas[i + 1][k];
			_deltas[i][j] = sum * _derivatives[i][j];
		}
	}
	//TODO: Rewrite this entire function because it's 5:10AM. Your brain is shutting down
	for(int i = 0; i < num_layers; i++) {
		for(int j = 0; j < _sizes[i]; j++) {
			for(int k = 0; k < _sizes[i + 1]; k++) _weights[i - 1][j][k] = -eta * _deltas[i][j] * _outputs[i][k];
		}
	}
}

inline double sigmoid(double x) {
	return 1.0 / (1.0 + std::exp(-x));
}

int main() {
	auto nn = NeuralNetwork({2, 2, 1});
	auto input = ten::Tensor({1, 2});
	input[0][0] = 1;
	input[0][1] = 0;
	nn.forward(input);
	std::cout << "Got here" << std::endl;
	return 0;
}
