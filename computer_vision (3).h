#ifndef INC_COMPUTER_VISION
#define INC_COMPUTER_VISION

#define TEST_DS_IN 0
#define TEST_DS_OUT_GFX_1 1
#define TEST_DS_OUT_GFX_2 2
#define TEST_DS_OUT_TEXT_1 3
#define TEST_DS_OUT_TEXT_2 4

#define TEST_WIN_TEXT_1 0
#define TEST_WIN_TEXT_2 1

#define OVERLAP_CT 3

#include <string.h>

draw_surface* _test_ds[16];
window* _test_win[16];

int _test_int[16] = {0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0};

/*
pool - a homogenous collection of data in an array, vector, list, etc
row - a horizontal row of pixels
segment - a contiguous horizontal line segment of homogenous characteristics
	there are 2 types of segments:
		row_segment - a segment that comes from a parsed horizontal row of pixels
		poly_segment - the type of segment(s) that form(s) a _polygon_
seg set - a set of segments that make up a single horizontal row
	there are 2 types of seg sets:
		row_seg_set -
		poly_seg_set - 
polygon - a collection of _poly_segments_ that can be parsed into a shape
shape - a collection of _features_ that define the shape
feature - the elements of a _shape_ (curve, angle, vertex)
*/

#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\math_1d.h"
#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\math_2d.h"
#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\my_stl.h"
#include <vector>
#include <list>

void show_text(char* s)
{
	_test_win[TEST_WIN_TEXT_1]->put_text(s);
}

void show_text(double n)
{
	_test_win[TEST_WIN_TEXT_1]->put_text(n);
}

void show_text(char* s, double n)
{
	_test_win[TEST_WIN_TEXT_1]->put_text(n, s);
}

void show_text(char* s1, char* s2)
{
	char s[256];
	strcpy(s, s1);
	strcat(s, s2);
	_test_win[TEST_WIN_TEXT_1]->put_text(s);
}

void color_point(int x, int y, rgb c, double alpha = 1.0)
{
	_test_ds[TEST_DS_OUT_GFX_1]->alpha_blend_safe(x, y, alpha, c);
}

void color_point(vect_2d<int> p, rgb c, double alpha = 1.0)
{
	_test_ds[TEST_DS_OUT_GFX_1]->alpha_blend_safe(p.x, p.y, alpha, c);
}

void color_point_in(int x, int y, rgb c, double alpha = 1.0)
{
	_test_ds[TEST_DS_IN]->alpha_blend_safe(x, y, alpha, c);
}

void show_point(vect_2d<int> p, rgb c = RGB_RED, int radius = 3)
{
	for (int x = p.x - radius; x <= p.x + radius; x++) {
		color_point(x, p.y - radius, c);
	}

	for (int y = p.y - radius + 1; y <= p.y + radius -1; y++) {
		color_point(p.x - radius, y, c);
		color_point(p.x + radius, y, c);
	}

	for (int x = p.x - radius; x <= p.x + radius; x++) {
		color_point(x, p.y + radius, c);
	}

//	blit_windows(0);
}

void show_point_in(vect_2d<int> p, rgb c = RGB_RED, int radius = 3)
{
	for (int x = p.x - radius; x <= p.x + radius; x++) {
		color_point_in(x, p.y - radius, c);
	}

	for (int y = p.y - radius + 1; y <= p.y + radius -1; y++) {
		color_point_in(p.x - radius, y, c);
		color_point_in(p.x + radius, y, c);
	}

	for (int x = p.x - radius; x <= p.x + radius; x++) {
		color_point_in(x, p.y + radius, c);
	}

//	blit_windows(0);
}

rgb read_point_in(int x, int y)
{
	return _test_ds[TEST_DS_IN]->read_pxl_safe(x, y);
}

void show_line(line_2d_dydx l, rgb color = RGB_RED)
{
	static quantum_2d d = _test_ds[TEST_DS_OUT_GFX_1]->get_dims();
	double m;
	double dep;
	int ind_comp;

	if (abs(l.m) < 1.0) {
		ind_comp = VECT_2D_X;
		m = l.m;
		dep = l.b;
	} else {
		ind_comp = VECT_2D_Y;
		m = 1.0 / l.m;
		dep = l.b * m;
	}

	//show_text(l.m);

	for (int ind = 0; ind < d.get_component(ind_comp); ind++) {
		vect_2d<int> p = point_2d(double(ind), dep).alias_pixel();
		color_point(p.get_component(ind_comp), p.get_other_component(ind_comp), color);
		dep += m;
	}
}

class pixel_info {
public:
	int r, g, b;

	pixel_info()
	{
	}

	pixel_info(int p_r, int p_g, int p_b)
	{
		r = p_r;
		g = p_g;
		b = p_b;
	}

	int extent()
	{
		return max(max(abs(g), abs(r)), abs(b));
	}

	rgb report()
	{
		int prim = extent();
		return rgb(prim, prim, prim);
	}
};

class pixel : public rgb
{
public:
	pixel () : rgb()
	{
	}

	pixel (rgb p_rgb)
	{
		r = p_rgb.r;
		g = p_rgb.g;
		b = p_rgb.b;
	}

	pixel (int p_r, int p_g, int p_b) : rgb(p_r, p_g, p_b)
	{
	}

	pixel_info operator - (pixel p_to)
	{
		return pixel_info(p_to.r - r, p_to.g - g, p_to.b - b);
	}
};

class pixel_parser {
	draw_surface* in;
	int x, x_end, y;
public:
	pixel_parser(int p_x1, int p_x2, int p_y, draw_surface* p_in)
	{
		x = p_x1;
		x_end = p_x2;
		y = p_y;
		in = p_in;
	}

	~pixel_parser()
	{
	}

	inline void advance()
	{
		x++;
	}

	inline int get_x()
	{
		return x;
	}

	inline int get_y()
	{
		return y;
	}

	inline int get_left()
	{
		return x - 1;
	}

	inline int get_right()
	{
		return x - 1;
	}

	inline bool at_end()
	{
		return x >= x_end;
	}
	
	inline pixel read(int offset_x = 0, int offset_y = 0)
	{
		return in->read_pxl(x + offset_x, y + offset_y);
	}

	inline pixel read_l()
	{
		return read(-1);
	}

	inline pixel read_r()
	{
		return read(+1);
	}

	inline pixel read_t()
	{
		return read(0, -1);
	}

	inline pixel read_b()
	{
		return read(0, +1);
	}

	inline pixel_info diff(int offset_x, int offset_y = 0)
	{
		return read() - read(offset_x, offset_y);
	}

	inline pixel_info diff_l()
	{
		return diff(-1);
	}

	inline pixel_info diff_r()
	{
		return diff(+1);
	}

	inline pixel_info diff_t()
	{
		return diff(0, -1);
	}

	inline pixel_info diff_b()
	{
		return diff(0, +1);
	}
};

typedef quantum_2d tracer_unit;
typedef std::list<tracer_unit> tracer_type;
typedef appendable_element<tracer_type*> tracer_base;

class tracer : public tracer_base {
public:
	tracer* connects;
	inline tracer ()
	{
		data = new tracer_type;
		connects = NULL;
	}

	inline ~tracer ()
	{
		data->clear();
		delete(data);
	}

	inline void store_lef(int x, int y)
	{
		data->push_front(tracer_unit(x, y));
	}

	inline void store_rig(int x, int y)
	{
		data->push_back(tracer_unit(x, y));
	}

	inline tracer_unit get_last_lef()
	{
		return data->front();
	}

	inline tracer_unit get_last_rig()
	{
		return data->back();
	}

	inline tracer_unit delete_lef()
	{
		tracer_unit ret = data->front();
		data->pop_front();
		return ret;
	}

	inline tracer_unit delete_rig()
	{
		tracer_unit ret = data->back();
		data->pop_back();
		return ret;
	}

	inline void add_inny(tracer* trace)
	{
		// todo: differentiate between an inny on the same level and an inny inside this tracer
		// do this with two separate pointers - within, beside?
		if (connects == NULL) { // nothing connected to this tracer (but could still be connected to tracer in this circuit)
			connects = trace;
		} else { // tracer already connected.  stick tracer in between
			trace->connects = connects;
			connects = trace;
		}
	}
};

// the segments that result from parsing a horizontal row of screen pixels
class row_segment {
public:
	int lef_x;
	int rig_x;
	tracer* lef_trace;
	tracer* rig_trace;
	row_segment* last_connection; // last connection below this segment

	row_segment()
	{
		lef_trace = rig_trace = NULL;
		last_connection = NULL;
	}

	row_segment(int p_lef_x, int p_rig_x, tracer* p_lef_trace, tracer* p_rig_trace)
	{
		lef_x = p_lef_x;
		rig_x = p_rig_x;
		lef_trace = p_lef_trace;
		rig_trace = p_rig_trace;
		last_connection = NULL;
	}
};
typedef std::vector<row_segment*> row_seg_set;
typedef row_seg_set::iterator row_segment_it;

template <class unit_type = point_2d, class container_type = std::vector<unit_type>>
class area_tracer {
public:
	acc_area<unit_type> acc;
	round_robin_iterator<unit_type, container_type> start, end;
	int first;

	inline void init(container_type& p_container, int p_start = 0)
	{
		start = end = round_robin_iterator<unit_type, container_type>(p_start, p_container);
		acc.init(get_element(p_start));
	}

	inline void init_rev(tracer_type* p_tracer)
	{
	}

	inline void set_start(int p_start)
	{
		start.e = p_start;
		acc.init(get_element(p_start));
	}

	area_tracer ()
	{
	}

	area_tracer (container_type& p_container, int p_start = 0)
	{
		init(p_container, p_start);
	}

	unit_type get_element(int n)
	{
		return start.get_element(n);
	}

	void reset()
	{
		start = end;
		acc.init(*start);
	}

	void iterate_tail()
	{
		start++;
		acc.sub_col_x2(*start);
	}

	inline void iterate_head()
	{
		end++;
		acc.add_col_x2(*end);
	}

	inline void iterate_head_rev()
	{
		end--;
		acc.sub_col_x2(*end);
	}

	/*
	bool iterate_fixed(int len = 10, double th = 0.5)
	{
		iterate_head();

		if (end - start >= len) {
			iterate_tail();
		}

		if (fabs(calc_ave_height()) > th) {
			last_area = acc.calc_tot_area_x2();
			reset();
			
			if (first < 0) {
				first = end - 1;
			}

			return true;
		} else {
			return false;
		}
	}
	*/

	inline bool nonlinear(double th = 0.5)
	{
		return fabs(calc_ave_height()) > th;
	}

	inline bool parse_span(double th = 0.5)
	{
		if (nonlinear(th)) {
			first = end.e.i - 1;
			reset();
			return true;
		} else {
			return false;
		}
	}

	bool iterate_span(double th = 0.5)
	{
		iterate_head();
		return parse_span(th);
	}

	bool iterate_span_rev(double th = 0.5)
	{
		iterate_head_rev();
		return parse_span(th);
	}

	int correct_vert(int tent, int e1, int e2)
	{
		max_value<double> mx;

		mess(e1, "e1");
		mess(e2, "e2");

		for (int i = e1; i < e2; i++) {
			tri_2d<> tri(
				get_element(interpolate(0.5, i, e1)),
				get_element(interpolate(0.5, i, e2)),
				get_element(i)
			);

			//mess(tri.area_x2(), "tri");
			mx.update(fabs(tri.area_x2()), i);
		}

		if (mx.report_key() < 0) {
			mess(mx.report_key(), "oh crap");
		}

		return mx.report_key();
	}

	double calc_area_x2()
	{
		return acc.calc_tot_area_x2();
	}

	double calc_area()
	{
		return acc.calc_tot_area();
	}

	double calc_ave_height()
	{
		return acc.calc_ave_height();
	}

	unit_type get_start_sample()
	{
		return *start;
	}

	unit_type get_end_sample()
	{
		return *end;
	}

	unit_type get_vertex()
	{
		return *(end - 1);
	}

	double get_chord_len_sqrd()
	{
		return get_start_sample().dist_sqrd(get_end_sample());
	}

	double diff_ave_height(area_tracer<tracer_type, unit_type> at)
	{
		return calc_ave_height() - at.calc_ave_height();
	}

	/*
	int iterate_worm(int limit = -1, float th = 0.5)
	{
		if ((limit == -1) || (end < limit)) {
			if (fabs(calc_ave_height()) > th) {
				iterate_tail();
				return 1;
			} else {
				iterate_head();
				return -1;
			}
		}

		return 0;
	}
	*/

	bool cont()
	{
		return end < round_robin_element<int>(0, 1);
	}

	bool cont_first()
	{
		return end < round_robin_element<int>(first, 1);
	}

	bool cont_rev()
	{
		return  !(lap != 0 && end < first);
	}
};

template <class tracer_type, class vert_type = point_2d>
class twin_area_tracer {
public:
	area_tracer<tracer_type, vert_type> a, b;

	inline void init(tracer_type* t, int start = 0, int end = 0, int mid = 0)
	{
		a.init(t, start);
		b.init(t, mid);
	}

	twin_area_tracer(tracer_type* t, int start = 0, int end = 0, int mid = 0)
	{
		init(t, start, end, mid);
	}

	bool iterate_mid_len()
	{
		if (b.get_chord_len_sqrd() >= a.get_chord_len_sqrd()) {
			a.iterate_head();
			b.iterate_tail();
			return true;
		}

		return false;
	}

	bool iterate_mid_area()
	{
		if (b.calc_area_x2() >= a.calc_area_x2()) {
			a.iterate_head();
			b.iterate_tail();
			return true;
		}

		return false;
	}

	bool iterate_head()
	{
		b.iterate_head();
		return iterate_mid_len();
	}

	bool iterate_tail()
	{
		a.iterate_tail();
		return iterate_mid_len();
	}

	vert_type get_front()
	{
		return b.get_end_sample();
	}

	vert_type get_back()
	{
		return a.get_start_sample();
	}

	vert_type get_mid()
	{
		return a.get_end_sample();
	}

	double diff_ave_height()
	{
		return a.diff_ave_height(b);
	}

	void reset()
	{
		a.set_start(b.end);
		b.set_start(b.end);
	}

	bool cont()
	{
		return b.cont();
	}
};

template <class tracer_type, class vert_type = point_2d>
class arc_tracer {
	tracer_type* tracer;
	double threshhold;
	int start, end;
	line_2d<> chord;
	mean<vert_type> mean_point;
	mean<vert_type> mean1;
	mean<vert_type> mean2;
	int norm_index;

	inline void init(tracer_type* p_tracer, int p_start = 0, int p_end = 0, float p_threshhold = 0.5)
	{
		tracer = p_tracer;
		start = p_start;
		end = p_end;
		threshhold = p_threshhold;
		norm_index = 0;
	}

public:

	inline arc_tracer (tracer_type* p_tracer, int p_start = 0, int p_end = 1, float p_threshhold = 0.5)
	{
		init(p_tracer, p_start, p_end, p_threshhold);
	}

	int get_start()
	{
		return start;
	}

	int get_end()
	{
		return end;
	}

	int get_mid_index(float alpha = 0.5)
	{
		return int(start + int(float(end - start) * alpha));
	}

	vert_type get_element(int n)
	{
		return (*tracer)[n];
	}

	vert_type get_mid(float alpha = 0.5)
	{
		return (*tracer)[get_mid_index(alpha)];
	}

	vert_type get_mean()
	{
		return mean_point.calc();
	}

	double get_mean_height()
	{
		chord = line_2d<>((*tracer)[start], (*tracer)[end]);
		return fabs(chord.distance(mean_point.calc()));
	}

	inline double chord_length()
	{
		return ((*tracer)[start]).dist((*tracer)[end]);
	}

	inline void update_norm_index()
	{
		line_2d<> norm((*tracer)[start], (*tracer)[end], true);

		while (norm.relative((*tracer)[norm_index]) == norm.relative((*tracer)[norm_index + 1])) {
			norm_index++;
		}

		end++;
	}

	inline bool update_threshhold(double p_threshhold)
	{
		chord = line_2d<>((*tracer)[start], (*tracer)[end]);

		if (fabs(chord.distance(mean_point.calc())) > p_threshhold) {
			start++;
			mean_point.cull((*tracer)[start]);
			return true;
		} else {
			mean_point.update((*tracer)[end]);
			end++;
			if (end >= (*tracer).size() - 1) {
				end = 0;
			}
			return false;
		}
	}

	inline bool update_mean()
	{
		chord = line_2d<>((*tracer)[start], (*tracer)[end]);

		if (chord.distance(mean_point.calc()) > threshhold) {
			mean_point.update((*tracer)[end]);
			end++;
			if (end >= tracer->size()) {
				end = 0;
			}
			return true;
		} else {
			mean_point.cull((*tracer)[start]);
			start++;
			return false;
		}
	}

	inline void update_perp()
	{
		mean_point.update(*tracer[end]);
		chord = line_2d<>(*tracer[start], *tracer[end]);
		//line_2
		//while (
	}

	bool update_arc()
	{
		end++;
		return true;
	}

	/* report arc extrapolate 50% of radius sample */
	double report_arc_ext_half()
	{
		vert_type p1 = get_mid(0.0);
		vert_type p2 = get_mid(0.33333333333);
		vert_type p3 = get_mid(0.66666666666);
		vert_type p4 = get_mid(1.0);
		vert_type p5 = get_mid(1.0);

		show_point(p1, RGB_RED);
		show_point(p2, RGB_GREEN);
		show_point(p3, RGB_BLUE);
		show_point(p4, RGB_WHITE);
		//show_point(p5, RGB_GREY);

		circle_2d<> circ(p1, p2, p3);
		double d = circ.dist(p4);
		if (fabs(d) > 1.0) show_text("diff", d);
		return d;
	}

	/* report arc extrapolate one-whole of radius sample */
	double report_arc_ext_whole()
	{
		vert_type p1 = get_mid(0.0);
		vert_type p2 = get_mid(0.25);
		vert_type p3 = get_mid(0.5);
		vert_type p4 = get_mid(1.0);

		show_point(p1, RGB_RED);
		//show_point(p2, RGB_GREEN);
		//show_point(p3, RGB_BLUE);
		show_point(p4, RGB_WHITE);

		circle_2d<> circ(p1, p2, p3);
		double d = circ.dist(p4);
		//if (fabs(d) > 6.0) show_text("diff", d);
		//return d;
		return tolerance(circ.radius, circ.dist_center(p4));
	}

	/* report arc interpolate with cross radius calculation */
	double report_arc_cross()
	{
		vert_type p1 = get_mid(0.0);
		vert_type p2 = get_mid(0.25);
		vert_type p3 = get_mid(0.5);
		vert_type p4 = get_mid(0.75);
		vert_type p5 = get_mid(1.0);

		show_point(p1, RGB_RED);
		show_point(p2, RGB_GREEN);
		show_point(p3, RGB_BLUE);
		show_point(p4, RGB_WHITE);
		show_point(p5, RGB_GREY);

		circle_2d<> circ(p1, p4, p2, p5);
		double d = circ.dist(p3);
		if (fabs(d) > 1.5) { 
			show_text("diff", d);
			mess((float)d, "diff");
		}

		return d; //tolerance(circ.radius, circ.dist_center(p3));
		//return d;
	}

	void find_apogee()
	{
		max_value<double> apogee;
		line_2d<> chord((*tracer)[start], (*tracer)[end]);

		for (int i = start; i < end; i++) {
			apogee.update(chord.distance((*tracer)[i]), i);
		}

		show_point((*tracer)[apogee.report_key()], RGB_GREEN);
		show_windows();
	}

	double report_arc_rads()
	{
		vert_type p1 = get_mid(0.0);
		vert_type p2 = get_mid(0.25);
		vert_type p3 = get_mid(0.5);
		vert_type p4 = get_mid(0.75);
		vert_type p5 = get_mid(1.0);

		show_point(p1, RGB_RED);
		show_point(p2, RGB_GREEN);
		show_point(p3, RGB_BLUE);
		show_point(p4, RGB_WHITE);
		show_point(p5, RGB_GREY);

		circle_2d<> c1(p1, p2, p3);
		circle_2d<> c2(p3, p4, p5);

		mess((float)c1.radius, "r1");
		mess((float)c2.radius, "r2");
		
		return tolerance(c1.radius, c2.radius);
	}

	void report_angle()
	{

	}

	/* report arc interpolate with dual sample calculation */
	double report_arc_dual()
	{
		vert_type p1 = get_mid(0.0);
		vert_type p2 = get_mid(0.25);
		vert_type p3 = get_mid(0.5);
		vert_type p4 = get_mid(0.75);
		vert_type p5 = get_mid(1.0);

		show_point(p1, RGB_RED);
		show_point(p2, RGB_GREEN);
		show_point(p3, RGB_BLUE);
		show_point(p4, RGB_WHITE);
		show_point(p5, RGB_GREY);

		circle_2d<> circ(p1, p3, p5);
		double d1 = circ.dist(p2);
		double d2 = circ.dist(p4);
		double d = d1 - d2; //(d1 + d2) * 0.5;

		show_point(circ.center, RGB_GREEN);
		//if (fabs(d1) > 1.25) show_text("diff 1", float(d1));
		//if (fabs(d2) > 1.25) show_text("diff 2", float(d2));
		if (fabs(d) > 1.0) show_text("diff 1-2", float(d));
		mess((float)d, "d 1-2");
		return d;//tolerance(circ.radius, circ.dist_center(p4));
	}

	inline bool cont()
	{
		return start < tracer->size() - 2;
	}
};

template <class element_type>
class linear_tracer {

	double threshold;
	double ct;
	point_2dd vert_acc;
	line_2d_dydx curr_line;
	//line_2d_gen<element_type> curr_line;
	vect_2d<float> start;
	vect_2d<float> prev;
	element_type prev_p;
public:

	linear_tracer(double p_threshhold = 1.01)
	{
		threshold = p_threshhold;
		init();
	}

	~linear_tracer()
	{
	}

	void init()
	{
		vert_acc = VECT_2D_DOUBLE_ORIG;
		ct = 0.0;
	}

	bool compare(point_2d p_p)
	{
		double offset = curr_line.distance(p_p);
		//show_text("offset", offset);
		return ((offset > threshold) || (offset < -threshold));
	}

	bool update(element_type p)
	{
		bool ret = false;
		vect_2d<float> curr;

		if (ct == 0.0) {
			curr = point_2d(0.0, 0.0);
		} else if (ct == 1.0) {
			curr = p.mean(prev_p);
			start = curr;
		} else if (ct == 2.0) {
			curr = p.mean(prev_p);
		} else if (prev != start) {
			curr = p.mean(prev_p);
			curr_line = line_2d_dydx(start, prev, vert_acc / (ct - 1));

			/* process corner */
			if (compare(curr)) {
				ct = 2.0;
				start = prev;
				vert_acc = start;
				ret = true;
			} 
		} else {
			curr = point_2d(0.0, 0.0);
			//show_text("ps", start.report());
		}

		vert_acc += curr;
		ct++;
		prev = curr;
		prev_p = p;

		return ret;
	}

	element_type get_corner()
	{
		return prev;
	}

	line_2d_gen<element_type> get_curr_line()
	{
		return curr_line;
	}
};

template <class gen>
class linear_auditor {
	
	line_2d_gen<gen> line; 
	double tolerance;

public:

	linear_auditor(line_2d_gen<gen> p_line, double p_tolerance = 0.51)
	{
		line = p_line;
		tolerance = p_tolerance;
	}

	bool audit(gen p_p)
	{
		double offset = line.offset(p_p.center());
		//show_point(gen(int(line.func(p_p) + .5), int(p_p.y)), RGB_BLUE, 0);
		if (offset > tolerance || offset < -tolerance) {
			//show_text(line.get_dxdy().func(p_p.y + .5) - (p_p.x + .5));
			//show_text(p_p.x + .5);
			//show_text("");
			//show_text(offset);
		}
		return (offset > tolerance) || (offset < -tolerance);
	}

	/*
	bool audit_dydx(gen p_p)
	{
		point_2d p = p_p.center();
		double y = line.dydx.func(p.x);
		//if ((fabs(p.y - y) > tolerance) || (fabs(p.y - y) < -tolerance)) show_text(p.y - y);
		//show_text(line.dydx.m);
		//show_text(line.dxdy.m);
		//show_point(vect_2d<int>(p.x, y), RGB_RED, 1);
		return (p.y - y > tolerance) || (p.y - y < -tolerance);
	}
	*/

	/*
	bool audit(gen p_p)
	{
		double dep = line.func(p_p);
		//if ((fabs(p.x - x) > tolerance) || (fabs(p.x - x) < -tolerance)) show_text(p.x - x);
		show_text(line.dxdy.m);
		show_text(line.dydx.m);
		//show_point(vect_2d<int>(x, p.y), RGB_RED, 1);
		return (p.x - x > tolerance) || (p.x - x < -tolerance);
	}

	bool audit(gen p_p)
	{
		if (line.is_dydx()) {
			return audit_dydx(p_p);
		} else {
			return audit_dxdy(p_p);
		}
	}
	*/
};

class feature {
	// a corner is a curve with radius of (0, 0) and arc = angle
	// an edge is a curve with radius of (+inf, +inf) and arc = segment length
	float radius[2];
	float arc;

	inline feature(float p_arc, float p_r1, float p_r2)
	{
		radius[0] = p_r1;
		radius[1] = p_r2;
		arc = p_arc;
	}
};

typedef std::vector<feature> feature_bank;
typedef std::vector<tracer_unit> circuit_type;
typedef std::vector<point_2d> mids_type;

class vertex {
public:
	point_2d pixel;
	int index;

	vertex (point_2d p_pixel, int p_index)
	{
		pixel = p_pixel;
		index = p_index;
	}
};

/* this is a class that holds a complete circuit of tracers once
     a blob has been completely parsed */
class shape {
public:
	std::vector<vertex> verts;
	tracer_unit prev_el;
	linear_tracer<tracer_unit> lt;
	circuit_type circuit;
	mids_type mids;

	void init()
	{
		prev_el = VECT_2D_INT_UNDEF_VALUE;
	}

	void log_vertex(area_tracer<>& t)
	{
		verts.push_back(vertex(t.get_vertex(), (t.end - 1).iteration()));
		//show_point(v, RGB_RED);
		//show_windows();
	}
	
	void process_element(tracer_unit u)
	{
		//show_text("x", u.x);
		//show_text("y", u.y);
		color_point(u.x, u.y, RGB_BLUE);
		//show_windows(0);
		/*
		mess(u.x - 10, "x");
		mess(u.y - 10, "y");

		return;
		*/

		/*
		if (read_point_in(u.x, u.y) == RGB_WHITE) {
			color_point(u.x, u.y, RGB_RED);
		} else {
			color_point(u.x, u.y, RGB_BLUE);
		}
		*/

		circuit.push_back(u);
		if (!prev_el.is_undef()) {
			mids.push_back(u.mean(prev_el));
			//show_text("curr", prev_el.report());
		}
		prev_el = u;
	}

	void process_curves()
	{
		mean<point_2d> ac(POINT_2D_ORIG);
		mean<double> ar;

		for (unsigned i = 0; i < circuit.size(); i++) {
			point_2d p1 = circuit[0].to_cart_coord();
			point_2d p2 = circuit[i % circuit.size()].to_cart_coord();
			point_2d bot = p1.center(p2);
			point_2d pc = circuit[int(i * 0.5 + 0.5)].to_cart_coord();
			ray_2d se(p1, p2);
			ray_2d norm(float(se.angle - PI * 0.5), pc);
			double c = pc.dist(bot);

			if (c > 2.0) {
				double s = 0.5 * p1.dist(p2);
				double r = s / sin(2 * atan(c / s));
				ar.update(r);
				point_2d cn = norm.extend(ar.calc()).alias_pixel();
				ac.update(cn);

				show_text(r);
				//show_point(p2.alias_pixel(), RGB_GREEN);
				show_point(ac.calc());
				//show_windows();
			} else {
				//show_point(p2.alias_pixel(), RGB_BLUE);
			}
		}
	}

	void process_between(tracer_unit p1, tracer_unit p2)
	{
		if (p1.x < p2.x) {
			if (p1.y < p2.y) { // case 1 (changed)
				process_element(tracer_unit(p1.x, p1.y));
				for (int x = p1.x; x < p2.x; x++) {
					process_element(tracer_unit(x, p2.y));
				}
			} else if (p1.y > p2.y) { // case 2 (changed)
				for (int x = p1.x; x <= p2.x; x++) {
					process_element(tracer_unit(x, p1.y));
				}
			} else { // case 3 (not changed)
				for (int x = p1.x; x < p2.x; x++) {
					process_element(tracer_unit(x, p1.y));
				}
			}
		} else if (p1.x > p2.x) {
			if (p1.y < p2.y) { // case 4 (changed)
				process_element(tracer_unit(p1.x, p1.y));
				for (int x = p1.x; x > p2.x; x--) {
					process_element(tracer_unit(x, p2.y));
				}
			} else if (p1.y > p2.y) { // case 5 (changed)
				//process_element(tracer_unit(p1.x, p1.y));
				for (int x = p1.x; x >= p2.x; x--) {
					process_element(tracer_unit(x, p1.y));
				}
			} else { // case 6 (not changed)
				for (int x = p1.x; x > p2.x; x--) {
					process_element(tracer_unit(x, p1.y));
				}
			}
		} else { // case 7 (not changed)
			process_element(tracer_unit(p1.x, p1.y));
		}
	}

	void process_circuit(feature_bank* p_bank, tracer* p_circuit_start, rgb p_color)
	{
		static int ct = 0;
		tracer_unit first, last, p(VECT_2D_INT_UNDEF_VALUE);
		tracer_unit prev(VECT_2D_INT_UNDEF_VALUE);
		tracer* prev_tracer;
		tracer_type::iterator prev_corner;
		bool rep;
		prev_tracer = p_circuit_start;
		prev_corner = p_circuit_start->data->begin();

		ct++;
		show_text("begin", (double)ct);
		//show_windows();

		if (ct != 4) {
			//return;
		}

		// iterate through tracers in circuit
		rep = false;
		for (tracer* t = p_circuit_start; t != (tracer*)_alt_null.get_null(ALT_NULL_NA); t = (tracer*)t->next) {
			// iterate through all points in tracer
			for (tracer_type::iterator u = t->data->begin(); u != t->data->end(); ++u) {				
				if (p != VECT_2D_INT_UNDEF_VALUE) {
					process_between(p, *u);
				}
				p = *u;
			}

			// process connector (inny) here on the fly? 
			// or maybe queue them to process after outy?
			if (t->connects != NULL) {
				//shape sh(t->connects);
			}

			rep = true;
		}
	
		process_between(p, *p_circuit_start->data->begin());
		p = *p_circuit_start->data->begin();

		//process_test4();
		test_current();
		/*
		process_corners();
		correct_verts();
		*/

		//process_curves();
		//process_test3();
		//process_test2();
		//identify();
}

void process_test4()
{
	mean<point_2d> ave;
	mean<angle_2d<double>> ang;
	line_2d_dydx secant;
	tracer_unit prev_circ;
	int start_i, end_i;
	int curve_start = -1;
	angle_2d<double> angle_start = -1000.0;
	angle_2d<double> prev_angle;
	double len = 0;
	double dlen;
	
	start_i = 0;
	for (start_i = end_i = 0; start_i < mids.size(); ) {
		secant = line_2d_dydx(mids[start_i % mids.size()] , mids[end_i % mids.size()]);
		int mid_low = int((start_i + end_i) / 2);

		show_point(mids[start_i % mids.size()], RGB_RED);
		show_point(mids[end_i % mids.size()], RGB_GREEN);
		show_point(ave.calc(), RGB_YELLOW);
		//show_windows();

		double d2 = secant.distance(ave.calc());
		quantum_2d c(351, 160);
		angle_2d<double> a_calc = secant.angles().correct_360();
		angle_2d<double> a_real(ave.calc(), c);

		if (fabs(d2) > 0.5) {
			if (curve_start == -1) {
				curve_start = mid_low;
				angle_start = a_calc;
			}
			ave.cull(mids[start_i % mids.size()]);
			ang.cull(a_calc);
			start_i++;
		} else {
			ave.update(mids[end_i % mids.size()]);
			ang.update(a_calc);
			end_i++;
		}

		if (start_i == mids.size()) {
			start_i = 0;
		}
		if (end_i == mids.size()) {
			end_i = 0;
		}

		dlen = 0.0;
		if (curve_start != -1) {
			if (a_calc.pxl_to_coord().xtant() & 1) {
				if (!approx_equal(prev_circ.x, circuit[mid_low % mids.size()].x)) {
					dlen = 1.0 / cos(a_calc.angle);
				}
			} else {
				if (!approx_equal(prev_circ.y, circuit[mid_low % mids.size()].y)) {
					dlen = 1.0 / sin(a_calc.angle);
				}
			}
			
			//mess((float)(len / (ang.calc().angle - angle_start.angle)), "~r");
			len += fabs(dlen);
			//mess((float)a_calc.to_degrees(), "a");
			//mess((float)angle_start.to_degrees(), "a start");
			//mess((float)(ang.calc()).to_degrees(), "ang");
		}

		prev_circ = circuit[mid_low % mids.size()];
		prev_angle = a_calc;

		double w = circuit[0].dist(circuit[end_i]);
		line_2d_dydx chord(circuit[0], circuit[end_i]);
		//double h = chord.distance(circuit[end_i / 2]);
		double h = chord.distance(ave.calc());

		//mess((float)radius_from_arc(h, w), "rad from arc");
	}

}

void test4b()
{
	int start;
	int end;
	min_value<double> min;
	max_value<double> max;
	mean<point_2d> point_mean;

	for (start = end = 0; end < mids.size(); ) {
		point_2d p1 = mids[start];
		point_2d p2 = mids[(end + start) >> 1];
		point_2d p3 = mids[end];
		circle_2d<> circle(p1, p2, p3);
		double d = circle.dist(p1.center(p3));
		double h = p2.dist(p1.center(p3));

		show_point(mids[(end + start) >> 1], RGB_PURPLE);
		circle_2d<> ci(p1, point_mean.calc(), p3);
		//h = ci.radius;

		if (h < 1.0) {
			point_mean.update(mids[end]);
			end++;
		} else {
			point_mean.cull(mids[start]);
			min.update(ci.radius, (end + start) >> 1);
			max.update(ci.radius, (end + start) >> 1);
			show_point(mids[min.report_key()], RGB_GREEN);
			show_point(mids[max.report_key()], RGB_RED);
			show_windows();

			if (end % 25 == 0) {
				show_text(p1.dist(p2));
				mess((float)min.report_value(), "min");
				//mess(
			}
			start++;
		}

		//if (i % 25 == 0) {
			//show_text("dist", d);
		/*
			show_point(p1, RGB_GREEN);
			show_point(p3, RGB_RED);
			show_point(p2, RGB_YELLOW);
		*/
		
		//}
	}
}

void test5()
{
	int start = 0;

	for (int i = 0; i < mids.size(); i++) {
		int mid = int((start + i) * 0.5);
		int m1 = int((start + mid) * 0.5);
		int m2 = int((i + mid) * 0.5);

		line_2d_dydx l1(mids[start], mids[mid]);
		line_2d_dydx l2(mids[mid], mids[i]);

		line_2d_dydx r1 = l1.perp_line(mids[start].center(mids[mid]));
		line_2d_dydx r2 = l2.perp_line(mids[mid].center(mids[i]));

		point_2d p = r1.intersect(r2);

		double d1 = p.dist(mids[start]);
		double d2 = p.dist(mids[i]);
		double dm = p.dist(mids[mid]);
		double da = p.dist(mids[m1]);
		double db = p.dist(mids[m2]);
		double d3 = (d1 + d2 + dm) * (1.0 / 3.0);

		//mess((float)d1, "d1");
		//mess((float)d2, "d2");
		//mess((float)(dm - (d1 + d2) * 0.5), "diff");
		//mess((float)((d1 + d2) * 0.5), "~r");
		//mess((float)(da - d3), "a");
		//mess((float)(db - d3), "b");

		double th = 1.1;
		if ((fabs(da - d3) > th) || (fabs(db - d3) > th)) {
			show_text("diff a", da - d3);
			show_text("diff b", db - d3);
			//show_text("r1", d1);
			//show_text("r2", d2);
			show_point(mids[i], RGB_RED);
			show_point(mids[m1], RGB_YELLOW);
			show_point(mids[m2], RGB_YELLOW);
			show_point(mids[start], RGB_GREEN);
			show_point(mids[mid], RGB_WHITE);
			show_windows();
			start = i;
		}

		/*
		show_point(mids[start], RGB_RED);
		show_point(mids[i], RGB_GREEN);
		show_point(mids[mid], RGB_YELLOW);
		show_windows();
		*/
	}
}

void test_current()
{
	test_current_fwd();
	//test_current_rev();
	//test_retrace();
	//correct_verts3();
	refine_verts();
}

void test_current_fwd()
{
	area_tracer<> t(mids);

	while (t.cont_first()) {
		//show_point(t.get_end_sample(), RGB_GREEN, 0);
		//show_windows();
		if (t.iterate_span()) {
			log_vertex(t);
			show_point(t.get_vertex(), RGB_RED);
		}
	}
}

void refine_verts()
{
	/*
	for (round_robin_iterator<vertex> i(0, verts); i.cont(); ++i) {
		show_point((*i).pixel, RGB_YELLOW);
	}
	*/

	for (round_robin_iterator<vertex> i(0, verts); i.cont(); ++i) {
		//show_point((*i).pixel, RGB_YELLOW);
		max_value<double, round_robin_iterator<point_2d>> mx(;
		point_2d p1 = (i - 1)->pixel;
		point_2d p2 =  (i + 1)->pixel;

		for (round_robin_iterator<point_2d> mid((i - 1)->index, mids); mid <= (i + 1)->index; ++mid) {
			tri_2d<> tri(
				p1, *mid, p2
			);
			mx.update(fabs(tri.area_x2()), mid);aqa
			//mess(tri.area_x2(), "area");
		}

		//mess(mx.report_key().e.i, "i");
		show_point(*mx.report_key(), RGB_PURPLE, 0);
	}
}

void correct_verts3()
{
	/*
	area_tracer<mids_type, point_2d> t(&mids);

	for (int i = 0; i < verts.size(); i++) {
		int p = 
			t.correct_vert(
				verts[(i + 1) % verts.size()].index, 
				verts[(i + 0) % verts.size()].index, 
				verts[(i + 2) % verts.size()].index
			);
		//verts[i + 1].index = p;
		//mess(p, "p");
		show_point(t.get_element(p), RGB_YELLOW, 0);
		show_windows(0);
	}
	*/
}

void distinct_angle(point_2d p1, point_2d p2, point_2d p3, bool& d1, bool& d2, double thresh = 10.0)
{
	point_2d delta1 = p2 - p1;
	point_2d delta2 = p2 - p3;
	angle_2d<> a(p1, p2, p3);
	double ext = a.supplement().angle * 0.5;
	double distinct1 = ext * p2.dist_sqrd(p1);
	double distinct2 = ext * p2.dist_sqrd(p3);

	//mess((float)a, "a");
	//mess((float)((PI - fabs(a2 - a1)) * (180 / (2 * PI))), "s");
	mess((float)distinct1, "d1");
	mess((float)distinct2, "d2");

	d1 = (distinct1 > thresh);
	d2 = (distinct2 > thresh);
}

/*
		double w = circuit[0].dist(circuit[end_i]);
		line_2d_dydx chord(circuit[0], circuit[end_i]);
		double h = chord.distance(circuit[end_i / 2]);
*/

/*
void identify()
{
	mean<point_2d> mea(POINT_2DD_ORIG);
	point_2d start = circuit[0];

	for (int i = 0; i < circuit.size(); i++) {
		color_point(circuit[i], RGB_RED);
		mea.update(circuit[i]);
		color_point(mea.calc(), RGB_GREEN);
		tri_2d<point_2d> tri(start, mea.calc(), circuit[i]);
	}
}
*/

public:

	feature_bank perimeter;
	std::vector<feature_bank*> voids;

	shape()
	{
		init();
	}

	// shape is created when the perimeter is completed
	// and so the proto-shape is completed
	shape(tracer* p_outy)
	{
		init();
		// process perimeter (outie)
		process_circuit(&perimeter, p_outy, RGB_GREY);		
	}

	inline void add_inny(tracer* p_inny)
	{
		voids.push_back(new feature_bank());
		process_circuit(voids.back(), p_inny, RGB_RED);
	}
};

class circuit_bank {
	std::vector<shape*> shapes;
	std::vector<row_segment*> connections; // temp buffer for segments connected above segment
public:
	void clear_connections()
	{
		connections.clear();
	}

	void add_connection(row_segment* seg)
	{
		connections.push_back(seg);
	}

	/*
	void trace2(int lef_x, int rig_x, int y, tracer*& lef_trace, tracer*& rig_trace)
	{
		// connections are segments above the current scan line that connect to scan line
		// last_connection is  the 
		if (connections.size()) { // if there were connections above segment
			lef_trace = connections.front()->lef_trace;
			lef_trace->store_lef(lef_x, y);
			//show_point(quantum_2d(lef_x, y), RGB_BLUE);
			//show_point(quantum_2d(unit.x, unit.y), RGB_GREEN);
			//show_windows();

			std::vector<row_segment*>::iterator i = connections.begin();
			while (true) {
				if ((*i)->last_connection != NULL) { // diverge case - has a previous connection
					(*i)->last_connection->lef_trace->delete_lef();
					tracer_unit unit = (*i)->last_connection->rig_trace->delete_rig();
					(*i)->last_connection->rig_trace = lef_trace = new tracer();
					// when we do an inny, we go in the reverse direction
					lef_trace->store_lef(lef_x, y);
					//show_point(quantum_2d(lef_x, y), RGB_GREEN);
					//lef_trace->store_lef(lef_x - 1, y - 1);
					//lef_trace->store_rig(unit.x + 1, unit.y - 1);
					lef_trace->store_rig(unit.x, unit.y); 
					//show_point(quantum_2d(unit.x, unit.y), RGB_ORANGE);
					//show_point(quantum_2d(unit.x, y), RGB_GREY);
					//show_windows();
				}
				

				std::vector<row_segment*>::iterator prev = i;
				++i;
				if (i == connections.end()) {
					break;
				}

				color_point_in(lef_x + 1, y + 1, RGB_BLACK);
				color_point_in(rig_x - 1, y + 1, RGB_BLACK);
				show_text("lef_x", lef_x);
				show_text("rig_x", rig_x);
				show_text("y", y);

				// converge case
				if ((*prev)->rig_trace->append((*i)->lef_trace)) {	// complete inny circuit
					connections.front()->lef_trace->add_inny((*i)->lef_trace);
				}
			}

			rig_trace = connections.back()->rig_trace;
			rig_trace->store_rig(rig_x, y);

			//show_point(quantum_2d(rig_x, y), RGB_BLUE);
		} else { // new tracer
			lef_trace = rig_trace = new tracer();
			lef_trace->store_lef(lef_x, y);
			//show_point(quantum_2d(lef_x, y), RGB_GREY);
			lef_trace->store_rig(rig_x, y);
			//show_point(quantum_2d(rig_x, y), RGB_RED);
		}
	}
	*/
	
	void trace(int lef_x, int rig_x, int y, tracer*& lef_trace, tracer*& rig_trace)
	{
		std::vector<row_segment*>::iterator prev;

		// connections are segments above the current scan line that connect to scan line

		if (connections.size()) { // if there were connections above segment
			lef_trace = connections.front()->lef_trace;
			lef_trace->store_lef(lef_x, y);

			std::vector<row_segment*>::iterator i = connections.begin();
			while (true) {
				if ((*i)->last_connection != NULL) { // diverge case - has a previous connection
					connections.front()->lef_trace->delete_lef();
					tracer_unit unit = (*i)->last_connection->rig_trace->delete_rig();

					(*i)->last_connection->rig_trace = lef_trace = new tracer();
					// when we do an inny, we go in the reverse direction
					lef_trace->store_lef(lef_x, y);
					lef_trace->store_rig(unit.x, unit.y); 
				} else {
				}

				if (i != connections.begin()) {
					//color_point_in((*prev)->rig_x + 1, y, RGB_BLACK);
					//color_point_in((*i)->lef_x - 1, y, RGB_BLUE);
					(*prev)->rig_trace->store_rig((*prev)->rig_x + 1, y);
					(*i)->lef_trace->store_lef((*i)->lef_x - 1, y);
					//show_windows();
				}

				prev = i;
				++i;
				if (i == connections.end()) {
					break;
				}

				// converge case
				if ((*prev)->rig_trace->append((*i)->lef_trace)) {	// complete inny circuit
					connections.front()->lef_trace->add_inny((*i)->lef_trace);
				} else {
				}
			}

			rig_trace = connections.back()->rig_trace;
			rig_trace->store_rig(rig_x + 1, y);

			/*
			color_point_in(rig_x, y, RGB_RED);
			show_windows();
			*/
		} else { // new tracer
			lef_trace = rig_trace = new tracer();
			lef_trace->store_lef(lef_x, y);
			lef_trace->store_rig(rig_x + 1, y);
		}
	}

	// last connection is the most downward and rightward connection to the "connection" segment
	void set_connections(row_segment* seg)
	{
		for (std::vector<row_segment*>::iterator i = connections.begin(); i != connections.end(); ++i) {
			(*i)->last_connection = seg;
		}
	}

	void tie_ends(row_seg_set* segs)
	{
		for (row_seg_set::iterator i = segs->begin(); i != segs->end(); ++i) {
			if ((*i)->last_connection == NULL) {
				tracer_unit ll = (*i)->lef_trace->get_last_lef();
				tracer_unit lr = (*i)->rig_trace->get_last_rig();
				(*i)->lef_trace->store_lef(ll.x, ll.y + 1);
				(*i)->rig_trace->store_rig(lr.x, ll.y + 1);

				if ((*i)->rig_trace->append((*i)->lef_trace)) {
					new_shape((*i)->lef_trace);
				}
			}
		}
	}

	void new_shape(tracer* p_perimeter)
	{
		shapes.push_back(new shape(p_perimeter));
	}
};

class row_parser {
	row_seg_set segments;
	bool seg_at_end;
	int xcut;
public:
	int test;
	row_segment_it locate_segment_it;

	inline row_parser(int p_xcut = 0)
	{
		seg_at_end = true;
		xcut = p_xcut;
	}

	inline ~row_parser()
	{
		clear();
	}

	inline row_seg_set* get_segs()
	{
		return &segments;
	}

	inline row_segment* add_segment(int p_x1, int p_x2, tracer* p_lef_trace, tracer* p_rig_trace)
	{
		row_segment* seg = new row_segment(p_x1, p_x2, p_lef_trace, p_rig_trace);
		segments.push_back(seg);
		return seg;
	}

	inline void init_locate_segment()
	{
		locate_segment_it = segments.begin();
	}

	row_segment_it locate_segment(int x)
	{
		while (locate_segment_it != segments.end() && (*locate_segment_it)->rig_x < x) {
			++locate_segment_it;
		}

		seg_at_end = (locate_segment_it == segments.end());
		return locate_segment_it;
	}

	inline bool segment_found()
	{
		return !seg_at_end;
	}

	inline bool segment_null()
	{
		return locate_segment_it == segments.end();
	}

	void clear()
	{
		for (row_segment_it i = segments.begin(); i != segments.end(); ++i) {
			delete *i;
		}
		segments.clear();
	}

	inline bool parse_pixel(pixel_parser parse, int extent, int offset_x = 0, int offset_y = 0)
	{
		if (parse.get_x() + offset_x < xcut ) {
			return true;
		} else {
			int ext = parse.diff(offset_x, offset_y).extent();
			return ext <= extent;
		}
	}

	void parse(draw_surface* in, row_parser* prev, circuit_bank& circuits, int extent, int y, int x1, int x2)
	{
		pixel_parser parse(x1, x2, y, in);
		int left, right;

		clear();
		prev->init_locate_segment();

		while (!parse.at_end()) {
			// while broken color
			while (!parse.at_end() && !parse_pixel(parse, extent, -1)) {
				parse.advance();
			}			
			left = parse.get_left();

			//poly = NULL;
			circuits.clear_connections();
			row_segment* prev_it = NULL;

			// while continuous color
			while (!parse.at_end() && parse_pixel(parse, extent, -1)) {
				if (parse_pixel(parse, extent, 0, -1)) {
					row_segment_it it = prev->locate_segment(parse.get_x());
					if (prev->segment_found() && (*it != prev_it)) {
						circuits.add_connection(*it);
						prev_it = *it;
					}
				}
				parse.advance();
			}			
			right = parse.get_right();

			if (left <= right) {
				tracer* lef_tracer = NULL;
				tracer* rig_tracer = NULL;
				circuits.trace(left, right, y, lef_tracer, rig_tracer);
				row_segment* seg = add_segment(left, right, lef_tracer, rig_tracer);
				circuits.set_connections(seg);
			}
		}

		circuits.tie_ends(prev->get_segs());
	}

	void report(draw_surface* p_in, int p_y, rgb color = RGB_GREEN)
	{
		for (row_segment_it i = segments.begin(); i != segments.end(); ++i) {
			p_in->line_seg((int)(*i)->lef_x, p_y, (int)(*i)->rig_x, p_y, color);
		}
	}
};

void group_filter(draw_surface* in, draw_surface* out = NULL)
{
	int length = in->get_dims().x;
	int end = length - 1;
	row_parser row1, row2;
	row_parser* curr = &row1;
	row_parser* prev = &row2;
	int extent = 10;
	circuit_bank circuits;

	for (int y = 0; y < in->get_dims().y - 1; y++) {
		curr->parse(in, prev, circuits, extent, y, 0, length);
		swap(curr, prev);
	}
	circuits.tie_ends(prev->get_segs());
}

#endif // !INC_COMPUTER_VISION