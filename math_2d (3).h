#ifndef INC_MATH_2D
#define INC_MATH_2D

#define VECT_2D_X 0
#define VECT_2D_Y 1
#define PLANE_XY 0 // independent, dependent - value matches independent
#define PLANE_YX 1 //   "

#define VECT_2D_FLOAT_UNDEF_VALUE vect_2d<float>(FLOAT_UNDEF_VALUE, FLOAT_UNDEF_VALUE)
#define VECT_2D_DOUBLE_UNDEF_VALUE vect_2d<double>(DOUBLE_UNDEF_VALUE, DOUBLE_UNDEF_VALUE)
#define VECT_2D_INT_UNDEF_VALUE quantum_2d(INT_UNDEF_VALUE, INT_UNDEF_VALUE)
#define VECT_2D_FLOAT_ORIG vect_2d<float>(0.0f, 0.0f)
#define VECT_2D_DOUBLE_ORIG vect_2d<double>(0.0, 0.0)
#define VECT_2D_INT_ORIG vect_2d<int>(0, 0)
#define POINT_2D_ORIG VECT_2D_FLOAT_ORIG
#define POINT_2DD_ORIG VECT_2D_DOUBLE_ORIG

#include <math.h>
#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\math_1d.h"

typedef int quantum, quantum_1d;

template <class gen1>
class vect_2d {
	static char s_report_text[40];

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

	template <class gen2, class gen3>
	inline vect_2d(int ind_axis, gen2 p_ind, gen3 p_dep)
	{
		component[ind_axis] = (gen1)p_ind;
		component[1 - ind_axis] = (gen1)p_dep;
	}

	inline vect_2d(int ind)
	{
		x = (gen1)ind;
		y = (gen1)(0);
	}

	inline vect_2d(float ind)
	{
		x = (gen1)ind;
		y = (gen1)(0);
	}

	inline vect_2d(double ind)
	{
		x = (gen1)ind;
		y = (gen1)(0);
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

	inline vect_2d<int> alias_pixel()
	{
		return vect_2d<int>((int)floor(x), -(int)floor(y));
	}

	inline vect_2d<gen1> cv_coord()
	{
		return vect_2d<gen1>(x, -y);
	}

	inline vect_2d<gen1> center(vect_2d<gen1> p_p)
	{
		return (*this + p_p) * 0.5;
	}

	// alpha = 0.0: 100% this, 0% that
	// alpha = 1.0: 0% this, 100% that
	inline vect_2d<gen1> interpolate(vect_2d<gen1> p_p, double alpha)
	{
		return ((*this) * (1.0 - alpha)) + (p_p * alpha);
	}

	inline vect_2d round()
	{
		retun vect_2d<float>((float)floor(x + 0.5), (float)floor(y + 0.5));
	}

	inline vect_2d alias()
	{
		return vect_2d<int>((int)floor(x), (int)floor(y));
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

	inline double dist(vect_2d v)
	{
		vect_2d<double> diff = v - *this;
		return sqrt(diff.x * diff.x + diff.y * diff.y);
	}

	inline double dist_sqrd(vect_2d v)
	{
		vect_2d<double> diff = v - *this;
		return diff.x * diff.x + diff.y * diff.y;
	}

	inline char* report()
	{
		sprintf(s_report_text, "(%f, %f)", float(x), float(y));
		return s_report_text;
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

	// screen coords are quantum_2d
	// screen coords are different from cartesian coords - mathematically, pixels exist in the 4th quadrant (x, -y)
	vect_2d<float> to_cart_coord()
	{
		return vect_2d<float>(float(x) + 0.5f, float(-y) - 0.5f);
	}

	vect_2d<float> mean(quantum_2d q)
	{
		return vect_2d<float>(float(q.x + x) * 0.5, float(q.y + y) * 0.5);
	}
};

typedef vect_2d<float> point_2d;
typedef vect_2d<double> point_2dd;

#define QUANTUM_2D_UNDEF_VALUE (quantum_2d)VECT_2D_INT_UNDEF_VALUE
#define POINT_2D_UNDEF_VALUE VECT_2D_FLOAT_UNDEF_VALUE
#define POINT_2DD_UNDEF_VALUE VECT_2D_DOUBLE_UNDEF_VALUE

template <class ang_type = double>
class angle_2d {
public:
	ang_type angle;

	inline angle_2d()
	{
	}

	inline angle_2d(ang_type p_a)
	{
		angle = p_a;
	}

	inline angle_2d(vect_2d<ang_type> p_p1, vect_2d<ang_type> p_p2, double p_offset = 0.0)
	{
		angle = atan2(p_p2.y - p_p1.y, p_p2.x - p_p1.x) + p_offset;
	}

	inline angle_2d(vect_2d<ang_type> p_p1, vect_2d<ang_type> p_p2, vect_2d<ang_type> p_p3)
	{
		*this = (angle_2d(p_p2, p_p1) - angle_2d(p_p2, p_p3)).correct_360();
	}

	inline angle_2d perpendicular()
	{
		return angle + PI * 0.5;
	}

	inline angle_2d complement()
	{
		return PI * 0.5 - angle;
	}

	inline angle_2d supplement()
	{
		return PI - angle;
	}

	inline angle_2d correct_360()
	{
		if (angle < 0.0) {
			return PI * 2 + angle;
		} else {
			return angle;
		}
	}

	inline ang_type to_degrees()
	{
		return angle * (180 / PI);
	}

	inline angle_2d operator - (angle_2d p_a)
	{
		return angle - p_a.angle;
	}

	inline angle_2d operator + (angle_2d p_a)
	{
		return angle + p_a.angle;
	}

	inline angle_2d operator += (angle_2d p_a)
	{
		return angle += p_a.angle;
	}

	template <class par_type>
	inline angle_2d operator += (par_type p)
	{
		return angle += (ang_type)p;
	}

	inline angle_2d operator -= (angle_2d p_a)
	{
		return angle -= p_a.angle;
	}

	template <class par_type>
	inline angle_2d operator -= (par_type p)
	{
		return angle -= (ang_type)p;
	}

	template <class par_type>
	inline angle_2d operator / (par_type p)
	{
		return angle / (ang_type)p;
	}

	inline angle_2d pxl_to_coord()
	{
		return -angle;
	}

	template <class par_type>
	inline angle_2d cv_bot(par_type top)
	{
		if (angle > ang_type(top)) {
			angle = angle - PI * 2.0;
		}
	}

	template <class par_type>
	inline angle_2d cv_top(par_type bot)
	{
		if (angle < ang_type(bot)) {
			angle = angle + PI * 2.0;
		}
	}

	int xtant()
	{
		if (angle >= 0.0) {
			if (angle <= PI * 0.25) {
				return 1;
			} else if (angle <= PI * 0.75) {
				return 2;
			} else if (angle <= PI * 1.25) {
				return 3;
			} else if (angle < PI * 1.75) {
				return 4;
			} else {
				return 1;
			}
		} else {
			if (angle >= PI * -0.25) {
				return 1;
			} else if (angle >= PI * -0.75) {
				return 2;
			} else if (angle >= PI * -1.25) {
				return 3;
			} else if (angle >= PI * -1.75) {
				return 4;
			} else {
				return 1;
			}
		}
	}
};

template <int gen_coord_ind = PLANE_XY, class coord_type = float, class gen_m = double, class gen_b = double>
class line_2d {
	
	char s_report_text[60];

	inline gen_m asmtote_value()
	{
		return DINF;
	}

	inline gen_b calc_b (gen_m p_m, vect_2d<coord_type> p_p)
	{
		return gen_b(get_dep(p_p)) - gen_b(p_m * get_ind(p_p));
	}

	template <class gen1, class gen2, class gen3>
	inline void calc(gen1 p_d_ind, gen2 p_d_dep, vect_2d<gen3> p_p)
	{
		if (approx_zero(p_d_ind)) {
			m = asmtote_value();
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

	template <class gen1>
	inline line_2d (vect_2d<coord_type> p_p1, vect_2d<coord_type> p_p2, vect_2d<gen1> p_p)
	{
		vect_2d<gen1> delta = p_p2 - p_p1;
		calc(get_ind(delta), get_dep(delta), p_p);
	}

	template <class gen1>
	inline line_2d (vect_2d<gen1> p_p1, vect_2d<gen1> p_p2)
	{
		vect_2d<gen1> delta = p_p2 - p_p1;
		calc(get_ind(delta), get_dep(delta), p_p1);
	}

	/* perpendicular line constructor */
	template <class gen1>
	inline line_2d (vect_2d<gen1> p_p1, vect_2d<gen1> p_p2, bool b)
	{
		vect_2d<gen1> delta = p_p2 - p_p1;

		if (b) {
			calc(get_dep(delta), -get_ind(delta), p_p1.center(p_p2));
		} else {
			calc(get_dep(delta), -get_ind(delta), p_p1);
		}
	}

	inline bool is_asmtote ()
	{
		return m == asmtote_value();
	}

	inline bool is_undef()
	{
		return m == -FINF;
	}

	inline coord_type get_ind (vect_2d<coord_type> c)
	{
		return c.get_component(gen_coord_ind);
	}

	inline coord_type get_dep (vect_2d<coord_type> c)
	{
		return c.get_other_component(gen_coord_ind);
	}

	inline void set_ind (vect_2d<coord_type>& c, coord_type value)
	{
		c.component[gen_coord_ind] = value;
	}

	inline void set_dep (vect_2d<coord_type>& c, coord_type value)
	{
		c.component[1 - gen_coord_ind] = value;
	}

	template <class gen1>
	inline coord_type func (gen1 p_ind)
	{
		return coord_type(m * p_ind + b);
	}

	template <class gen1>
	inline vect_2d<coord_type> func_point (gen1 p_ind)
	{
		vect_2d<coord_type> ret;

		set_ind(ret, p_ind);
		set_dep(ret, func(p_ind));

		return ret;
	}

	/* returns the distance between a point and its closest point on the line */
	template <class gen1>
	inline double distance (gen1 p_p)
	{
		if (is_asmtote()) {
			return coord_type(get_ind(p_p) - b);
		} else {
			double d_dep = get_dep(p_p) - func (get_ind(p_p));
			return d_dep * cos(atan(m));
		}
	}

	inline double distance (coord_type p_p)
	{
		return p_p.dist (closest_point(p_p));
	}

	template <class coord>
	inline line_2d perp_line (coord p_p)
	{
		if (approx_zero(m)) {
			return line_2d(asmtote_value(), get_ind(p_p));
		} else if (is_asmtote()) {
			return line_2d(0.0, get_dep(p_p));
		} else {
			double perp_m = -1.0 / m;
			return line_2d(perp_m, calc_b(perp_m, p_p));
		}
	}

	inline point_2d intersect (line_2d p_l)
	{
		point_2d ret;

		if (approx_equal(m, p_l.m)) {
			ret = VECT_2D_FLOAT_UNDEF_VALUE;
		} else {
			if (is_asmtote()) {
				ret = point_2d(b, p_l.func(b));
			} else if (p_l.is_asmtote()) {
				ret = point_2d(p_l.b, func(p_l.b));
			} else {
				double ind = (p_l.b - b) / (m - p_l.m);
				ret = point_2d(gen_coord_ind, ind, func(ind));
			}
		}

		return ret;
	}

	angle_2d<double> angles()
	{
		return angle_2d<double>(atan(m));
	}

	template <class point_type>
	int relative(vect_2d<point_type> p)
	{
		if (is_asmtote()) {
			return sign(p.x, b);
		} else {
			return sign(p.y, func(p.x));
		}
	}

	inline char* report()
	{
		sprintf(s_report_text, "m=%f, b=%f", float(m), float(b));
		return s_report_text;
	}
};

template <class vert_type = point_2d>
class segment_2d {
public:
	vert_type a, b;

	segment_2d ()
	{
	}

	segment_2d (vert_type p)
	{
		a = vert_type(0);
		b = p;
	}

	segment_2d (vert_type p1, vert_type p2)
	{
		a = p1;
		b = p2;
	}

	double magnitude()
	{
		return a.dist(b);
	}

	double magnitude_sqrd()
	{
		return a.dist_sqrd(b);
	}
};

template <class vert_type = point_2d, class area_type = double>
class acc_area {
public:
	area_type area;
	segment_2d<vert_type> chord;
	
	inline void init(vert_type first)
	{
		area = (area_type)0;
		chord = segment_2d<>(first, first);
	}

	acc_area ()
	{
	}

	acc_area (vert_type first)
	{
		init(first);
	}

	inline void add_col_x2(vert_type p)
	{
		area += calc_col_area_x2(chord.b, p);
		chord.b = p;
	}

	inline void sub_col_x2(vert_type p)
	{
		area -= calc_col_area_x2(chord.a, p);
		chord.a = p;
	}

	inline area_type calc_tot_area()
	{
		return 0.5 * calc_tot_area_x2();
	}

	inline area_type calc_tot_area_x2()
	{
		return area - calc_col_area_x2(chord.a, chord.b);
	}

	inline area_type calc_col_area_x2(vert_type p1, vert_type p2)
	{
		return (p2.y + p1.y) * (p2.x - p1.x);
	}

	inline area_type calc_col_area (vert_type p)
	{
		return calc_area_x2(p) * 0.5;
	}

	inline area_type calc_ave_height_x2()
	{
		return calc_tot_area_x2() / chord.magnitude();
	}

	inline area_type calc_ave_height()
	{
		if (chord.magnitude() == 0.0) {
			return 0.0;
		} else {
			return 0.5 * (calc_tot_area_x2() / chord.magnitude());
		}
	}

	inline int ave_height_threshhold(float th)
	{
		area_type a = calc_tot_area_x2();
		
		if (a > (area_type)0) {
			if (a * a > 4 * th * th * chord.magnitude_sqrd()) {
				return 1;
			} else {
				return 0;
			}
		} else {
			if (a * a > 4 * th * th * chord.magnitude_sqrd()) {
				return -1;
			} else {
				return 0;
			}
		}
	}

	inline area_type calc_curve_x2()
	{
		return calc_tot_area_x2() / chord.magnitude_sqrd();
	}

	inline area_type calc_curve()
	{
		return 0.5 * calc_tot_area_x2() / chord.magnitude_sqrd();
	}

	inline double calc_dist()
	{
		return chord.magnitude();
	}
};

template <class center_type = point_2d, class radius_type = double>
class circle_2d {
public:
	center_type center;
	radius_type radius;

	circle_2d ()
	{
	}

	circle_2d (center_type p_center, radius_type p_radius)
	{
		center = p_center;
		radius = p_radius;
	}

	template <class vert_type>
	circle_2d (vert_type p1, vert_type p2, vert_type p3)
	{
		line_2d<> n12(p1, p2, true);
		line_2d<> n23(p2, p3, true);
		
		center = n12.intersect(n23);
		radius = p1.dist(center);
	}

	template <class vert_type>
	circle_2d (vert_type p1, vert_type p2, vert_type p3, vert_type p4)
	{
		line_2d<> n12(p1, p2, true);
		line_2d<> n34(p3, p4, true);

		center = n12.intersect(n34);
		radius = p1.dist(center);
	}

	template <class vert_type>
	radius_type dist(vert_type p)
	{
		return radius_type(center.dist(p)) - radius;
	}

	template <class vert_type>
	radius_type dist_center(vert_type p)
	{
		return radius_type(center.dist(p));
	}
};

typedef line_2d<PLANE_XY, float, double, double> line_2d_dydx;
typedef line_2d<PLANE_YX, float, double, double> line_2d_dxdy;
#define LINE_2D_DYDX_UNDEF line_2d_dydx(-FINF, -FINF);

template <class gen>
class line_2d_gen {
	
	line_2d_dydx dydx;
	line_2d_dxdy dxdy; 

public:

	gen test_gen[5];
	point_2d test_p2d[5];

	line_2d_gen ()
	{
	}

	line_2d_gen (point_2d start, point_2d finish, point_2d ref)
	{
		point_2d delta = finish - start;

		test_p2d[0] = start;
		test_p2d[1] = finish;
		test_p2d[2] = ref;

		if (fabs(delta.x) > fabs(delta.y)) {
			dxdy.m = 10.0;
			dydx = line_2d_dydx(delta.x, delta.y, ref);
		} else {
			dydx.m = 10.0; /* m should never be > 1 or < -1 */
			dxdy = line_2d_dxdy(delta.y, delta.x, ref);
		}
	}

	int get_ind_axis()
	{
		if (dydx.m >= 2.0) {
			return VECT_2D_Y;
		} else {
			return VECT_2D_X;
		}
	}

	bool is_dydx()
	{
		return dxdy.m >= 2.0;
	}

	double func(point_2d p_p)
	{
		if (is_dydx()) {
			return dydx.func(p_p.x);
		} else {
			return dxdy.func(p_p.y);
		}
	}

	double offset(point_2d p_p)
	{
		if (is_dydx()) {
			return p_p.y - dydx.func(p_p.x);
		} else {
			return p_p.x - dxdy.func(p_p.y);
		}
	}

	line_2d_dxdy get_dxdy()
	{
		return dxdy;
	}

	line_2d_dydx get_dydx()
	{
		return dydx;
	}

	inline char* report()
	{
		sprintf(s_report_text, "m=%f, b=%f)", float(m), float(b));
		return s_report_text;
	}
};

/*
template <class gen1, class gen2>
line_2d_dydx perp_line_dydx (vect_2d<gen1> p1, vect_2d<gen2> p2)
{
}
*/

/* variable axis line */
/*
template <class gen1>
class axis_line_2d : public line_2d<0, gen1, double, double> {
	axis _line_2d (vect_2d<gen1> p1, vect_2d<gen1> p2) {
		vect_2d<gen1> diff = p2 - p1;
		if (fabs(diff.x) > fabs(diff.y)) {
			// more horz than vert
			calc(diff.x, diff.y, p1);
		} else {
			// more vert than horz
			calc(diff.y, diff.x, p1);
		}
	}

};
*/

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
			m = asmtote_value();
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
			m = asmtote_value();
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

	inline ray_2d(vect_2d<float> p_p1, vect_2d<float> p_p2, double p_offset = 0.0)
	{
		angle = atan2(p_p2.y - p_p1.y, p_p2.x - p_p1.x) + p_offset;
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

	inline vect_2d<float> extend(float p_r)
	{
		return orig + vect_2d<float>(cos(angle), sin(angle)) * p_r;
	}
};

template <class vert_type, int sides>
class poly_2d {
public:
	vert_type vert[sides];
};

template <class vert_type = point_2d>
class tri_2d : poly_2d<vert_type, 3> {
	/* vert_type vert[3]; */
public:
	tri_2d (vert_type v1, vert_type v2, vert_type v3)
	{
		vert[0] = v1;
		vert[1] = v2;
		vert[2] = v3;
	}

	double area_x2()
	{
		return 
			(vert[0].x * vert[1].y - vert[0].y * vert[1].x) +
			(vert[1].x * vert[2].y - vert[1].y * vert[2].x) +
			(vert[2].x * vert[0].y - vert[2].y * vert[0].x) 
			;
	}

	double area_x2b()
	{
		return 
			(vert[1].x - vert[0].x) * (vert[1].y + vert[0].y) +
			(vert[2].x - vert[1].x) * (vert[2].y + vert[1].y) +
			(vert[0].x - vert[3].x) * (vert[0].y + vert[3].y)
			;
	}

	double area()
	{
		return area_x2() * 0.5;
	}
};

double radius_from_arc(double h, double w)
{
	return 0.5 * h + ((w * w) / (8 * h));
}

#endif