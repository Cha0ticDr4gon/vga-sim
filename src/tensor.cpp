#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <initializer_list>
#include "tensor.h"

namespace ten {

Tensor::Accessor::Accessor(const Tensor& tensor, std::vector<int> *indices, int index) : tensor{tensor} {
	this->indices = indices;
	indices->push_back(index);
	id = indices->size();
}

Tensor::Accessor::~Accessor() {
	if(id == tensor.dim()) delete indices;
}

Tensor::Accessor Tensor::Accessor::operator[](int index) const {
	return Tensor::Accessor(tensor, indices, index);
}

Tensor::Accessor::operator double&() const {
	return tensor._values[tensor.get_index(*indices)];
}

double Tensor::Accessor::operator=(double x) const {
	return tensor._values[tensor.get_index(*indices)] = x;
}

Tensor::Tensor(std::initializer_list<int> shape) {
	_size = 1;
	_offsets.push_back(1);
	std::initializer_list<int>::iterator it;
	for(it = shape.begin(); it < shape.end(); it++) _shape.push_back(*it), _size *= *it, _offsets.push_back(_size);
	_values = new double[_size];
	//Sanity check
	std::cout << "Created tensor of shape: (";
	for(int i = 0; i < _shape.size(); i++) std::cout << _shape[i] << (i < _shape.size() - 1 ? ", " : "");
	std::cout << ")" << std::endl;
}

Tensor::Tensor(const std::vector<int>& shape) {
	_size = 1;
	_offsets.push_back(1);
	for(int i = 0; i < shape.size(); i++) _shape.push_back(shape[i]), _size *= shape[i], _offsets.push_back(_size);
	_values = new double[_size];
}

Tensor::Tensor(std::initializer_list<int> shape, std::initializer_list<int> values) : Tensor(shape) {
	std::initializer_list<int>::iterator it;
	int i = 0;
	for(it = values.begin(); it < values.end(); it++) _values[i++] = *it;
}

Tensor::~Tensor() {
	std::cout << "this ig" << std::endl;
	delete[] _values;
}

//Only printing errors for now because I'm stupid and don't know the error throwing etiquette of C++ yet
//Inshallah I don't make any indexing mistakes
int Tensor::get_index(std::initializer_list<int> l) const {
	int index = 0, running = 1;
	std::initializer_list<int>::iterator it;
	int i = _shape.size() - 1;
	//Should make last index the first to be accounted for
	for(it = std::prev(l.end()); it != l.begin(); it = std::prev(it)) {
		if(i <= 0) std::cout << "ERROR::TENSOR::SHAPE_INDEX_OUT_OF_RANGE" << std::endl;
		//Gonna allow negative indices
		index += *it >= 0 ? *it * running : (_shape[i] + *it) * running;
		running *= _shape[i--];
	}
	index += *it * running;
	if(i > 0) std::cout << "ERROR::TENSOR::SHAPE_LARGER_THAN_INDEX_COUNT" << std::endl;
	return index;
}

//TODO: Consolidate this with the other indexing method
int Tensor::get_index(std::vector<int>& l) const {
	int index = 0, running = 1;
	std::vector<int>::iterator it;
	int i = _shape.size() - 1;
	//Should make last index the first to be accounted for
	for(it = std::prev(l.end()); it != l.begin(); it = std::prev(it)) {
		if(i <= 0) std::cout << "ERROR::TENSOR::SHAPE_INDEX_OUT_OF_RANGE" << std::endl;
		//Gonna allow negative indices
		index += *it >= 0 ? *it * running : (_shape[i] + *it) * running;
		running *= _shape[i--];
	}
	index += *it * running;
	if(i > 0) std::cout << "ERROR::TENSOR::SHAPE_LARGER_THAN_INDEX_COUNT" << std::endl;
	return index;
}

double Tensor::get_value(std::initializer_list<int> l) const {
	return _values[get_index(l)];
}

void Tensor::set_value(std::initializer_list<int> l, double value) {
	_values[get_index(l)] = value;
}

int Tensor::shape(int i) const {
	return _shape[i];
}

int Tensor::dim() const {
	return _shape.size();
}

int Tensor::rows() const {
	return _shape[_shape.size() - 2];
}

int Tensor::cols() const {
	return _shape[_shape.size() - 1];
}

void Tensor::print() const {
	int i = 0;
	print(0, i);
	std::cout << std::endl;
}

void Tensor::print(int depth, int& x) const {
	if(depth < _shape.size() - 1) {
		for(int i = 0; i < depth; i++) std::cout << "  ";
		std::cout << "[" << std::endl;
		for(int i = 0; i < _shape[depth]; i++) {
			print(depth + 1, x);
			if(i < _shape[depth] - 1) std::cout << "," << std::endl;
		}
		std::cout << std::endl;
		for(int i = 0; i < depth; i++) std::cout << "  ";
		std::cout << "]";
	}
	else {
		for(int i = 0; i < depth; i++) std::cout << "  ";
		std::cout << "[";
		for(int i = 0; i < _shape[depth] - 1; i++) std::cout << _values[x++] << ", ";
		std::cout << _values[x++] << "]";
	}
}

double Tensor::operator()(std::initializer_list<int> l) const {
	return _values[get_index(l)];
}

double& Tensor::operator()(std::initializer_list<int> l) {
	return _values[get_index(l)];
}

double& Tensor::operator()(std::vector<int>& l) {
	return _values[get_index(l)];
}

Tensor::Accessor Tensor::operator[](int index) const {
	return Tensor::Accessor(*this, new std::vector<int>(), index);
}

//I took some creative liberties with this
void multiply(const Tensor& t1, const Tensor& t2, Tensor& dest) {
	//Dimensions must be the same
	if(t1.dim() != t2.dim()) std::cout << "ERROR::TENSOR::MULTIPLY: 1" << std::endl;
	if(dest.dim() != t1.dim()) std::cout << "ERROR::TENSOR::MULTIPLY: 2" << std::endl;
	//Row/col for multiplied matrices must be the same
	if(t1.cols() != t2.rows()) std::cout << "ERROR::TENSOR::MULTIPLY: 3" << std::endl;
	//Destination tensor must be compatible
	if(dest.rows() != t1.rows()) std::cout << "ERROR::TENSOR::MULTIPLY: 4" << std::endl;
	if(dest.cols() != t2.cols()) std::cout << "ERROR::TENSOR::MULTIPLY: 5" << std::endl;
	//There should be a check to ensure all other indices are the same, but I'm lazy rn
	//TODO: Write that check
	//All indices ahead are considered matrix slices
	int num_slices = 1;
	for(int i = t1.dim() - 2; i >= 0; i--) num_slices *= t1.shape(i);
	//Multiply each slice (should be two compatible matrices to multiply)
	int t1_slice_size = t1.rows() * t1.cols();
	int t2_slice_size = t2.rows() * t2.cols();
	int dest_slice_size = dest.rows() * dest.cols();
	for(int slice = 0; slice < num_slices; slice++) {
		int t1_s = slice * t1_slice_size;
		int t2_s = slice * t2_slice_size;
		int dest_s = slice * dest_slice_size;
		for(int r = 0; r < t1.rows(); r++) {
			for(int c = 0; c < t2.cols(); c++) {
				double sum = 0;
				for(int i = 0; i < t1.cols(); i++) {
					sum += t1._values[t1_s + r * t1.cols() + i] * t2._values[t2_s + i * t2.cols() + c];
				}
				dest._values[dest_s + r * dest.cols() + c] = sum;
			}
		}
	}
}

//TODO: Exceptions
void copy(const Tensor& from, Tensor& to) {
	if(from.dim() != to.dim()) std::cout << "ERROR::TENSOR::COPY: Dimensions don't match" << std::endl;
	int size = 1;
	for(int i = 0; i < from.dim(); i++) {
		if(from.shape(i) != to.shape(i)) std::cout << "ERROR::TENSOR::COPY: Shape mismatch at index: " << i << std::endl;
		size *= from.shape(i);
	}
	std::memcpy(to._values, from._values, size);
}

void init(const Tensor& dest, double value) {
	for(int i = 0; i < dest._size; i++) dest._values[i] = value;
}

}
