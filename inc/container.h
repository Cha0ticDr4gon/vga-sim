#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <vector>
#include <initializer_list>
#include "tensor.h"

namespace ten {
	class Container {
	private:
		std::vector<Tensor*> layers;
	public:
		Container();
		~Container();
		Tensor& operator[](int i);
		void add(std::initializer_list<int> l);
	};
}

#endif
