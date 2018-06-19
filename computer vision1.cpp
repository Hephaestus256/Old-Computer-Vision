#include "stdafx.h"
#include "computer vision1.h"
#include <stdio.h>
#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\win_32.h"
#include "C:\Users\Nathan\Documents\Visual Studio 2010\Libraries\computer_vision.h"
#include <vector>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	draw_surface ds1(1000, 500);
	draw_surface ds2(1000, 500);
	_test_ds[TEST_DS_IN] = &ds1;
	_test_ds[TEST_DS_OUT_GFX_1] = &ds2;

	window w1 = window(L"Input Window", hInstance, nCmdShow, IDC_COMPUTERVISION1);
	window w2 = window(L"Output Window", hInstance, nCmdShow, IDC_COMPUTERVISION1);
	window w3 = window(L"Text Window", hInstance, nCmdShow, IDC_COMPUTERVISION1, 0.5, 0.5, 1.0, 1.0);
	_test_win[TEST_WIN_TEXT_1] = &w3;

	bitmap b(LPCWSTR(L"C:\\Users\\Nathan\\Pictures\\divconv.bmp"));
	b.blit(quantum_2d(10,10), ds1.hdc);

	w1.blit(&ds1, quantum_2d(100, 100));
	w2.blit(&ds2, quantum_2d(100, 100));

	//ds1.circle(quantum_2d(342, 300), 115,  RGB_GREEN);
	//ds1.circle(quantum_2d(-1042, 300), 1390,  RGB_GREEN);
	//ds1.circle(quantum_2d(351, 160), 151,  RGB_GREEN);
	//ds1.rectangle(quantum_2d(200, 160), quantum_2d(500, 460), RGB_GREEN);
	//ds1.rectangle(quantum_2d(200, 150), quantum_2d(300, 250), RGB_BLUE);
	//ds1.rectangle(quantum_2d(210, 160), quantum_2d(270, 180), RGB_BLACK);
	//ds1.rectangle(quantum_2d(260, 160), quantum_2d(270, 230), RGB_BLACK);
	//ds1.rectangle(quantum_2d(240, 190), quantum_2d(245, 200), RGB_BLACK);
	//ds1.rectangle(quantum_2d(240, 190), quantum_2d(245, 200), RGB_RED);
	//ds1.rectangle(quantum_2d(210, 160), quantum_2d(230, 230), RGB_BLACK);
	//ds1.rectangle(quantum_2d(200, 150), quantum_2d(300, 250), RGB_RED);
	//ds1.triangle(quantum_2d(223 - 200, 214 - 25), quantum_2d(457 - 249, 209), quantum_2d(415 - 200, 665), RGB_GREEN);
	//this one: 
	//ds1.triangle(quantum_2d(223 - 200, 314 - 25), quantum_2d(457 - 209, 409), quantum_2d(515 - 200, 265), RGB_GREEN);
	//ds1.line_seg(100, 100, 100, 200, RGB_WHITE);

	group_filter(&ds1, &ds2);

	w1.repaint();
	w2.repaint();
	
	while(window::get_message() && !w1.user_closed) {
	}

	return 0;
}
