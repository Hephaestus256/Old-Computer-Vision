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

/* for overloading with templates */
inline int floor(int i)
{
	return i;
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

/* sums a sequence of integers
	 example: n = 5, m = 10; sum = 5 + 6 + 7 + 8 + 9 + 10
	 offset is the fractional part of the sequence
	 example: n = 5, m = 7, offset = 0.3; sum = 5.3 + 6.3 + 7.3 */
template <class gen1>
inline gen1 sum_sequence (gen1 n, gen1 m, float offset = 0.0f)
{
	return (m * (m + (gen1)1) - n * (n - (gen1)1)) / (gen1)2 + (gen1)((m - n + 1) * offset);
}

/* finds the minimum value of values */
template <class min_type = int, class key_type = int>
class min_value {
	
	min_type value;
	key_type key;

public:

	min_value(min_type p_init = 10000, key_type p_key = -1)
	{
		value = p_init;
		key = p_key;
	}

	inline bool update(min_type p_value, key_type p_key)
	{
		if (p_value < value) {
			value = p_value;
			key = p_key;
			return true;
		}

		return false;
	}

	inline min_type report_value()
	{
		return value;
	}

	inline key_type report_key()
	{
		return key;
	}
};

/* finds the maximum value */
template <class max_type = int, class key_type = int>
class max_value {
	
	max_type value;
	key_type key;

public:

	max_value(max_type p_init = -10000, key_type p_key = -1)
	{
		init(p_init, p_key);
	}

	void init(max_type p_init = -10000, key_type p_key = -1)
	{
		value = p_init;
		key = p_key;
	}

	// change value if new max value and return if updated or not
	inline bool update(max_type p_value, key_type p_key)
	{
		if (p_value > value) {
			value = p_value;
			key = p_key;
			return true;
		}

		return false;
	}

	inline max_type report_value()
	{
		return value;
	}

	inline key_type report_key()
	{
		return key;
	}
};

/* finds the average value */
template <class mean_type>
class mean {
public:

	mean_type acc;
	double ct;

	inline mean(mean_type p_init = 0, double p_ct = 0.0)
	{
		ct = p_ct;
		acc = p_init;
	}

	inline void update(mean_type p_next)
	{
		ct += 1.0;
		acc += p_next;
	}

	inline void cull(mean_type p_value)
	{
		ct -= 1.0;
		acc -= p_value;
	}

	inline mean_type calc()
	{
		return acc / (double)ct;
	}

	inline mean_type sum()
	{
		return acc;
	}

	inline mean_type count()
	{
		return ct;
	}
};

/* round-robin iterator class (wrap-around) */
template <class iter_type = int>
class circular_iterator {
	iter_type iter;
	iter_type end;
	int size;
	int wrap_ct;
	int overlap_ct;
	int count;

public:

	circular_iterator (int p_size, iter_type p_begin = 0)
	{
		init (p_size, p_begin);
	}

	circular_iterator (int p_size, iter_type p_begin, iter_type p_end)
	{
		init (p_size, p_begin);

		/* assuming forward iteration, if end is less than begin, it means that it wraps around,
			 so add size to end.  will have to use wrap_iter to get actual iteration.
		     assuming that it only wraps once */
		if (p_begin < p_end) {
			end = p_end;
		} else {
			end = p_end + p_size;
		}
	}

	inline void init (int p_size = -1, iter_type p_begin = 0)
	{
		size = p_size;
		iter = p_begin;
		wrap_ct = overlap_ct = count = 0;
	}

	inline iter_type iterate()
	{
		/*
		if (iter == size) {
			iter = 0;
			wrap_ct++;
		}
		*/

		++iter;
		//overlap_ct += wrap_ct; // not designed for more than one wrap

		return iter;
	}

	inline iter_type iterate_rev()
	{
		if (iter == 0) {
			iter = size;
			wrap_ct--;
		}

		--iter;
		overlap_ct += wrap_ct;

		return iter;
	}

	inline iter_type wrap_iter(iter_type offset = 0)
	{
		return (iter + offset + size) % size;
	}

	inline bool not_wrap()
	{
		return wrap_ct == 0;
	}

	inline bool cont()
	{
		return iter <= end;
	}

	inline int get_count()
	{
		return count;
	}

	inline iter_type get_iter()
	{
		return iter;
	}

	inline iter_type get_size()
	{
		return size;
	}

	inline iter_type get_end()
	{
		return end;
	}

	inline int get_wrap_ct()
	{
		return wrap_ct;
	}

	// not designed for > 1 wrap
	inline int get_overlap_ct()
	{
		return overlap_ct;
	}
};

#endif