#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <ctime>



////////////////////////////
// Image processing stuff //
////////////////////////////
Pixel::Pixel(const Pixel32& p)
{
}
Pixel32::Pixel32(const Pixel& p)
{
}

int Image32::AddRandomNoise(const float& noise,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	srand(time(NULL));

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			float randNum;

            //noisify r
			randNum = ((rand() % 511) -255)*noise;
			int newr = (*this)(x,y).r + randNum;
			if (newr < 0) { outputImage(x, y).r = 0; }
			else if (newr > 255) { outputImage(x, y).r = 255; }
			else { outputImage(x, y).r = newr; }

			//noisify g
			randNum = ((rand() % 511) - 255)*noise;
			int newg = (*this)(x, y).g + randNum;
			if (newg < 0) { outputImage(x, y).g	= 0; }
			else if (newg > 255) { outputImage(x, y).g = 255; }
			else { outputImage(x, y).g = newg; }

			//noisify b
			randNum = ((rand() % 511) - 255)*noise;
			int newb = (*this)(x, y).b + randNum;
			if (newb < 0) { outputImage(x, y).b = 0; }
			else if (newb > 255) { outputImage(x, y).b = 255; }
			else { outputImage(x, y).b = newb; }
		}
	}

	return 1;
}
int Image32::Brighten(const float& brightness,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			//brighten r
			int newr = (*this)(x, y).r * brightness;
			if (newr < 0) { outputImage(x, y).r = 0; }
			else if (newr > 255) { outputImage(x, y).r = 255; }
			else { outputImage(x, y).r = newr; }

			//brighten g
			int newg = (*this)(x, y).g * brightness;
			if (newg < 0) { outputImage(x, y).g = 0; }
			else if (newg > 255) { outputImage(x, y).g = 255; }
			else { outputImage(x, y).g = newg; }

			//brighten b
			int newb = (*this)(x, y).b * brightness;
			if (newb < 0) { outputImage(x, y).b = 0; }
			else if (newb > 255) { outputImage(x, y).b = 255; }
			else { outputImage(x, y).b = newb; }
		}
	}

	return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			int lum = (*this)(x, y).r * .3 + (*this)(x, y).g * .59 + (*this)(x, y).b * .11;
			
			outputImage(x, y).r = lum;
			outputImage(x, y).g = lum;
			outputImage(x, y).b = lum;
		}
	}

	return 1;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	float avgImgLum = 0;

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			float lum = (*this)(x, y).r * .3 + (*this)(x, y).g * .59 + (*this)(x, y).b * .11;
			avgImgLum += lum / ((*this).width()*(*this).height()*1.0);
		}
	}

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			//contrast r
			int newr = avgImgLum + ((*this)(x, y).r - avgImgLum) * contrast;
			if (newr < 0) { outputImage(x, y).r = 0; }
			else if (newr > 255) { outputImage(x, y).r = 255; }
			else { outputImage(x, y).r = newr; }

			//contrast g
			int newg = avgImgLum + ((*this)(x, y).g - avgImgLum) * contrast;
			if (newg < 0) { outputImage(x, y).g = 0; }
			else if (newg > 255) { outputImage(x, y).g = 255; }
			else { outputImage(x, y).g = newg; }

			//contrast b
			int newb = avgImgLum + ((*this)(x, y).b - avgImgLum) * contrast;
			if (newb < 0) { outputImage(x, y).b = 0; }
			else if (newb > 255) { outputImage(x, y).b = 255; }
			else { outputImage(x, y).b = newb; }
		}
	}

	return 1;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			int lum = (*this)(x, y).r * .3 + (*this)(x, y).g * .59 + (*this)(x, y).b * .11;

			//saturate r
			int newr = lum + ((*this)(x, y).r - lum) * saturation;
			if (newr < 0) { outputImage(x, y).r = 0; }
			else if (newr > 255) { outputImage(x, y).r = 255; }
			else { outputImage(x, y).r = newr; }

			//saturate g
			int newg = lum + ((*this)(x, y).g - lum) * saturation;
			if (newg < 0) { outputImage(x, y).g = 0; }
			else if (newg > 255) { outputImage(x, y).g = 255; }
			else { outputImage(x, y).g = newg; }

			//saturate b
			int newb = lum + ((*this)(x, y).b - lum) * saturation;
			if (newb < 0) { outputImage(x, y).b = 0; }
			else if (newb > 255) { outputImage(x, y).b = 255; }
			else { outputImage(x, y).b = newb; }
		}
	}

	return 1;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	//A level's size is 2^(8-b), as there are 2^b levels: {0, ..., 2^b-1}
	//Level n has pixel value n*255/(2^b-1). I call 255/(2^b-1) the level multiplier.
	float levelMultiplier = 255 / (0xff >> (8 - bits));

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			outputImage(x, y).r = ((*this)(x, y).r >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).g = ((*this)(x, y).g >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).b = ((*this)(x, y).b >> (8 - bits)) * levelMultiplier;
		}
	}

	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	float levelMultiplier = 255 / (0xff >> (8 - bits));

	srand(time(NULL));

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			float randNum; // randNum will be a random number between [-2^(8-b), 2^(8-b)], just enough to enable any pixel to round either up or down 

			//noisify r
			randNum = (rand() % ((int)(0x200 >> bits) + 1)) - (0x200 >> (bits + 1));
			int newr = (*this)(x, y).r + randNum;
			if (newr < 0) { newr = 0; }
			else if (newr > 255) { newr = 255; }

			//noisify g
			randNum = (rand() % ((int)(0x200 >> bits) + 1)) - (0x200 >> (bits + 1));
			int newg = (*this)(x, y).g + randNum;
			if (newg < 0) { newg = 0; }
			else if (newg > 255) { newg = 255; }

			//noisify b
			randNum = (rand() % ((int)(0x200 >> bits) + 1)) - (0x200 >> (bits + 1));
			int newb = (*this)(x, y).b + randNum;
			if (newb < 0) { newb = 0; }
			else if (newb > 255) { newb = 255; }

			//quantize
			outputImage(x, y).r = (newr >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).g = (newg >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).b = (newb >> (8 - bits)) * levelMultiplier;
		}
	}

	return 1;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	double dither22 [4] = { .2, .6, .8, .4 };

	float levelMultiplier = 255 / (0xff >> (8 - bits));

	for (int x = 0; x < outputImage.width(); x++)
	{
		for (int y = 0; y < outputImage.height(); y++)
		{
			int i = x % 2;
			int j = y % 2;

			//Note: casting a double to an int floors it
			outputImage(x, y).r = ((int)((*this)(x, y).r / 256.0 * (0xff >> (8 - bits)) + dither22[j + 2 * i])) * levelMultiplier;
			outputImage(x, y).g = ((int)((*this)(x, y).g / 256.0 * (0xff >> (8 - bits)) + dither22[j + 2 * i])) * levelMultiplier;
			outputImage(x, y).b = ((int)((*this)(x, y).b / 256.0 * (0xff >> (8 - bits)) + dither22[j + 2 * i])) * levelMultiplier;
		}
	}

	return 1;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	float levelMultiplier = 255 / (0xff >> (8 - bits));

	outputImage = (*this);

	//Notice the loop order is relevant
	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			int newr = outputImage(x, y).r;
			int newg = outputImage(x, y).g;
			int newb = outputImage(x, y).b;

			//quantize
			outputImage(x, y).r = (newr >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).g = (newg >> (8 - bits)) * levelMultiplier;
			outputImage(x, y).b = (newb >> (8 - bits)) * levelMultiplier;


			//diffuse error
			//not 100% of the error is diffused into the image for boundary pixels. I believe this makes sense.
			int adjusted;
			if (x + 1 < outputImage.width()) {
				adjusted = outputImage(x + 1, y).r + (newr - outputImage(x, y).r) * 7 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y).r = adjusted;

				adjusted = outputImage(x + 1, y).g + (newg - outputImage(x, y).g) * 7 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y).g = adjusted;

				adjusted = outputImage(x + 1, y).b + (newb - outputImage(x, y).b) * 7 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y).b = adjusted;
			}
			if (y + 1 < outputImage.height()) {
				adjusted = outputImage(x, y + 1).r + (newr - outputImage(x, y).r) * 5 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x, y + 1).r = adjusted;

				adjusted = outputImage(x, y + 1).g + (newg - outputImage(x, y).g) * 5 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x, y + 1).g = adjusted;

				adjusted = outputImage(x, y + 1).b + (newb - outputImage(x, y).b) * 5 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x, y + 1).b = adjusted;
			}
			if (x - 1 >= 0 && y + 1 < outputImage.height()) {
				adjusted = outputImage(x - 1, y + 1).r + (newr - outputImage(x, y).r) * 3 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x - 1, y + 1).r = adjusted;

				adjusted = outputImage(x - 1, y + 1).g + (newg - outputImage(x, y).g) * 3 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x - 1, y + 1).g = adjusted;

				adjusted = outputImage(x - 1, y + 1).b + (newb - outputImage(x, y).b) * 3 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x - 1, y + 1).b = adjusted;
			}
			if (x + 1 < outputImage.width() && y + 1 < outputImage.height()) {
				adjusted = outputImage(x + 1, y + 1).r + (newr - outputImage(x, y).r) * 1 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y + 1).r = adjusted;

				adjusted = outputImage(x + 1, y + 1).g + (newg - outputImage(x, y).g) * 1 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y + 1).g = adjusted;

				adjusted = outputImage(x + 1, y + 1).b + (newb - outputImage(x, y).b) * 1 / 16.0;
				if (adjusted < 0) { adjusted = 0; }
				else if (adjusted > 255) { adjusted = 255; }
				outputImage(x + 1, y + 1).b = adjusted;
			}
		}
	}

	return 1;
}

int Image32::Blur3X3(Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			double newr = (*this)(x, y).r * 2;
			double newg = (*this)(x, y).g * 2;
			double newb = (*this)(x, y).b * 2;
		    int normalizer = 2;

			for (int xmodifier = -1; xmodifier <= 1; xmodifier++) {
				for (int ymodifier = -1; ymodifier <= 1; ymodifier++) {
					if (0 <= x + xmodifier && x + xmodifier < outputImage.width() && 0 <= y + ymodifier && y + ymodifier < outputImage.height()) {
						int weight = (xmodifier * ymodifier == 0) ? 2 : 1;
						newr += (*this)(x + xmodifier, y + ymodifier).r * weight;
						newg += (*this)(x + xmodifier, y + ymodifier).g * weight;
						newb += (*this)(x + xmodifier, y + ymodifier).b * weight;
						normalizer += weight;
					}
				}
			}

			outputImage(x, y).r = newr / normalizer;
			outputImage(x, y).g = newg / normalizer;
			outputImage(x, y).b = newb / normalizer;
		}
	}

	return 1;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
	outputImage.setSize((*this).width(), (*this).height());

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			double newr = 0;
			double newg = 0;
			double newb = 0;
			int sum = 0;

			for (int xmodifier = -1; xmodifier <= 1; xmodifier++) {
				for (int ymodifier = -1; ymodifier <= 1; ymodifier++) {
					if (0 <= x + xmodifier && x + xmodifier < outputImage.width() && 0 <= y + ymodifier && y + ymodifier < outputImage.height()) {
						newr += (*this)(x + xmodifier, y + ymodifier).r * -1;
						newg += (*this)(x + xmodifier, y + ymodifier).g * -1;
						newb += (*this)(x + xmodifier, y + ymodifier).b * -1;
						sum -= 1;
					}
				}
			}

			newr += (*this)(x, y).r * -sum;
			if (newr < 0) { newr = 0; }
			else if (newr > 255) { newr = 255; }

			newg += (*this)(x, y).g * -sum;
			if (newg < 0) { newg = 0; }
			else if (newg > 255) { newg = 255; }

			newb += (*this)(x, y).b * -sum;
			if (newb < 0) { newb = 0; }
			else if (newb > 255) { newb = 255; }

			outputImage(x, y).r = newr;
			outputImage(x, y).g = newg;
			outputImage(x, y).b = newb;
		}
	}

	return 1;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
	int newwidth = (*this).width() * scaleFactor;
	int newheight = (*this).height() * scaleFactor;

	outputImage.setSize(newwidth, newheight);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			outputImage(x, y) = NearestSample(x / scaleFactor, y / scaleFactor);
		}
	}

	return 1;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
	int newwidth = (*this).width() * scaleFactor;
	int newheight = (*this).height() * scaleFactor;

	outputImage.setSize(newwidth, newheight);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			outputImage(x, y) = BilinearSample(x / scaleFactor, y / scaleFactor);
		}
	}

	return 1;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
	int newwidth = (*this).width() * scaleFactor;
	int newheight = (*this).height() * scaleFactor;

	outputImage.setSize(newwidth, newheight);

	//suggested Gaussian values from Piazza
	float radius = (scaleFactor >= 1) ? 1 : 1 / scaleFactor;
	float variance = .25*sqrtf(radius);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			outputImage(x, y) = GaussianSample(x / scaleFactor, y / scaleFactor, variance, radius);
		}
	}

	return 1;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
	float degreeangle = angle*0.0174532925;

	float leftpeek = (*this).height() * sin( degreeangle);
	float toppeek  = (*this).width()  * sin(-degreeangle);

	int newheight = ceil(fabs(toppeek)  + fabs((*this).height() * cos(degreeangle)));
	int newwidth  = ceil(fabs(leftpeek) + fabs((*this).width()  * cos(degreeangle)));

	outputImage.setSize(newwidth, newheight);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			float effectivex = (leftpeek < 0) ? x + leftpeek : x;
			float effectivey = (toppeek < 0) ? y + toppeek : y;
			
			float sourcex = effectivex * cos(degreeangle) - effectivey * sin(degreeangle);
			float sourcey = effectivex * sin(degreeangle) + effectivey * cos(degreeangle);
			outputImage(x, y) = NearestSample( sourcex, sourcey);
		}
	}

	return 1;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
	float degreeangle = angle*0.0174532925;

	float leftpeek = (*this).height() * sin(degreeangle);
	float toppeek = (*this).width()  * sin(-degreeangle);

	int newheight = ceil(fabs(toppeek) + fabs((*this).height() * cos(degreeangle)));
	int newwidth = ceil(fabs(leftpeek) + fabs((*this).width()  * cos(degreeangle)));

	outputImage.setSize(newwidth, newheight);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			float effectivex = (leftpeek < 0) ? x + leftpeek : x;
			float effectivey = (toppeek < 0) ? y + toppeek : y;

			float sourcex = effectivex * cos(degreeangle) - effectivey * sin(degreeangle);
			float sourcey = effectivex * sin(degreeangle) + effectivey * cos(degreeangle);
			outputImage(x, y) = BilinearSample(sourcex, sourcey);
		}
	}

	return 1;
}
	
int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
	float degreeangle = angle*0.0174532925;

	float leftpeek = (*this).height() * sin(degreeangle);
	float toppeek = (*this).width()  * sin(-degreeangle);

	int newheight = ceil(fabs(toppeek) + fabs((*this).height() * cos(degreeangle)));
	int newwidth = ceil(fabs(leftpeek) + fabs((*this).width()  * cos(degreeangle)));

	outputImage.setSize(newwidth, newheight);

	for (int y = 0; y < outputImage.height(); y++)
	{
		for (int x = 0; x < outputImage.width(); x++)
		{
			float effectivex = (leftpeek < 0) ? x + leftpeek : x;
			float effectivey = (toppeek < 0) ? y + toppeek : y;

			float sourcex = effectivex * cos(degreeangle) - effectivey * sin(degreeangle);
			float sourcey = effectivex * sin(degreeangle) + effectivey * cos(degreeangle);
			outputImage(x, y) = GaussianSample(sourcex, sourcey, .25, 1);
		}
	}

	return 1;
}


int Image32::SetAlpha(const Image32& matte)
{
	return 0;
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
	return 0;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

int Image32::FunFilter(Image32& outputImage) const
{
	return 0;
}
int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
	//I've interpretted this such that the output image includes input pixels in the range [x1, x2) x [y1, y2)
	outputImage.setSize(x2-x1, y2-y1);

	for (int x = x1; x < x2; x++)
	{
		for (int y = y1; y < y2; y++)
		{
			outputImage(x - x1, y - y1).r = (*this)(x,y).r;
			outputImage(x - x1, y - y1).g = (*this)(x, y).g;
			outputImage(x - x1, y - y1).b = (*this)(x, y).b;
		}
	}

	return 1;
}


Pixel32 Image32::NearestSample(const float& x, const float& y) const
{
	int ix = (int)(x + .5);
	int iy = (int)(y + .5);

	if (0 <= ix && ix < (*this).width() && 0 <= iy && iy < (*this).height()) {
		return (*this)(ix, iy);
	}
	
	return Pixel32();
}
Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
	int x1 = (int)x;
	int y1 = (int)y;
	Pixel32 returnpixel = Pixel32();

	//this chunk makes us use source values when within the source image, black ones otherwise
	int sourcepointsr [4] = { 0, 0, 0, 0 };
	int sourcepointsg [4] = { 0, 0, 0, 0 };
	int sourcepointsb [4] = { 0, 0, 0, 0 };
	for (int sourcex = x1; sourcex <= x1 + 1; sourcex++) {
		for (int sourcey = y1; sourcey <= y1 + 1; sourcey++) {
			if (0 <= sourcex && sourcex < (*this).width() && 0 <= sourcey && sourcey < (*this).height()) {
				sourcepointsr[sourcex - x1 + 2 * (sourcey - y1)] = (*this)(sourcex, sourcey).r;
				sourcepointsg[sourcex - x1 + 2 * (sourcey - y1)] = (*this)(sourcex, sourcey).g;
				sourcepointsb[sourcex - x1 + 2 * (sourcey - y1)] = (*this)(sourcex, sourcey).b;
			}
		}
	}

	float dx = x - x1;
	float dy = y - y1;

	float ar = sourcepointsr[0] * (1 - dx) + sourcepointsr[1] * dx;
	float br = sourcepointsr[2] * (1 - dx) + sourcepointsr[3] * dx;
	returnpixel.r = ar * (1 - dy) + br * dy;

	float ag = sourcepointsg[0] * (1 - dx) + sourcepointsg[1] * dx;
	float bg = sourcepointsg[2] * (1 - dx) + sourcepointsg[3] * dx;
	returnpixel.g = ag * (1 - dy) + bg * dy;

	float ab = sourcepointsb[0] * (1 - dx) + sourcepointsb[1] * dx;
	float bb = sourcepointsb[2] * (1 - dx) + sourcepointsb[3] * dx;
	returnpixel.b = ab * (1 - dy) + bb * dy;
	
	return returnpixel;
}
Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	float totalweight = 0;
	Pixel32 returnpixel = Pixel32();
	float newr = 0;
	float newg = 0;
	float newb = 0;
	for (int px = ceil(x - radius); px <= floor(x + radius); px++) {
		for (int py = ceil(y - radius); py <= floor(y + radius); py++) {
			float rsquared = pow(px - x, 2) + pow(py - y, 2);
			if (rsquared <= pow(radius, 2)) {
				float weight = exp(-rsquared / (2 * variance));
				totalweight += weight;
				if (0 <= px && px < (*this).width() && 0 <= py && py < (*this).height()) {
					newr += (*this)(px, py).r * weight;
					newg += (*this)(px, py).g * weight;
					newb += (*this)(px, py).b * weight;
				}
			}
		}
	}

	newr /= totalweight;
	if (newr < 0) { newr = 0; }
	else if (newr > 255) { newr = 255; }

	newg /= totalweight;
	if (newg < 0) { newg = 0; }
	else if (newg > 255) { newg = 255; }

	newb /= totalweight;
	if (newb < 0) { newb = 0; }
	else if (newb > 255) { newb = 255; }

	returnpixel.r = newr;
	returnpixel.g = newg;
	returnpixel.b = newb;

	return returnpixel;
}
