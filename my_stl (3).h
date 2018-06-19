#ifndef INC_MY_STL
#define INC_MY_STL

#define MY_STL_NOT_INIT 0
#define MY_STL_ERROR 1
#define MY_STL_VALID 2
#define MY_STL_AT_END 3
#define ROUND_ROBIN_ELEMENT_LOOP round_robin_element<int>(0, 1)
#define ROUND_ROBIN_ELEMENT_LOOP_REV round_robin_element<int>(0, -1)

#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\alt_null.h"
#include <vector>
#include <list>

template <class gen1>
class my_stl {
	int status;
public:
	gen1 iter;

	my_stl(int p_status = MY_STL_NOT_INIT)
	{
		status = p_status;
	}

	my_stl(gen1 p_iterator)
	{
		iter = p_iterator;
	}

	inline gen1 operator = (gen1 p_iterator)
	{
		return iter = p_iterator;
	}

	inline bool operator != (gen1 p_iterator)
	{
		return iter != p_iterator;
	}

	inline bool operator == (gen1 p_iterator)
	{
		return iter == p_iterator;
	}

	inline gen1 operator -> ()
	{
		return iter;
	}

	inline gen1 operator ++ ()
	{
		return ++iter;
	}

	inline gen1 operator -- ()
	{
		return --iter;
	}

	inline void set_status(int p_status)
	{
		status = p_status;
	}

	inline int get_status()
	{
		return status;
	}
};

template <class gen1>
class my_list {
public:
	class element {
		element* next;
		gen1 data;
	public:
		element(element* p_next = (element*)_alt_null.get_null(MY_STL_NOT_INIT))
		{
			next = p_next;
		}

		element(gen1 p_data, element* p_next = (element*)_alt_null.get_null(MY_STL_NOT_INIT))
		{
			data = p_data;
			next = p_next;
		}

		element* get_next()
		{
			return next;
		}

		void set_next(element* p_next = _alt_null.get_null(MY_STL_NOT_INIT))
		{
			next = p_next;
		}

		gen1 get_data()
		{
			return data;
		}

		gen1* get_data_ptr()
		{
			return &data;
		}
	};

	class iterator {
		element* el;

		void set_null(int null_n = MY_STL_NOT_INIT)
		{
			el = (element*)_alt_null.get_null(null_n);
		}

	public:
		iterator ()
		{
			set_null();
		}

		iterator (element* p_el)
		{
			el = p_el;
		}

		bool is_valid()
		{
			return el != (element*)_alt_null.get_null(MY_STL_NOT_INIT);
		}

		inline void invalidate()
		{
			set_null();
		}

		void operator ++ ()
		{
			el = el->get_next();
		}

		inline gen1 operator * ()
		{
			return el->get_data();
		}

		inline gen1* operator -> ()
		{
			return &el->get_data();
		}

		inline bool operator == (iterator p_it)
		{
			return p_it.el == el;
		}

		inline bool operator != (iterator p_it)
		{
			return p_it.el != el;
		}
	};

private:
	element* start;
	element* finish;
	int el_ct;

	void init()
	{
		start = finish = (element*)_alt_null.get_null(MY_STL_AT_END);
		el_ct = 0;
	}

public:
	void clear()
	{
		element* curr = start;
		while(curr != (element*)_alt_null.get_null(MY_STL_AT_END)) {
			element* next = curr->get_next();
			delete curr;
			curr = next;
		};
		init();
	}

	my_list()
	{
		init();
	}

	~my_list()
	{
		clear();
	}

	gen1* push_back(gen1 p_data)
	{
		element* curr = new element(p_data, (element*)_alt_null.get_null(MY_STL_AT_END));

		if (finish == (element*)_alt_null.get_null(MY_STL_AT_END)) {
			start = curr;
		} else {
			finish->set_next(curr);
		}

		el_ct++;
		finish = curr;
		return (*curr).get_data_ptr();
	}

	iterator begin()
	{
		return start;
	}

	iterator end()
	{
		return iterator((element*)_alt_null.get_null(MY_STL_AT_END));
	}

	int size()
	{
		return el_ct;
	}
};

/*
Primises of this class:
- Has no central object, but are all joined elements
- Optimized to add elements to the beginning or end, but not insert elements.
- Order of all methods should be constant and independent of the number of elements. O(c) not O(n).
*/
template <class gen1>
class appendable_element {
public:
	//appendable_element* begin;
	//appendable_element* end;
	appendable_element* next;
	appendable_element* other_end;
	gen1 data;
public:
	class iterator {
		appendable_element* ptr;
	public:
		void operator ++ ()
		{
			ptr = ptr->next;
		}

		gen1 operator * ()
		{
			return data;
		}

		gen1* operator -> ()
		{
			return &data;
		}
	};

	appendable_element ()
	{
		//end = begin = this;
		other_end = this;
		next = at_end();
	}

	appendable_element (gen1 p_data)
	{
		data = p_data;
		//end = begin = this;
		other_end = this;
		next = at_end();
	}

	inline appendable_element* get_next ()
	{
		return next;
	}

	inline bool append (appendable_element* p_tail)
	{
		next = p_tail;
		
		if (is_connected(p_tail)) { // joining to itself, round robin
			//p_tail->other_end->other_end = 
			next = (appendable_element*)_alt_null.get_null(ALT_NULL_NA);
			return true;
		} else { // not round robin
			appendable_element* other_end_bak = other_end;
			other_end->other_end = p_tail->other_end;
			p_tail->other_end->other_end = other_end_bak;
			return false;
		}
	}

	inline bool prepend (appendable_element* p_begin)
	{
		p_begin->next = begin;

		if (is_connected(p_begin)) { // joining to itself, round robin
			return true;
		} else { // not round robin
			begin = p_begin->begin;
			return false;
		}
	}

	// if is connected or is same list, return true, otherwise false
	inline bool is_connected (appendable_element* p_ae)
	{
		return other_end == p_ae;
	}

	inline static appendable_element* at_end()
	{
		return (appendable_element*)_alt_null.get_null(ALT_NULL_NOT_INIT);
	}

	inline gen1* data_ptr()
	{
		return &data;
	}
};


template <class iter_type = int>
class round_robin_element {

	static char outp[50];

public:

	int lap;
	iter_type i;

	inline void init(iter_type p_i = 0, int p_lap = 0)
	{
		lap = p_lap;
		i = p_i;
	}

	inline round_robin_element()
	{
	}

	inline round_robin_element(iter_type p_i, int p_lap = 0)
	{
		init(p_i, p_lap);
	}

	inline round_robin_element(iter_type p_i, int p_lap, int p_size)
	{
		init(p_i, p_lap);
		resolve(p_size);
	}

	// constructor for end point of for-loop
	inline round_robin_element(iter_type p_begin, iter_type p_i, bool forward)
	{
		if (forward) { // forward
			if (p_begin <= p_i) {
				init(p_i, 0);
			} else {
				init(p_i, 1);
			}
		} else { // reverse
			if (p_begin >= p_i) {
				init(p_i, 0);
			} else {
				init(p_i, -1);
			}
		}
	}

	inline int effective(int p_size)
	{
		if (p_size > 0) {
			return i + (lap * p_size);
		} else {
			return i;
		}
	}

	inline void resolve_long(int p_size)
	{
		if (p_size > 0) {
			int ef = effective(p_size);
			i = ef % p_size;
			lap = ef / p_size;
		}
	}

	inline round_robin_element resolve(int p_size)
	{
		if (p_size > 0) {
			if (i < 0) {
				i += p_size;
				lap--;
			} else if (i >= p_size) {
				i -= p_size;
				lap++;
			}
		}

		return *this;
	}

	inline void resolve_pos(int p_size)
	{
		if (p_size > 0) {
			if (i >= p_size) {
				i -= p_size;
				lap++;
			}
		}
	}

	inline void resolve_neg(int p_size)
	{
		if (p_size > 0) {
			if (i < 0) {
				i += p_size;
				lap--;
			}
		}
	}

	inline bool operator == (round_robin_element rr)
	{
		return (rr.lap == lap) && (rr.i == i);
	}

	inline bool operator != (round_robin_element rr)
	{
		return (rr.lap != lap) || (rr.i != i);
	}

	inline bool operator < (round_robin_element rr)
	{
		if (lap == rr.lap) {
			return i < rr.i;
		} else  {
			return lap < rr.lap;
		}
	}

	inline bool operator <= (round_robin_element rr)
	{
		if (lap == rr.lap) {
			return i <= rr.i;
		} else  {
			return lap < rr.lap;
		}
	}

	inline bool operator > (round_robin_element rr)
	{
		if (lap == rr.lap) {
			return i > rr.i;
		} else {
			return lap > rr.lap;
		}
	}

	inline bool operator >= (round_robin_element rr)
	{
		if (lap == rr.lap) {
			return i >= rr.i;
		} else {
			return lap > rr.lap;
		}
	}

	inline round_robin_element inc(int p_size)
	{
		i++;
		resolve_pos(p_size);
		return round_robin_element(i, lap);
	}

	template <class gen>
	inline round_robin_element inc(int p_size, gen p_inc)
	{
		i += p_inc;
		resolve_pos(p_size);
		return round_robin_element(i, lap);
	}

	inline round_robin_element dec(int p_size)
	{
		i--;
		resolve_neg(p_size);
		return round_robin_element(i, lap);
	}

	template <class gen>
	inline round_robin_element dec(int p_size, gen p_dec)
	{
		i -= p_dec;
		resolve_neg(p_size);
		return round_robin_element(i, lap);
	}

	inline round_robin_element operator + (round_robin_element e)
	{
		return round_robin_element(i + e.i, lap + e.lap);
	}

	inline round_robin_element operator - (round_robin_element e)
	{
		return round_robin_element(i - e.i, lap - e.lap);
	}

	inline round_robin_element add(iter_type p_n, int p_size = -1)
	{
		return round_robin_element(i + p_n, lap).resolve(p_size);
	}

	inline round_robin_element add(round_robin_element e, int p_size = -1)
	{
		return (*this + e).resolve(p_size);
	}

	inline round_robin_element sub(iter_type p_n, int p_size = -1)
	{
		return round_robin_element(i - p_n, lap).resolve(p_size);
	}

	inline round_robin_element sub(round_robin_element e, int p_size = -1)
	{
		return (*this - e).resolve(p_size);
	}

	inline round_robin_element operator * (iter_type n)
	{
		return round_robin_element(i * n, lap * n);
	}

	template <class gen>
	inline round_robin_element mult(gen n, int p_size = -1)
	{
		return round_robin_element(int((gen)effective(p_size) * n)).resolve(p_size);
	}

	inline char* report()
	{
		sprintf(outp, "lap=%i, i=%i", lap, i);
		return outp;
	}
};
template <class iter_type>
char round_robin_element<iter_type>::outp[50];

template <class data_type, class containter_type = std::vector<data_type>, class iter_type = int>
class round_robin_iterator {
	
	static char outp[70];

public:
	round_robin_element<iter_type> e;
	round_robin_element<iter_type> start;
	containter_type* vect;
	int size;

	round_robin_iterator()
	{
	}

	round_robin_iterator(iter_type p_i)
	{
		e = round_robin_element<iter_type>(p_i, 0);
		start = e;
	}

	round_robin_iterator(round_robin_element<iter_type> p_e)
	{
		e = p_e;
		start = e;
	}

	round_robin_iterator(round_robin_element<iter_type> p_e, int p_size)
	{
		e = p_e;
		start = e;
		size = p_size;
	}

	/*
	round_robin_iterator(iter_type p_i, container_type& p_vect)
	{
		e = p_i;
		start = e;
		vect = &p_vect;
		size = vect->size();
	}
	*/

	round_robin_iterator(round_robin_element<iter_type> p_e, containter_type& p_vect)
	{
		e = p_e;
		start = e;
		vect = &p_vect;
		size = vect->size();
	}

	inline iter_type iteration()
	{
		return e.i;
	}

	inline bool cont()
	{
		return e < ROUND_ROBIN_ELEMENT_LOOP;
	}

	template <class gen>
	inline round_robin_iterator operator = (gen p_i)
	{
		e = p_i;
		start = e;
		return e;
	}

	/*
	template <class data_type>
	inline round_robin_iterator(round_robin_element<iter_type> p_e, containter_type vect)
	{
		e = p_e;
		start = e;
		size = vect.size();
		data = &vect;
	}
	*/

	inline round_robin_iterator operator ++ ()
	{
		return e.inc(size);
	}

	inline round_robin_iterator operator -- ()
	{
		return e.dec(size);
	}

	inline round_robin_iterator operator += (iter_type p_n)
	{
		return round_robin_iterator(e.inc(size, p_n), *vect);
	}

	inline round_robin_iterator operator -= (iter_type p_n)
	{
		return round_robin_iterator(e.dec(size, p_n), *vect);
	}

	inline round_robin_iterator operator + (round_robin_iterator p_i)
	{
		return round_robin_iterator(e.add(p_i.e, size), *vect);
	}

	inline round_robin_iterator operator + (iter_type p_n)
	{
		return round_robin_iterator(e.add(p_n, size), *vect);
	}

	inline round_robin_iterator operator - (round_robin_iterator p_i)
	{
		return round_robin_iterator(e.sub(p_i.e, size), *vect);
	}

	inline round_robin_iterator operator - (iter_type p_n)
	{
		return round_robin_iterator(e.sub(p_n, size), *vect);
	}

	template <class gen>
	inline round_robin_iterator operator * (gen p_n)
	{
		return round_robin_iterator(e.mult(p_n, size), *vect);
	}

	inline bool& operator == (round_robin_iterator p_i)
	{
		return e == p_i.e;
	}

	inline bool operator != (round_robin_iterator p_i)
	{
		return e != p_i.e;
	}

	inline bool operator < (round_robin_iterator p_i)
	{
		return e < p_i.e;
	}

	inline bool operator <= (round_robin_iterator p_i)
	{
		return e <= p_i.e;
	}

	inline bool operator > (round_robin_iterator p_i)
	{
		return e > p_i.e;
	}

	inline bool& operator >= (round_robin_iterator p_i)
	{
		return e >= p_i.e;
	}

	inline bool& cont_fwd_inc(iter_type p_start, iter_type p_i)
	{
		return e <= round_robin_element<iter_type>(start, p_i, true);
	}

	inline bool& cont_fwd_exc(iter_type p_start, iter_type p_i)
	{
		return e < round_robin_element<iter_type>(start, p_i, true);
	}

	inline data_type get_element(iter_type p_n)
	{
		round_robin_element<iter_type> e(p_n);
		return (*vect)[e.resolve(size).i];
	}

	inline data_type operator * ()
	{
		return (*vect)[e.i];
	}

	inline data_type* operator -> ()
	{
		return &(*vect)[e.i];
	}

	inline char* report()
	{
		sprintf(outp, "lap=%i, i=%i, size=%i", e.lap, e.i, size);
		return outp;
	}
};
template <class data_type, class container_type, class iter_type>
char round_robin_iterator<data_type, container_type, iter_type>::outp[70];

/*
template <class iter_type = int, class data_type = int, containter_type>
class round_robin_vect : public round_robin_iterator<int> {
	


public:

	inline round_robin_vect (round_robin_iterator<int> p_rr)
	{
		*this = p_rr;
	}

	inline round_robin_vect (data_type p_i, std::vector<data_type>& p_vect)
	{
		e = p_i;
		vect = &p_vect;
		size = p_vect.size();
	}

	inline data_type operator * ()
	{
		return (*vect)[e.i];
	}

	inline data_type* operator -> ()
	{
		return &vect[e.i];
	}
	
};
*/

/*
template <class iter_type = int>
class round_robin {

	void init(int p_size)
	{
		len = p_size;
	}

public:

	iter_type i;
	int len;
	int lap;

	round_robin(int p_size, iter_type p_start = 0, int p_lap = 0)
	{
		init(p_size);
		i = p_start;
		lap = p_lap;
	}

	void iterate()
	{
		i++;
		
		if (i >= len) {
			i = 0;
			lap++;
		}
	}

	void iterate_rev()
	{
		i--;

		if (i < 0) {
			i = len - 1;
			lap--;
		}
	}

	// returns equivalent iterator between 0 and len - 1
	// only works for period(or lap) between -1 and +1 inclusively
	inline iter_type resolve(iter_type p_i)
	{
		if (p_i < 0) {
			return p_i + len;
		} else if (p_i >= len) {
			return p_i - len;
		} else {
			return p_i;
		}
	}

	inline bool cont(int p_lap, 
	inline iter_type iter_start()
	{

	}
};
*/

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


#endif // !INC_MY_STL