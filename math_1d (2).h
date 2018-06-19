#ifndef INC_MATH_1D
#define INC_MATH_1D

#include <math.h>
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <limits>

float _fzero = (float)0.0f;
double _dzero = (double)0.0;

#define FNAN ((float)0.0f / _fzero)
#define DNAN ((double)0.0 / _dzero)
#define FINF ((float)1.0f / _fzero)
#define DINF ((double)1.0f / _dzero)
#define INT_UNDEF_VALUE INT_MAX
#define FLOAT_UNDEF_VALUE FNAN
#define DOUBLE_UNDEF_VALUE DNAN

#define FLOAT_PRECISION 0.001f
#define DOUBLE_PRECISION 0.00001

#define PI M_PI
#define PI_2 M_PI * 0.5
#define PI_4 M_PI * 0.25

template <class gen1>
inline void swap(gen1 &a, gen1 &b)
{
	gen1 temp = a;
	a = b;
	b = temp;
}

template <class gen1>
inline bool is_nan(gen1 num)
{
	return num != num;
}

template <class gen1>
inline bool is_undef_value(gen1 num)
{
	return is_nan(num);
}

inline bool is_undef_value(int num)
{
	return num == INT_UNDEF_VALUE;
}

inline float precision(float v)
{
	return FLOAT_PRECISION;
}

inline double precision(double v)
{
	return DOUBLE_PRECISION;
}

template <class gen1>
inline bool is_between(gen1 x, gen1 x1, gen1 x2)
{
	return x >= x1 && x <= x2;
}

inline bool approx_equal(int x1, int x2)
{
	return true;
}

inline bool approx_equal(float x1, float x2)
{
	return is_between(x1, x2 - FLOAT_PRECISION, x2 + FLOAT_PRECISION);
}

inline bool approx_equal(double x1, double x2)
{
	return is_between(x1, x2 - DOUBLE_PRECISION, x2 + DOUBLE_PRECISION);
}

inline bool approx_zero (int x)
{
	return true;
}

inline bool approx_zero (float x)
{
	return is_between(x, -FLOAT_PRECISION, FLOAT_PRECISION);
}

inline bool approx_zero (double x)
{
	return is_between(x, -DOUBLE_PRECISION, DOUBLE_PRECISION);
}

template <class gen1>
inline gen1 interpolate (double alpha, gen1 x1, gen1 x2)
{
	return gen1((double)x1 + double(x2 - x1) * alpha);
}

#endif