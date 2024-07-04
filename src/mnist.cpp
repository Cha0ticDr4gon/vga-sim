#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include "mnist.h"

MnistImageSet::MnistImageSet(const std::string& image_file, const std::string& label_file) {
	read_image_file(image_file);
	read_label_file(label_file);

	if(num_images != num_labels) {
		std::cout << "ERROR::MNIST::IMAGE_LABEL_COUNT_MISMATCH: num_images = " << num_images << ", num_labels = " << num_labels  << std::endl;
	}
}

MnistImageSet::~MnistImageSet() {
	delete[] image_data;
	delete[] label_data;
}

static uint32_t read_uint32(std::ifstream& file_in) {
	char buffer[4];
	file_in.read(buffer, 4);
	return (static_cast<uint32_t>(buffer[0]) << 24) | (static_cast<uint32_t>(buffer[1]) << 16) | (static_cast<uint32_t>(buffer[2]) << 8) | (static_cast<uint32_t>(buffer[3]));
}

void MnistImageSet::read_image_file(const std::string& image_file) {
	std::ifstream file_in;
	file_in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file_in.open(image_file, std::ifstream::binary);

		uint32_t magic_number = read_uint32(file_in);
		if(magic_number != 2051) {
			std::cout << "ERROR::MNIST::IMAGE_FILE::INVALID_MAGIC_NUMBER: " << magic_number << std::endl;
			return;
		}

		num_images = read_uint32(file_in);
		image_height = read_uint32(file_in);
		image_width = read_uint32(file_in);

		std::cout << "MNIST::IMAGE_FILE: num_images = " << num_images << ", image_width = " << image_width << ", image_height = " << image_height << std::endl;

		size_t buffer_size = image_width * image_height * num_images;
		image_data = new uint8_t[buffer_size];
		file_in.read(reinterpret_cast<char *>(image_data), buffer_size);

		file_in.close();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::MNIST::IMAGE_FILE::READ_FAILED: " << image_file << std::endl;
	}
}

void MnistImageSet::read_label_file(const std::string& label_file) {
	std::ifstream file_in;
	file_in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file_in.open(label_file, std::ifstream::binary);

		uint32_t magic_number = read_uint32(file_in);
		if(magic_number != 2049) {
			std::cout << "ERROR::MNIST::LABEL_FILE::INVALID_MAGIC_NUMBER: " << magic_number << std::endl;
			return;
		}

		num_labels = read_uint32(file_in);

		std::cout << "MNIST::LABEL_FILE: num_labels = " << num_labels << std::endl;

		label_data = new uint8_t[num_labels];
		file_in.read(reinterpret_cast<char *>(label_data), num_labels);

		file_in.close();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::MNIST::LABEL_FILE::READ_FAILED: " << label_file << std::endl;
	}
}

uint32_t MnistImageSet::get_width() {
	return image_width;
}

uint32_t MnistImageSet::get_height() {
	return image_height;
}

uint8_t *MnistImageSet::get_image(int index) {
	if(index < 0 || index >= num_images) return nullptr;
	size_t image_size = image_width * image_height;
	uint8_t *buffer = new uint8_t[image_size * 3];
	size_t buffer_i = 0;
	for(size_t i = image_size * index; i < image_size * (index + 1); i++) {
		buffer[buffer_i++] = image_data[i];
		buffer[buffer_i++] = image_data[i];
		buffer[buffer_i++] = image_data[i];
	}
	return buffer;
}

uint8_t MnistImageSet::get_label(int index) {
	return label_data[index];
}
