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

#include "C:\Users\Owner\Documents\Visual Studio 2010\Library\math_1d.h"
#include "C:\Users\Owner\Documents\Visual Studio 2010\Library\math_2d.h"
#include "C:\Users\Owner\Documents\Visual Studio 2010\Library\my_stl.h"
#include <vector>
#include <list>

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
		return x;
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
typedef appendable_element<std::list<tracer_unit>*> tracer_base;

class tracer : public tracer_base {
public:
	tracer* connects;
	inline tracer ()
	{
		data = new std::list<tracer_unit>;
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
typedef std::list<row_segment*> row_seg_set;
typedef row_seg_set::iterator row_segment_it;

template <class gen>
class linear_tracer {
public:
	float threshold;
	double ct;
	point_2dd vert_acc;
	gen start;
	gen prev;

	linear_tracer(float p_threshold)
	{
		threshold = p_threshold;
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

	bool update(gen curr)
	{
		bool ret = false;

		if (ct == 0.0) {
			start = curr;
		} else if (prev != start) {			
			point_2d curr_center = curr.center();
			point_2d delta = prev.center() - start.center();
			float offset;

			if (fabs(delta.x) > fabs(delta.y)) {
				line_2d_dydx ave_l(delta.x, delta.y, vert_acc / ct);
				offset = ave_l.func(curr_center.x) - curr_center.y;
			} else {
				line_2d_dxdy ave_l(delta.y, delta.x, vert_acc / ct);
				offset = ave_l.func(curr_center.y) - curr_center.x;
			}

			if ((offset > threshold) || (offset < -threshold)) {
				ct = 1.0;
				start = prev;
				vert_acc = start.center();
				ret = true;
				//_test_ds[TEST_DS_OUT_GFX_1]->write_pxl(start, RGB_RED);
				blit_windows(-1);
			} 
		}

		prev = curr;
		vert_acc += prev.center();
		ct++;

		return ret;
	}

	gen get_corner()
	{
		return prev;
	}
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

typedef std::list<feature> feature_bank;

class shape {
	void fine_process(
		int p_i1, int p_i2, 
		tracer* p_t1, tracer* p_t_start, 
		std::list<tracer_unit>::iterator p_u1, std::list<tracer_unit>::iterator p_u2)
	{
		int i = p_i1;
		tracer* t = p_t1;
		for (std::list<tracer_unit>::iterator u = p_u1; !(u == p_u2); ++u) {
			if (u == t->data->end()) {
				t = (tracer*)t->next;
				u = t->data->begin();				
			}

			_test_ds[TEST_DS_OUT_GFX_1]->write_pxl(*u, RGB_BLUE);
			blit_windows(-1);

			if (t == p_t_start) {
				_test_win[TEST_WIN_TEXT_1]->put_text((double)i, "i:::");
				i++;
			}

			// process iteration here
			//_test_ds[TEST_DS_OUT_GFX_1]->write_pxl(*u, RGB_GREY);
			//blit_windows(-1);
		}
	}

	void process_circuit(feature_bank* p_bank, tracer* p_circuit_start, rgb p_color)
	{
		tracer_unit first, last;
		tracer_unit prev(VECT_2D_INT_UNDEF_VALUE);
		int overlap_ct = 0;
		linear_tracer<tracer_unit> lt(1.01f);
		int prev_i;
		tracer* prev_tracer;
		std::list<tracer_unit>::iterator prev_corner, p;
		int i = 0;

		prev_i = 0;
		prev_tracer = p_circuit_start;
		prev_corner = p_circuit_start->data->begin();

		_test_win[TEST_WIN_TEXT_1]->put_text("begin");

		// iterate through tracers in circuit
		for (tracer* t = p_circuit_start; overlap_ct < OVERLAP_CT; t = (tracer*)t->next) {
			// iterate through all points in tracer
			for (std::list<tracer_unit>::iterator u = t->data->begin(); u != t->data->end() && overlap_ct < OVERLAP_CT; ++u) {				
				//_test_ds[TEST_DS_IN]->write_pxl(*u, RGB_WHITE);
				if (lt.update(*u)) {
					_test_ds[TEST_DS_OUT_GFX_1]->write_pxl(*p, RGB_RED);
					fine_process(prev_i, i, prev_tracer, p_circuit_start, prev_corner, u);
					prev_i = i;
					prev_tracer = t;
					prev_corner = u;
				}

				if (i == 1) {
					overlap_ct++;
					_test_win[TEST_WIN_TEXT_1]->put_text(double(overlap_ct), "enter process_cirtuit");
				}
				blit_windows(0);

				p = u;
			}

			// process connector here on the fly? 
			// or maybe queue them to process after outy?
			if (t->connects != NULL) {
			}

			if (t == p_circuit_start) {
				_test_win[TEST_WIN_TEXT_1]->put_text((double)i, "iteration");
				i++;
			}
		}
	}

	void process_unit (tracer_unit& curr)
	{
		_test_ds[TEST_DS_OUT_GFX_1]->write_pxl(curr, RGB_BLUE);
		blit_windows(0);

		//lt.update(curr);
	}
public:
	feature_bank perimeter;
	std::list<feature_bank*> voids;

	// shape is created when the perimeter is completed
	// and so the proto-shape is completed
	shape(tracer* p_outy)
	{
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
	std::list<shape*> shapes;
	std::list<row_segment*> connections; // temp buffer for segments connected above segment
public:
	void clear_connections()
	{
		connections.clear();
	}

	void add_connection(row_segment* seg)
	{
		connections.push_back(seg);
	}

	void trace(int lef_x, int rig_x, int y, tracer*& lef_trace, tracer*& rig_trace)
	{
		if (connections.size()) {
			lef_trace = connections.front()->lef_trace;
			lef_trace->store_lef(lef_x, y);

			std::list<row_segment*>::iterator i = connections.begin();
			while (true) {
				if ((*i)->last_connection != NULL) { // diverge case
					(*i)->last_connection->lef_trace->delete_lef();
					tracer_unit unit = (*i)->last_connection->rig_trace->delete_rig();
					(*i)->last_connection->rig_trace = lef_trace = new tracer();
					lef_trace->store_rig(unit.x, unit.y);
					lef_trace->store_lef(lef_x, y);
				}

				std::list<row_segment*>::iterator prev = i;
				++i;
				if (i == connections.end()) {
					break;
				}
				if ((*prev)->rig_trace->append((*i)->lef_trace)) {	// complete inny circuit
					connections.front()->lef_trace->add_inny((*i)->lef_trace);
				}
			}

			rig_trace = connections.back()->rig_trace;
			rig_trace->store_rig(rig_x, y);
		} else { // new tracer
			lef_trace = rig_trace = new tracer();
			lef_trace->store_lef(lef_x, y);
			lef_trace->store_rig(rig_x, y);
		}
	}

	void set_connections(row_segment* seg)
	{
		for (std::list<row_segment*>::iterator i = connections.begin(); i != connections.end(); ++i) {
			(*i)->last_connection = seg;
		}
	}

	void tie_ends(row_seg_set* segs)
	{
		for (row_seg_set::iterator i = segs->begin(); i != segs->end(); ++i) {
			if ((*i)->last_connection == NULL) {
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