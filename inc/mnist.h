#ifndef _MNIST_H_
#define _MNIST_H_

#include <string>

class MnistImageSet {
private:
	uint32_t num_images;
	uint32_t num_labels;
	uint32_t image_height;
	uint32_t image_width;

	uint8_t *image_data;
	uint8_t *label_data;

	void read_image_file(const std::string& image_file);
	void read_label_file(const std::string& label_file);

public:
	MnistImageSet(const std::string& image_file, const std::string& label_file);
	~MnistImageSet();
	uint32_t get_width();
	uint32_t get_height();
	uint8_t *get_image(int index);
	uint8_t get_label(int index);
};

#endif
