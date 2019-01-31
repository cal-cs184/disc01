#include "image.h"
#include "lodepng.h"


Image::Image() {
	width = height = 0;
}

Image::Image(const std::string& filename) {
	read(filename);
}

Image::Image(const unsigned& w, const unsigned& h)
: width(w), height(h), data(4 * w * h) {

}

void Image::read(std::string filename) {
	data.clear();
	lodepng::decode(data, width, height, filename);	
}

void Image::write(std::string filename) const {
	lodepng::encode(filename, data, width, height);
}

uint8_t* Image::at(int x, int y) {
	return &data[0] + 4 * (y * width + x);
}

Image Image::operator*(const Filter& filter) {
	Image new_image(width, height);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float temp[] = {0,0,0,0};
			for (int j = 0; j < filter.height; ++j) {
				for (int i = 0; i < filter.width; ++i) {
					int xi = x + i - filter.width/2, yi = y + j - filter.height/2;
					if (xi < 0 || xi >= width || yi < 0 || yi >= height)
						continue;
					else {
						for (int k = 0; k < 4; ++k)
							temp[k] += at(xi,yi)[k] * filter.at(i,j);
					}
				}
			}
			for (int k = 0; k < 4; ++k) {
				new_image.at(x,y)[k] = temp[k];
			}
		}
	}
	return new_image;
}


