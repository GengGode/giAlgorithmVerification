// OpencvConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#include "CodeTest.h"

#include <Windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "hook_listen_key.h"
#include "gi_test_calc.h"
#include "gi_test_diff_match.h"
#include "gi_test_uneven_light.h"
#include "gi_test_calc_angle.h"
#include "add_rgba_image.h"


int main()
{	
	//main_hook_listen_key();
	// test_calc();
	//test_diff_match();
	// test_uneven_light();
	//test_calc_angle();
	test_add_rgba_image();
	return 0;
}
