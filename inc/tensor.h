#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <vector>
#include <initializer_list>

namespace ten {

class Tensor {
private:
	std::vector<int> _shape;
	std::vector<int> _offsets;
	int _size;
	double *_values;
	int get_index(std::initializer_list<int> l) const;
	int get_index(std::vector<int>& l) const;
	void print(int depth, int& i) const;
	int rows() const;
	int cols() const;

	class Accessor {
	private:
		std::vector<int> *indices;
		const Tensor& tensor;
		int id;
	public:
		Accessor(const Tensor& tensor, std::vector<int> *indices, int index);
		~Accessor();
		Accessor operator[](int index) const;
		operator double&() const;
		double operator=(double x) const;
	};

public:
	Tensor(std::initializer_list<int> shape);
	Tensor(const std::vector<int>& shape);
	Tensor(std::initializer_list<int> shape, std::initializer_list<int> values);
	~Tensor();
	double get_value(std::initializer_list<int> l) const;
	void set_value(std::initializer_list<int> l, double value);
	int shape(int i) const;
	int dim() const;
	//Gonna need to think on these since they are specifically for 2D tensors and up
	void print() const;
	double operator()(std::initializer_list<int> l) const;
	double& operator()(std::initializer_list<int> l);
	double& operator()(std::vector<int>& l);
	Accessor operator[](int index) const;
	friend void multiply(const Tensor& t1, const Tensor& t2, Tensor& dest);
	friend void copy(const Tensor& from, Tensor& to);
	friend void init(const Tensor& dest, double value);
};

void multiply(const Tensor& t1, const Tensor& t2, Tensor& dest);
void copy(const Tensor& from, Tensor& to);
void init(const Tensor& dest, double value);

}

#endif
