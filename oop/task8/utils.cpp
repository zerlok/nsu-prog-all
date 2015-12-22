#include <iostream>
#include <vector>
#include <png++/palette.hpp>
#include "utils.h"


template<typename T>
T count_percent(const T &a, const T &b)
{
	return ((a * 100) / b);
}


short pngutils::count_brightness(const ImagePNG::pixel_t &pixel)
{
	return (pngconsts::red_color_brightness_ratio * pixel.red
			+ pngconsts::green_color_brightness_ratio * pixel.green
			+ pngconsts::blue_color_brightness_ratio * pixel.blue);
}


int pngutils::count_average(const ImagePNG::pixel_t &p)
{
	return (p.red + p.green + p.blue) / 3;
}


ImagePNG pngfilters::build_grayscale_image(const ImagePNG &img)
{
	ImagePNG graysclaed(img.get_width(), img.get_height());

	for (ImagePNG::const_iterator pit = img.cbegin();
		 pit != img.cend();
		 ++pit)
	{
		int average = pngutils::count_average(*pit);
		graysclaed.set_pixel(
				pit.get_x(),
				pit.get_y(),
				ImagePNG::pixel_t(average, average, average)
		);
	}

	return graysclaed;
}


std::vector<int> pngutils::get_histogram(const ImagePNG &img)
{
	std::vector<size_t> bins = std::vector<size_t>(pngconsts::palette_size, 0);
	std::vector<int> histogram;
	size_t max = 0;

	for (const ImagePNG::pixel_t pixel : img)
	{
		size_t i = pngutils::count_brightness(pixel);
		++bins[i];

		if (bins[i] > max)
			max = bins[i];
	}

	for (const size_t bin : bins)
		histogram.push_back(count_percent(bin, max));

	return histogram;
}


ImagePNG pngfilters::build_histogram_image(const ImagePNG &img)
{
	ImagePNG histogram_img(pngconsts::palette_size, pngconsts::histogram_height);
	std::vector<int> histogram = pngutils::get_histogram(img);

	for (size_t x = 0; x < histogram.size(); ++x)
		for (int y = (pngconsts::histogram_height - 1);
			 y >= int(pngconsts::histogram_height - histogram[x]);
			 --y)
			histogram_img.set_pixel(x, y, pngconsts::white_pixel);

	return histogram_img;
}
