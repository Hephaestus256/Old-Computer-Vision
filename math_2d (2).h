#ifndef INC_MATH_2D
#define INC_MATH_2D

#define VECT_2D_X 0
#define VECT_2D_Y 1
#define PLANE_XY 0
#define PLANE_YX 1

#define VECT_2D_FLOAT_UNDEF_VALUE vect_2d<float>(FLOAT_UNDEF_VALUE, FLOAT_UNDEF_VALUE)
#define VECT_2D_DOUBLE_UNDEF_VALUE vect_2d<double>(DOUBLE_UNDEF_VALUE, DOUBLE_UNDEF_VALUE)
#define VECT_2D_INT_UNDEF_VALUE quantum_2d(INT_UNDEF_VALUE, INT_UNDEF_VALUE)
#define VECT_2D_FLOAT_ORIG vect_2d<float>(0.0f, 0.0f)
#define VECT_2D_DOUBLE_ORIG vect_2d<double>(0.0, 0.0)
#define VECT_2D_INT_ORIG vect_2d<int>(0, 0)

#include <math.h>
#include "C:\Users\Owner\Documents\Visual Studio 2010\Library\math_1d.h"

typedef int quantum, quantum_1d;

template <class gen1>
class vect_2d {
public:
	union {
		struct {
			gen1 x, y;
		};
		gen1 component[2];
	};

	inline vect_2d()
	{
	}

	template <class gen2>
	inline vect_2d(vect_2d<gen2> v)
	{
		x = (gen1)(v.x);
		y = (gen1)(v.y);
	}

	template <class gen2, class gen3>
	inline vect_2d(gen2 p_x, gen3 p_y)
	{
		x = (gen1)p_x;
		y = (gen1)p_y;
	}
	
	inline gen1 get_component(int p_component = VECT_2D_X)
	{
		return component[p_component];
	}

	inline gen1 get_other_component(int p_component = VECT_2D_X)
	{
		return component[1 - p_component];
	}

	inline vect_2d translate_plane(int plane = PLANE_YX)
	{
		return vect_2d(get_component(plane), get_other_component(plane));
	}

	inline bool is_undef()
	{
		return is_undef_value(x) && is_undef_value(y);
	}

	inline vect_2d operator + (vect_2d v)
	{
		return vect_2d(v.x + x, v.y + y);
	}

	inline vect_2d operator - (vect_2d v)
	{
		return vect_2d(x - v.x, y - v.y);
	}

	//template <class gen2>
	inline vect_2d<gen1> operator * (double n)
	{
		return vect_2d<gen1>(gen1(x * n), gen1(y * n));
	}

	//template <class gen2>
	inline vect_2d<gen1> operator / (double n)
	{
		return vect_2d<gen1>(gen1(x / n), gen1(y / n));
	}

	inline vect_2d operator += (vect_2d v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	inline vect_2d operator -= (vect_2d v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline bool operator == (vect_2d v)
	{
		return x == v.x && y == v.y;
	}

	inline bool operator != (vect_2d v)
	{
		return x != v.x || y != v.y;
	}

	inline vect_2d center()
	{
		return *this;
	}

	inline vect_2d round()
	{
		retun vect_2d<float>((fload)floor(x + 0.5), (float)floor(y + 0.5));
	}

	inline float angle(vect_2d v)
	{
		vect_2d<float> diff = vect_2d<float>(v - *this);//v.center() - center();
		return (float)atan2((double)diff.y, (double)diff.x);
	}

	inline bool accute_horz()
	{
		return (m > 1.0) || (m < -1.0);
	}

	inline float dist(vect_2d v)
	{
		vect_2d<gen1> diff = v - *this;
		return float(sqrt(double(diff.x * diff.x + diff.y * diff.y)));
	}
};

template <class gen1>
bool is_undef_value(vect_2d<gen1> v)
{
	return is_undef_value(v.x) && is_undef_value(v.y);
}

vect_2d<float> undef_value(float)
{
	return VECT_2D_FLOAT_UNDEF_VALUE;
}

vect_2d<double> undef_value(double)
{
	return VECT_2D_DOUBLE_UNDEF_VALUE;
}

/*
quantum_2d undef_value(int)
{
	return VECT_2D_INT_UNDEF_VALUE;
}
*/

vect_2d<float> vect_2d_orig(float)
{
	return VECT_2D_FLOAT_ORIG;
}

vect_2d<double> vect_2d_orig(double)
{
	return VECT_2D_DOUBLE_ORIG;
}

vect_2d<int> vect_2d_orig(int)
{
	return VECT_2D_INT_ORIG;
}

class quantum_2d : public vect_2d<int> {
public:
	quantum_2d() : vect_2d ()
	{
	}

	quantum_2d(int x, int y) : vect_2d (x, y)
	{
	}

	vect_2d<float> center()
	{
		return vect_2d<float>(float(x) + 0.5f, float(y) + 0.5f);
	}
};

typedef vect_2d<float> point_2d;
typedef vect_2d<double> point_2dd;

#define QUANTUM_2D_UNDEF_VALUE (quantum_2d)VECT_2D_INT_UNDEF_VALUE
#define POINT_2D_UNDEF_VALUE VECT_2D_FLOAT_UNDEF_VALUE
#define POINT_2DD_UNDEF_VALUE VECT_2D_DOUBLE_UNDEF_VALUE

template <int gen_coord_ind = COORD_X, class coord_type = float, class gen_m = double, class gen_b = double>
class line_2d {
	inline gen_b calc_b (gen_m p_m, vect_2d<coord_type> p_p)
	{
		return gen_b(get_dep(p_p)) - gen_b(p_m * get_ind(p_p));
	}

	template <class gen1, class gen2, class gen3>
	inline void calc(gen1 p_d_ind, gen2 p_d_dep, vect_2d<gen3> p_p)
	{
		if (approx_zero(p_d_ind)) {
			m = DINF;
			b = get_ind(p_p);
		} else {
			m = gen_m(p_d_dep / p_d_ind);
			b = calc_b(m, p_p);
		}
	}

public:

	gen_m m;
	gen_b b;

	inline line_2d ()
	{
	}

	inline line_2d (gen_m p_m, gen_b p_b)
	{
		m = p_m;
		b = p_b;
	}

	template <class gen1>
	inline line_2d (double p_d_ind, double p_d_dep, vect_2d<gen1> p_p)
	{
		calc(p_d_ind, p_d_dep, p_p);
	}

	inline bool is_asmtote ()
	{
		return m == DINF;
	}

	inline coord_type get_ind (vect_2d<coord_type> c)
	{
		return c.get_component(gen_coord_ind);
	}

	inline coord_type get_dep (vect_2d<coord_type> c)
	{
		return c.get_other_component(gen_coord_ind);
	}

	template <class gen1>
	inline coord_type func (gen1 p_ind)
	{
		return coord_type(m * p_ind + b);
	}
};

typedef line_2d<VECT_2D_X, float, double, double> line_2d_dydx;
typedef line_2d<VECT_2D_Y, float, double, double> line_2d_dxdy;

/*
class line_2d_dydx : public line_2d {
	template <class gen1>
	inline float calc_b (double p_m, vect_2d<gen1> p_p)
	{
		return float(double(p_p.y) - p_m * double(p_p.x));
	}

	template <class gen1>
	inline void calc(double p_d_dep, double p_d_ind, vect_2d<gen1> p_p)
	{
		if (approx_equal(p_d_ind, 0.0)) {
			m = DINF;
			b = p_p.x;
		} else {
			m = p_d_dep / p_d_ind;
			b = calc_b(m, p_p);
		}
	}
public:
	inline line_2d_dydx () : line_2d ()
	{
	}

	inline line_2d_dydx (double p_m, double p_b) : line_2d (p_m, p_b)
	{
	}

	template <class gen1>
	inline line_2d_dydx (double p_dy, double p_dx, vect_2d<gen1> p_p)
	{
		calc(p_dy, p_dx, p_p);
	}

	template <class gen1>
	inline line_2d_dydx (double p_m, vect_2d<gen1> p_p)
	{
		m = p_m;
		b = calc_b (p_m, p_p);
	}

	template <class gen1>
	inline float func (gen1 p_x)
	{
		return float(m * p_x + b);
	}
};

class line_2d_dxdy : public line_2d {
	template <class gen1>
	inline void calc(double p_dx, double p_dy, vect_2d<gen1> p_p)
	{
		if (approx_equal(p_dy, 0.0)) {
			m = DINF;
			b = p_p.y;
		} else {
			m = p_dx / p_dy;
			b = calc_b(m, p_p);
		}
	}
public:
	inline line_2d_dxdy () : line_2d ()
	{
	}

	inline line_2d_dxdy (double p_m, double p_b) : line_2d (p_m, p_b)
	{
	}

	template <class gen1>
	inline line_2d_dxdy (double p_dx, double p_dy, vect_2d<gen1> p_p)
	{
		calc(p_dx, p_dy, p_p);
	}

	template <class gen1>
	inline line_2d_dxdy (double p_m, vect_2d<gen1> p_p)
	{
		m = p_m;
		b = calc_b (p_m, p_p);
	}

	inline line_2d_dxdy inverse()
	{

	}

	inline bool is_horizontal()
	{
		return is_asmtote();
	}

	template <class gen1>
	inline line_2d_dxdy (vect_2d<gen1> p_p1, vect_2d<gen1> p_p2)
	{
		calc(p_p2.x - p_p1.x, p_p2.y - p_p1.y, p_p1);
	}

	template <class gen1>
	inline float calc_b (double p_m, vect_2d<gen1> p_p)
	{
		return float(double(p_p.x) - p_m * double(p_p.y));
	}

	inline line_2d_dxdy perpendicular ()
	{
		return line_2d_dxdy (-1.0 / m, vect_2d<double>(0.0, b));
	}

	template <class gen1>
	inline float func (gen1 p_y)
	{
		return float(m * p_y + b);
	}

	inline vect_2d<float> point_on (float p_y)
	{
		return vect_2d<float>(func(p_y), p_y);
	}

	inline vect_2d<float> intersect (line_2d_dxdy l)
	{
		if (l.is_horizontal()) {
			if (is_horizontal()) {
				return VECT_2D_FLOAT_UNDEF_VALUE;
			} else {
				return point_on((float)l.b);
			}
		} else {
			if (is_horizontal()) {
				return l.point_on((float)b);
			} else {
				// ind = (b2 - b1) / (m1 - m2)
				if (approx_equal ((float)m, (float)l.m)) {
					return VECT_2D_FLOAT_UNDEF_VALUE;
				} else {
					return point_on((float)((m - l.m) / (l.b - b)));
				}
			}
		}
	}

	// returns the distance from the point to the closest point on the line to the point
	template <class gen1>
	inline float dist (vect_2d<gen1> p_p)
	{
		if (is_horizontal()) {
			return fabs(b - p_p.y);
		} else {
			line_2d_dxdy perp = perpendicular();

		}
	}

	template <class gen1>
	inline bool operator < (vect_2d<gen1> p_p)
	{
		return p_p. x < func (p_p.y);
	}

	template <class gen1>
	inline bool operator > (vect_2d<gen1> p_p)
	{
		return p_p. x > func (p_p.y);
	}
};
*/

class ray_2d {
public:
	double angle;
	vect_2d<float> orig;
	double cot; // cotangent of angle, pre-calculated for speed

	inline ray_2d()
	{
	}

	inline ray_2d(float p_angle, vect_2d<float> p_orig)
	{
		angle = p_angle;
		orig = p_orig;
	}

	inline ray_2d(vect_2d<float> p_p1, vect_2d<float> p_p2)
	{
		angle = atan2(p_p2.y - p_p1.y, p_p2.x - p_p1.x);
		cot = 1.0 / tan(angle);
		orig = p_p1;
	}

	// is obtuse angle to horizontal
	inline bool obtuse_horz()
	{
		return is_between(angle, PI_4, PI_4 + PI_2) || is_between(angle, -PI_4, -PI_4 - PI_2);
	}

	// is accute angle to horizontal
	inline bool accute_horz()
	{
		return !obtuse_horz();
	}

	inline float delta_x(float p_y)
	{
		return float(double(p_y - orig.y) * cot);
	}

	inline float delta_y(float p_x)
	{
		return float(double(p_x - orig.x) * tan(angle));
	}

	// function of y, returns x
	inline float func_y(float p_y)
	{
		return orig.x + delta_x(p_y);
	}

	// function of x, returns y
	inline float func_x(float p_x)
	{
		return orig.y + delta_y(p_x);
	}
	
	inline float dist_x(vect_2d<float> p_p)
	{
		return p_p.x - func_y(p_p.y);
	}

	inline float dist_y(vect_2d<float> p_p)
	{
		return p_p.y - func_x(p_p.x);
	}
};

#endif