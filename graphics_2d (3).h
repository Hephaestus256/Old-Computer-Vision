#ifndef INC_GRAPHICS_2D
#define INC_GRAPHICS_2D

#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\math_2d.h"
#include "stdafx.h"
#include <windows.h>

LPTSTR char_to_lptstr(const char* in_text)
{
	LPTSTR str = new TCHAR[strlen(in_text) + 1];
	MultiByteToWideChar(0, 0, in_text, -1, str, strlen(in_text) + 1);
	return str;
}

void mess(int num, char* text = NULL)
{
	char in_text[1000];

	if (text == NULL) {
		text = "value = ";
	}

	sprintf_s(in_text, "%s %i", text, num);
	LPTSTR str = char_to_lptstr(in_text);
	MessageBox(NULL, str, L"Info", MB_OK);

	delete [] str;
}

void mess(char* text1, char* text2 = NULL)
{
	char in_text[1000];

	if (text2 == NULL) {
		text2 = "";
	}

	sprintf_s(in_text, "%s %s", text2, text1);
	LPTSTR str = char_to_lptstr(in_text);
	MessageBox(NULL, str, L"Info", MB_OK);

	delete [] str;
}

template <class gen>
void mess(gen num, char* text = NULL)
{
	char in_text[1000];

	if (text == NULL) {
		text = "value = ";
	}

	sprintf_s(in_text, "%s %f", text, (float)num);
	LPTSTR str = char_to_lptstr(in_text);
	MessageBox(NULL, str, L"Info", MB_OK);

	delete [] str;
}

/*
		hdc = BeginPaint(hWnd, &ps);

		//BITMAP bm;
		//GetObject(bmp, sizeof(bm), &bm);

		bmp_hdc = CreateCompatibleDC(hdc);
		//bmp_hdc2 = CreateCompatibleDC(hdc);
		//HDC bmp_hdc3 = CreateCompatibleDC(hdc);

		//HBITMAP old_bmp = (HBITMAP)SelectObject(bmp_hdc, bmp);
		//HBITMAP old_bmp2 = (HBITMAP)SelectObject(bmp_hdc2, bmp2);

		//RECT Rect;
		//GetWindowRect(hWnd, &Rect); // grab window size

		BITMAPINFO bi24BitInfo; // We set this up to grab what we want
		bi24BitInfo.bmiHeader.biBitCount = 24; // rgb 8 bytes for each component(3)
		bi24BitInfo.bmiHeader.biCompression = BI_RGB;// rgb = 3 components
		bi24BitInfo.bmiHeader.biPlanes = 1;
		bi24BitInfo.bmiHeader.biSize = sizeof(bi24BitInfo.bmiHeader); // size of this struct
		bi24BitInfo.bmiHeader.biWidth = 100; // width of window
		bi24BitInfo.bmiHeader.biHeight = 100; // height of window

		void* dat = NULL;
		HBITMAP sect = CreateDIBSection(bmp_hdc, &bi24BitInfo, DIB_RGB_COLORS, &dat, NULL, 0);
		HBITMAP old_bmp3 = (HBITMAP)SelectObject(bmp_hdc, sect);

		BYTE* b = (BYTE*)dat;

		for (int i = 0; i < 1000; i++) {
			b[i] = 128;
		}

		//CHAR s[100];
		//printf(s, "%i", (int)GetLastError());
		//MessageBox(NULL, s, NULL, NULL);

		//TextOut(hdc, x, 0, L"Hello World", 11);
		//TextOut(hdc, 15, 15, L"Goodby", 6);
		
		DeleteObject(sect);

		for (int i = 0; i < 1000; i++) {
			b[i] = 255;
		}

		for (int i = 0; i < 1000; i++) {
			b[i] = (i % 3) * 100;
		}

		BitBlt(hdc, 100, 100 + x, 100, 100, bmp_hdc, 0, 0, SRCCOPY);

		//GetObject(sect, sizeof(bm), &bm);
		//BitBlt(hdc, 200, 100 + x, bm.bmWidth, bm.bmHeight, bmp_hdc3, 0, 0, SRCCOPY);

		//SelectObject(bmp_hdc, old_bmp);
		DeleteDC(bmp_hdc);

		//SelectObject(bmp_hdc2, old_bmp2);
		//DeleteDC(bmp_hdc2);

		EndPaint(hWnd, &ps);

*/

#define RGB_BLACK rgb(0, 0, 0)
#define RGB_WHITE rgb(255, 255, 255)
#define RGB_GREY rgb(127, 127, 127)
#define RGB_RED rgb(255, 0, 0)
#define RGB_GREEN rgb(0, 255, 0)
#define RGB_BLUE rgb(0, 0, 255)
#define RGB_YELLOW rgb(255, 255, 0)
#define RGB_AQUA rgb(0, 255, 255)
#define RGB_PURPLE rgb(255, 0, 255)
#define RGB_ORANGE rgb(255, 165, 0)

class rgb {
public:
	union {
		struct {
			BYTE r, g, b;
		};
		BYTE primary[3];
	};

	rgb()
	{
	}

	rgb(int p_r, int p_g, int p_b)
	{
		r = p_r;
		g = p_g;
		b = p_b;
	}

	bool operator == (rgb c)
	{
		return r == c.r && g == c.g && b == c.b;
	}

	bool operator != (rgb c)
	{
		return r != c.r || g != c.g || b != c.b;
	}

	rgb reverse()
	{
		return rgb(b, g, r);
	}

	/*
	RGB cv_win()
	{
		return RGB(r, g, b);
	}
	*/
};

class gfx_surface {
protected:
	HDC dest_hdc;
	HBITMAP pixels;
	HBITMAP old_bmp;
	quantum_2d dims;
	HPEN pen;
	HBRUSH brush;
public:
	HDC hdc;

	gfx_surface(HDC p_dest_hdc = NULL)
	{
		hdc = CreateCompatibleDC(p_dest_hdc);
		dest_hdc = p_dest_hdc;
		pixels = NULL;
		old_bmp = NULL;
		pen = NULL;
		brush = NULL;
	}

	~gfx_surface()
	{
		if (hdc != NULL) {
			SelectObject(hdc, old_bmp);
			DeleteDC(hdc);
		}

		if (pixels != NULL) {
			DeleteObject(pixels);
		}

		if (pen != NULL) {
			DeleteObject(pen);
		}

		if (brush != NULL) {
			DeleteObject(brush);
		}
	}

	bool init_surface(int width, int height, HBITMAP p_bmp = NULL)
	{
		pixels = p_bmp;
		old_bmp = (HBITMAP)SelectObject(hdc, p_bmp);
		dims = quantum_2d(width, height);

		return (old_bmp != NULL);
	}

	inline quantum_2d get_dims()
	{
		return dims;
	}

	bool blit(quantum_2d p_xy, HDC p_dest_hdc = NULL, quantum_2d p_dims = QUANTUM_2D_UNDEF_VALUE, quantum_2d p_uv = QUANTUM_2D_UNDEF_VALUE)
	{
		if (p_dest_hdc == NULL) {
			p_dest_hdc = dest_hdc;
		}

		//if (is_undef_value(p_dims)) {
			p_dims = dims;
		//}

		//if (is_undef_value(p_uv)) {
			p_uv = quantum_2d(0, 0);
		//}

		return BitBlt(p_dest_hdc, p_xy.x, p_xy.y, p_dims.x, p_dims.y, hdc, p_uv.x, p_uv.y, SRCCOPY) != FALSE;
	}

	void set_color(rgb color, bool is_solid = true)
	{
		static rgb last_color(RGB_BLACK);
		static rgb last_brush(RGB_BLACK);
		int pen_mode;

		if (is_solid) {
			pen_mode = PS_NULL;
		} else {
			pen_mode = PS_SOLID;
		}

		pen = CreatePen(pen_mode, 1, RGB(color.r, color.g, color.b));
		SelectObject(hdc, pen);
		last_color = color;
		
		if (is_solid) {
			if (last_brush != color) {
				brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
				SelectObject(hdc, brush);
				last_brush = color;
			}
		}
	}

	void circle(quantum_2d center, int radius, rgb color, bool solid = true)
	{
		set_color(color, solid);
		Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
	}

	void elipse(quantum_2d center, int x_radius, int y_radius, rgb color, bool solid = true)
	{
		set_color(color, solid);
		Ellipse(hdc, center.x - x_radius, center.y - y_radius, center.x + x_radius, center.y + y_radius);
	}

	void rectangle(quantum_2d tl, quantum_2d br, rgb color, bool solid = true)
	{
		set_color(color, solid);
		Rectangle(hdc, tl.x, tl.y, br.x + 2, br.y + 2);
	}

	void poly(quantum_2d* verts, int ct, rgb color, bool solid = true)
	{
		POINT* p_verts = new POINT[ct];

		for (int i = 0; i < ct; i++) {
			p_verts[i].x = verts[i].x;
			p_verts[i].y = verts[i].y;
		}

		set_color(color, solid);
		Polygon(hdc, p_verts, ct);
	}

	void triangle(quantum_2d p1, quantum_2d p2, quantum_2d p3, rgb color, bool solid = true)
	{
		quantum_2d verts[3] = {p3, p2, p1};
		poly(verts, 3, color, solid);
	}

	void test_box(quantum_2d center, rgb color = RGB_WHITE, int radius = 0)
	{
		int offset = radius + 1;
		//rectangle(center - quantum_2d(offset, offset), center + quantum_2d(offset, offset), color, false);
	}
};

class bitmap : public gfx_surface {
	BITMAP bm;
public:
	bitmap(LPCWSTR p_file_name, HDC p_dest_hdc = NULL) : gfx_surface(p_dest_hdc)
	{
		HBITMAP hbmp = (HBITMAP)LoadImage(NULL, p_file_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hbmp == NULL) {
			wchar_t tmp[50];
			swprintf(tmp, L"BMP load failed - error %i", 50, ::GetLastError());
			MessageBox(NULL, p_file_name, NULL, NULL);
			MessageBox(NULL, LPCWSTR(tmp), NULL, NULL);
		}

		GetObject(hbmp, sizeof(bm), &bm);
		init_surface(bm.bmWidth, bm.bmHeight, hbmp);
	}
	
	~bitmap()
	{
	}
};

class draw_surface : public gfx_surface {
protected:
	void* direct_dat;
public:
	draw_surface(int p_width, int p_height, HDC p_dest_hdc = NULL) : gfx_surface(p_dest_hdc)
	{
		BITMAPINFO bmp_info;
		bmp_info.bmiHeader.biBitCount = 24;
		bmp_info.bmiHeader.biCompression = BI_RGB;
		bmp_info.bmiHeader.biPlanes = 1;
		bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
		bmp_info.bmiHeader.biWidth = p_width;
		bmp_info.bmiHeader.biHeight = p_height;
		
		init_surface(p_width, p_height, CreateDIBSection(hdc, &bmp_info, DIB_RGB_COLORS, &direct_dat, NULL, 0));
	}

	draw_surface(HDC p_dest_hdc = NULL) : gfx_surface(p_dest_hdc)
	{
		direct_dat = NULL;
	}

	draw_surface(LPCWSTR p_file_name)
	{
		bitmap bmp(p_file_name);

		BITMAPINFO bmp_info;
		bmp_info.bmiHeader.biBitCount = 24;
		bmp_info.bmiHeader.biCompression = BI_RGB;
		bmp_info.bmiHeader.biPlanes = 1;
		bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
		bmp_info.bmiHeader.biWidth = bmp.get_dims().x;
		bmp_info.bmiHeader.biHeight = bmp.get_dims().y;
		
		init_surface(bmp_info.bmiHeader.biWidth, bmp_info.bmiHeader.biHeight, CreateDIBSection(hdc, &bmp_info, DIB_RGB_COLORS, &direct_dat, NULL, 0));
		bmp.blit(quantum_2d(0, 0), hdc);
	}

	~draw_surface()
	{		
	}

	void* get_direct_ptr()
	{
		return direct_dat;
	}

	inline rgb read_pxl(int offset)
	{
		rgb* start = (rgb*)get_direct_ptr();
		rgb ret = *(start + offset);
		return ret.reverse();
	}

	inline void write_pxl(int offset, rgb color)
	{
		rgb* start = (rgb*)get_direct_ptr();
		*(start + offset) = color.reverse();
	}

	inline rgb read_pxl(int x, int y)
	{
		quantum_2d dims = get_dims();
		return read_pxl(x + dims.x * (dims.y - 1 - y));
	}

	inline rgb read_pxl_safe(int x, int y)
	{
		static quantum_2d dims = get_dims();

		if ((x >= 0) && (x < dims.x) && (y >= 0) && (y < dims.y)) {
			return read_pxl(x + dims.x * (dims.y - 1 - y));
		} else {
			return rgb(-1, -1, -1);
		}
	}
	
	inline void write_pxl(int x, int y, rgb color)
	{
		static quantum_2d dims = get_dims();
		write_pxl(x + dims.x * (dims.y - 1 - y), color);
	}

	inline void write_pxl_safe(int x, int y, rgb color)
	{
		static quantum_2d dims = get_dims();

		if ((x >= 0) && (x < dims.x) && (y >= 0) && (y < dims.y)) {
			write_pxl(x + dims.x * (dims.y - 1 - y), color);
		}
	}

	template <class gen1>
	inline void write_pxl(vect_2d<gen1> p, rgb color)
	{
		write_pxl((int)p.x, (int)p.y, color);
	}

	inline void alpha_blend(int x, int y, float alpha = 0.5, rgb color = RGB_BLACK)
	{
		rgb exist = read_pxl(x, y);
		write_pxl(x, y, rgb(interpolate(alpha, exist.r, color.r), interpolate(alpha, exist.g, color.g), interpolate(alpha, exist.b, color.b)));
	}

	inline void alpha_blend_safe(int x, int y, float alpha = 0.5, rgb color = RGB_BLACK)
	{
		rgb exist = read_pxl_safe(x, y);
		write_pxl_safe(x, y, rgb(interpolate(alpha, exist.r, color.r), interpolate(alpha, exist.g, color.g), interpolate(alpha, exist.b, color.b)));
	}

	inline void line_seg(int x1, int y1, int x2, int y2, rgb color)
	{
		double dx = double(x2 - x1);
		double dy = double(y2 - y1);

		if (dx > dy) {
			double y = y1;
			double m = dy / dx;
			for (int x = x1; x <= x2; x++) {
				write_pxl(x, (int)floor(y), color);
				y += m;
			}
		} else {
			double x = x1;
			double m = dx / dy;
			for (int y = y1; y <= y2; y++) {
				write_pxl((int)x, y, color);
				x += m;
			}
		}
	}
};

class sprite {
public:
	gfx_surface* surf;
	quantum_2d pos;

	sprite(gfx_surface* p_surf, quantum_2d p_pos)
	{
		surf = p_surf;
		pos = p_pos;
	}

	~sprite()
	{
	}

	gfx_surface* get_bmp()
	{
		return surf;
	}

	bool blit(HDC p_dest_hdc)
	{
		return surf->blit(pos, p_dest_hdc);
	}
};

#endif // !INC_GRAPHICS_2D