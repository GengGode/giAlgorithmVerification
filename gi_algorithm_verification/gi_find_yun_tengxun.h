#pragma once

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
	//��ȡĿ�����Ĵ��ڴ�СRECT
	//GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */
	GetClientRect(handle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = GetDC(handle);/* ��ԭ�񴰿ڵĲ��� */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat����
	Mat giFrame;
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);
	return giFrame;
}

HWND han = 0x0;
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

void test(HandleImpl* hptr)
{

}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	//GetWindow
	char nameLis[256];
	GetWindowTextA(hwnd, nameLis, 256);
	if (string(nameLis) == "��Ѷ����Ϸ")
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

int main_find()
{
	HWND handle = 0x0;

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
			Mat map = getGiScreen(handle);
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
