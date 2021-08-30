// OpencvConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#include "CodeTest.h"

#include <Windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

LPCWSTR giWindowName = { L"原神" };
HWND giHandle;
RECT giRect;
RECT giClientRect;

cv::Size giClientSize;
cv::Mat giFrame;

double screen_scale = 1.0;

bool getGengshinImpactScale()
{
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// 获取监视器逻辑宽度与高度
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// 获取监视器物理宽度与高度
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	screen_scale = static_cast<double>(cxPhysical) / static_cast<double>(cxLogical);

	return true;
}

bool getGengshinImpactScreen()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)
	{
		return false;
	}
	if (!IsWindow(giHandle))
	{
		return false;
	}

	//获取目标句柄的窗口大小RECT
	GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = GetDC(giHandle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = static_cast<int>((screen_scale) * (giRect.right - giRect.left));
	int	nHeight = static_cast<int>((screen_scale) * (giRect.bottom - giRect.top));

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	//这里获取位图的大小信息,事实上也是兼容DC绘图输出的范围
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat操作
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, giFrame.data);

	giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));

	if (giFrame.empty())
	{
		return false;
	}
	return true;
}

int main()
{
	giWindowName = L"\u539F\u795E";
	giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */

	if (giHandle == NULL)
	{
		return 1;
	}

	if (!GetWindowRect(giHandle, &giRect))
	{
		return 2;
	}

	if (!GetClientRect(giHandle, &giClientRect))
	{
		return 3;
	}

	//获取屏幕缩放比例
	getGengshinImpactScale();

	giClientSize.width = static_cast<int>(screen_scale * (giClientRect.right - giClientRect.left));
	giClientSize.height = static_cast<int>(screen_scale * (giClientRect.bottom - giClientRect.top));

	//namedWindow("Genshin R", WINDOW_FREERATIO);
	//namedWindow("Genshin G", WINDOW_FREERATIO);
	//namedWindow("Genshin B", WINDOW_FREERATIO);
	//namedWindow("Genshin A", WINDOW_FREERATIO);
	//namedWindow("Genshin Gray", WINDOW_FREERATIO);

	std::vector<cv::Mat> lis;
	std::vector<cv::Mat> lisRectMat;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::Mat gray;
	int i = 0;
//#define b
#ifdef b
	while (getGengshinImpactScreen())
	{
#else
	while (1)
	{
		giFrame = imread("img.png", -1);
#endif
#ifdef a
		//giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 5.0*3.0), static_cast<int>(giFrame.rows / 4.0), static_cast<int>(giFrame.cols / 5.0), static_cast<int>(giFrame.rows / 2.0)));
		giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 5.0*3.0), static_cast<int>(giFrame.rows / 4.0), static_cast<int>(giFrame.cols / 6.0), static_cast<int>(giFrame.rows/2.0)));

		cv::split(giFrame, lis);

		cv::threshold(lis[3], gray, 100, 255, cv::THRESH_BINARY);

		//cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
		//cv::erode(gray, gray, erode_element);
		//cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
		//cv::dilate(gray, gray, dilate_element);

		cv::findContours(gray, contours, hierarcy, 0, 1); //CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合

		lisRectMat.clear();


		cvtColor(gray, gray, COLOR_GRAY2RGBA);

		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
			if (boundRect[i].x > static_cast<int>(giFrame.cols / 2))
			{
				continue;
			}
			//lisRectMat.push_back(giFrame(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].height, static_cast<int>(giFrame.cols*0.9 - boundRect[i].x))));
			lisRectMat.push_back(giFrame(boundRect[i]));
			lisRectMat[lisRectMat.size() - 1].copyTo(gray(boundRect[i]));
		}

		
		//imshow("Genshin R", lis[0]);
		//imshow("Genshin G", lis[1]);
		//imshow("Genshin B", lis[2]);
		imshow("Genshin", giFrame);
		imshow("Genshin A", lis[3]);
		imshow("Genshin Out", lis[3].mul(lis[2])/255.0);
		imshow("Genshin Gray", gray);
#else
		giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 20.0), static_cast<int>(giFrame.rows / 2.0), static_cast<int>(giFrame.cols / 6.0), static_cast<int>(giFrame.rows / 4.0)));

		cv::split(giFrame, lis);

		cv::threshold(lis[3], gray, 36, 255, cv::THRESH_BINARY);

#ifdef b
		imwrite("im" + to_string(i++) + ".png", giFrame);
#endif

		cv::findContours(gray, contours, hierarcy, 0, 1); //CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合

		lisRectMat.clear();


		cvtColor(gray, gray, COLOR_GRAY2RGBA);

		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
			if (boundRect[i].x > static_cast<int>(giFrame.cols / 2))
			{
				continue;
			}
			//lisRectMat.push_back(giFrame(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].height, static_cast<int>(giFrame.cols*0.9 - boundRect[i].x))));
			lisRectMat.push_back(giFrame(boundRect[i]));
			lisRectMat[lisRectMat.size() - 1].copyTo(gray(boundRect[i]));
		}

		imshow("Genshin", giFrame);
		imshow("Genshin A", lis[3]);
		imshow("Genshin Out", lis[3].mul(lis[2]) / 255.0);
		imshow("Genshin Gray", gray);
#endif
		waitKey(42);
	}

	return 0;
}