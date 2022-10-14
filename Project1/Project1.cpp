// Project1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat getGiScreen(HWND handle)
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (handle == NULL)
		throw exception("error");
	RECT giRect;
	//获取目标句柄的窗口大小RECT
	//GetWindowRect(giHandle, &giRect);/* 对原神窗口的操作 */
	GetClientRect(handle, &giRect);/* 对原神窗口的操作 */

	//获取目标句柄的DC
	HDC hScreen = GetDC(handle);/* 对原神窗口的操作 */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//获取目标句柄的宽度和高度
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//创建Bitmap对象
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//得到位图

	SelectObject(hCompDC, hBmp); //不写就全黑
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//释放对象
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//类型转换
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat操作
	Mat giFrame;
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);
	return giFrame;
}

HWND han=0x0;
BOOL CALLBACK EnumChildProc(
	HWND h,      // handle to child window
	LPARAM l    // application-defined value
)
{
	static int a = 0;
	//HWND han = (HWND)l;
	a++;
	cout << "		: " << h << endl;

	if (a == 2)
	{
		han = h;
		return false;
	}
	return true;
}

class HandleImpl
{
	//HWND inHandle;
public:
	HWND outHandle = NULL;
	virtual bool tryGetHandle(HWND handle) = 0;
	virtual HWND getHandle() = 0;
};

class Handle_GenshinImpact : private HandleImpl
{
public:
	bool tryGetHandle(HWND handle)
	{
		outHandle = handle;
		return IsWindow(outHandle);
	}
	HWND getHandle()
	{
		return outHandle;
	}
};
class Handle_TengXunYunYouXi :private HandleImpl
{
public:
	Handle_TengXunYunYouXi()
	{
		//ptr = this.getHandle_Success;
	}
//	~Handle_TengXunYunYouXi();
public:
	bool tryGetHandle(HWND handle)
	{
		callbackCount = 0;
		HWND parantHandle = GetNextWindow(handle, GW_HWNDNEXT);
		EnumChildWindows(parantHandle, (WNDENUMPROC)Handle_TengXunYunYouXi::EnumGameChildProc, NULL);
		outHandle = callbcakHandle;
		return IsWindow(outHandle);
	}
	HWND getHandle() 
	{
		return outHandle;
	}
private:
	//HWND  (Handle_TengXunYunYouXi::ptr)(void);

	static HWND callbcakHandle;
	static int callbackCount;
private:
	static BOOL CALLBACK EnumGameChildProc(
		HWND handle,      // handle to child window
		LPARAM lpa    // application-defined value
	)
	{
		callbackCount++;
		if (callbackCount == 2)
		{
			callbcakHandle = handle;
			return false;
		}
		return true;
	}

	HWND getHandle_Success()
	{

	}
	HWND getHandle_Fail()
	{

	}
};

void test(HandleImpl *hptr)
{

}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	//GetWindow
	char nameLis[256];
	GetWindowTextA(hwnd, nameLis, 256);
	if (string(nameLis) == "腾讯云游戏")
	{
		RECT r;
		GetClientRect(hwnd, &r);
		cout << r.right - r.left << " , " << r.bottom - r.top << endl;
		if (r.right - r.left > 1000 && r.bottom - r.top > 600)
		{
			cout << hwnd << " | " << nameLis << endl;
			//if()
		}
	}
	return true;
}

int main()
{
	HWND handle=0x0;

	/*Handle_TengXunYunYouXi tx;
	if (tx.tryGetHandle(handle))
	{
		handle=tx.getHandle();
	}*/

	::EnumWindows(EnumWindowsProc, NULL);


	cout << handle << endl;

	//handle = (HWND)0x000413AA;
	//cout << handle << endl;
	//handle = GetNextWindow(handle, GW_HWNDNEXT);
	//cout << handle << endl;
	///*HWND han;*/
	//EnumChildWindows(handle,EnumChildProc, NULL);
	////handle = GetParent(handle);
	//cout << handle << endl;
	//cout << han << endl;
	//handle = han;
	while (1)
	{
		cout << WindowFromPoint(POINT{ 960,540 }) << endl;
		try
		{
			Mat map= getGiScreen(handle);
			cv::imshow("asd", map);
		}
		catch (exception e)
		{
			cout << e.what() << endl;
			break;
		}
		waitKey(100);
	}


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
