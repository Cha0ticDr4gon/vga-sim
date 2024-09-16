#include <vector>
#include <initializer_list>
#include "tensor.h"
#include "container.h"

namespace ten {
	Container::Container() {}

	Container::~Container() {
		for(int i = 0; i < layers.size(); i++) delete layers[i];
	}

	void Container::add(std::initializer_list<int> l) {
		auto tensor = new Tensor(l);
		init(*tensor, 0);
		tensor->print();
		layers.push_back(tensor);
	}

	Tensor& Container::operator[](int i) {
		return *layers[i < 0 ? layers.size() + i : i];
	}
}
