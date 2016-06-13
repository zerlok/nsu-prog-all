#ifndef __HAAR_TRANSFORMATION_H__
#define __HAAR_TRANSFORMATION_H__


#include <vector>
typedef std::vector<float> Floats;

#include "transformation.h"


class HaarTransformation : public Transformation<Floats>
{
	public:
		HaarTransformation();

		void forward(Floats& vec) const override;
		void backward(Floats& vec) const override;

	private:
		float _half_sum(const float& a, const float& b) const;
		float _half_diff(const float& a, const float& b) const;
};


// __HAAR_TRANSFORMATION_H__
#endif
