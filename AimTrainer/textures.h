#pragma once
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void LoadTexture(std::string filename, unsigned int &texture_indexes) {
	int width, height, cnt;
	unsigned char* color_data = stbi_load(filename.c_str(), &width, &height, &cnt, 0);

	if (color_data == nullptr) {
		std::cout << "Can't load texture \"" << filename << "\"!" << std::endl;
		return;
	}

	glGenTextures(1, &texture_indexes);
	glBindTexture(GL_TEXTURE_2D, texture_indexes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, color_data);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(color_data);
}

class Font256 {
public:
	unsigned int texture_id = 0;
	float* width_array;

	Font256() {
		width_array = new float[256];
		for (int i = 0; i < 256; i++) width_array[i] = 1.2f;
	}

	~Font256() {
		delete[] width_array;
	}
};

void LoadFont(std::string filename, Font256 &font, int color_byte) {
	int width, height, cnt;
	unsigned char* color_data = stbi_load(filename.c_str(), &width, &height, &cnt, 0);

	if (color_data == nullptr) {
		std::cout << "Can't load texture \"" << filename << "\"!" << std::endl;
		return;
	}

	glGenTextures(1, &font.texture_id);

	int pixel_per_char = width / 16;
	for (int k = 0; k < 256; k++) {
		int x = (k % 16) * pixel_per_char;
		int y = (k / 16) * pixel_per_char;
		int i = 0;
		unsigned char alpha = 0;

		for (i = x + pixel_per_char - 1; i > x; i--) {
			for (int j = y + pixel_per_char - 1; j > y; j--) {
				alpha = color_data[(j * width + i) * cnt + color_byte];
				if (alpha > 0) break;
			}
			if (alpha > 0) break;
		}

		i += pixel_per_char / 10.0f;
		if (i > x + pixel_per_char - 1) i = x + pixel_per_char - 1;
		if (k == 32) i = (x + pixel_per_char / 1.5f);

		font.width_array[k] = (i - x) / (float)pixel_per_char;
	}

	glBindTexture(GL_TEXTURE_2D, font.texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, color_data);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(color_data);
}