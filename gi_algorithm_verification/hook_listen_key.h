#pragma once

#include <Windows.h>
#include <iostream> 
#include <format> 

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
			// 键盘相关
		case WM_KEYDOWN: {
			std::cout << std::format("WM_KEYDOWN: {}\n", p->vkCode);
		} break;
		case WM_SYSKEYDOWN: {
			std::cout << std::format("WM_SYSKEYDOWN: {}\n", p->vkCode);
		} break;
		case WM_KEYUP: {
			std::cout << std::format("WM_KEYUP: {}\n", p->vkCode);
		} break;
		case WM_SYSKEYUP: {
			std::cout << std::format("WM_SYSKEYUP: {}\n", p->vkCode);
		} break;
		}
	}
#if 0
	return 1;
#else
	return CallNextHookEx(NULL, nCode, wParam, lParam);
#endif
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
			// 鼠标相关
		case WM_LBUTTONDOWN: {
			std::cout << std::format("WM_LBUTTONDOWN: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_LBUTTONUP: {
			std::cout << std::format("WM_LBUTTONUP: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_RBUTTONDOWN: {
			std::cout << std::format("WM_RBUTTONDOWN: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_RBUTTONUP: {
			std::cout << std::format("WM_RBUTTONUP: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_MBUTTONDOWN: {
			std::cout << std::format("WM_MBUTTONDOWN: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_MBUTTONUP: {
			std::cout << std::format("WM_MBUTTONUP: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_MOUSEWHEEL: {
			std::cout << std::format("WM_MOUSEWHEEL: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_MOUSEHWHEEL: {
			std::cout << std::format("WM_MOUSEHWHEEL: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		case WM_MOUSEMOVE: {
			std::cout << std::format("WM_MOUSEMOVE: {}, {} state:{}\n", p->pt.x, p->pt.y, p->flags);
		} break;
		}
	}
#if 0
	return 1;
#else
	return CallNextHookEx(NULL, nCode, wParam, lParam);
#endif
}


int main1()
{
	//return main_screen();

	//cv::Mat mask = create_square_mask(100, 100, 20);
	//cv::imshow("mask", mask);
	//cv::waitKey(0); 

	HHOOK hhkLowLevelKybd = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
	HHOOK hhkLowLevelMous = SetWindowsHookExA(WH_MOUSE_LL, MouseProc, 0, 0);

	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);
	UnhookWindowsHookEx(hhkLowLevelMous);

	return 0;
}