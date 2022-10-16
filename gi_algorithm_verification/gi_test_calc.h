#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include <Windows.h>

using namespace std;
using namespace cv;

LPCWSTR giWindowName = { L"ԭ��" };
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

	// ��ȡ�������߼������߶�
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// ��ȡ��������������߶�
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

	//��ȡĿ�����Ĵ��ڴ�СRECT
	GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = static_cast<int>((screen_scale) * (giRect.right - giRect.left));
	int	nHeight = static_cast<int>((screen_scale) * (giRect.bottom - giRect.top));

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	//�����ȡλͼ�Ĵ�С��Ϣ,��ʵ��Ҳ�Ǽ���DC��ͼ����ķ�Χ
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat����
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));

	if (giFrame.empty())
	{
		return false;
	}
	return true;
}
void func_0()
{
	int MiniMap_Rect_x = cvRound(1920 * 0.033);
	int MiniMap_Rect_y = cvRound(1920 * 0.01);
	int MiniMap_Rect_w = cvRound(1920 * 0.11);
	int MiniMap_Rect_h = cvRound(1920 * 0.11);

	if (giFrame.cols == 2560 && giFrame.rows == 1080)
	{
		MiniMap_Rect_x = cvRound(1920 * 0.033 + 72);
		MiniMap_Rect_y = cvRound(1920 * 0.01);
		MiniMap_Rect_w = cvRound(1920 * 0.11);
		MiniMap_Rect_h = cvRound(1920 * 0.11);
	}
	
	std::vector<cv::Mat> split_gi_frame;
	split(giFrame, split_gi_frame);

	cv::Mat test_roi;
	split_gi_frame[3](cv::Rect(MiniMap_Rect_x, MiniMap_Rect_y, MiniMap_Rect_w, MiniMap_Rect_h)).copyTo(test_roi);

	// ��Ե��ȡ
	cv::Mat canny;
	cv::Canny(test_roi, canny, 80, 200);

	// �ڱ�Եͼ���Ļ��ư뾶Ϊ80�ĺ�ɫ����
	cv::circle(canny, cv::Point(canny.cols / 2, canny.rows / 2), 80, cv::Scalar(0), -1);

	cv::Mat mask = cv::Mat::zeros(canny.size(), CV_8UC1);
	cv::circle(mask, cv::Point(canny.cols / 2, canny.rows / 2), 96, cv::Scalar(255), -1);
	cv::circle(mask, cv::Point(canny.cols / 2, canny.rows / 2), 80, cv::Scalar(0), -1);

	cv::Mat mask_canny;
	canny.copyTo(mask_canny, mask);
	// ����һ�����Ͳ���
	cv::dilate(mask_canny, mask_canny, cv::Mat(), cv::Point(-1, -1), 1);


	// 83-194
	// ��ֵ�ָ���ȡ����83��194����ɫ��
	cv::Mat test_roi_gray;
	test_roi.copyTo(test_roi_gray);
	cv::Mat test_roi_binary;
	cv::threshold(test_roi_gray, test_roi_binary, 83, 255, cv::THRESH_BINARY);

	cv::circle(test_roi_binary, cv::Point(test_roi_binary.cols / 2, test_roi_binary.rows / 2), 86, cv::Scalar(255), 5);

	cv::Mat test_roi_binary_inv;
	cv::threshold(test_roi_gray, test_roi_binary_inv, 194, 255, cv::THRESH_BINARY_INV);
	cv::circle(test_roi_binary_inv, cv::Point(test_roi_binary_inv.cols / 2, test_roi_binary_inv.rows / 2), 80, cv::Scalar(0), -1);

	cv::Mat test_roi_binary_final;
	cv::bitwise_and(test_roi_binary, test_roi_binary_inv, test_roi_binary_final);

	cv::Mat mask_test_roi_binary_final;
	test_roi_binary_final.copyTo(mask_test_roi_binary_final, mask);

	cv::imshow("mask_test_roi_binary_final", mask_test_roi_binary_final);
	cv::Mat canny_mask_binary_final;
	cv::bitwise_and(mask_canny, mask_test_roi_binary_final, canny_mask_binary_final);
	cv::imshow("canny_mask_binary_final", canny_mask_binary_final);

	cv::Mat sum_mask;
	sum_mask = mask_test_roi_binary_final + mask_canny;
	cv::circle(sum_mask, cv::Point(sum_mask.cols / 2, sum_mask.rows / 2), 86, cv::Scalar(0), 12);

	cv::imshow("sum_mask", sum_mask);
	// ��һ�����Ͳ��� �뾶15
	cv::Mat sum_mask_dilate;
	cv::dilate(sum_mask, sum_mask_dilate, cv::Mat(), cv::Point(-1, -1), 15);
	// ��һ�θ�ʴ���� �뾶10
	cv::Mat sum_mask_erode;
	cv::erode(sum_mask_dilate, sum_mask_erode, cv::Mat(), cv::Point(-1, -1), 10);
	cv::imshow("sum_mask_erode", sum_mask_erode);

	// test_roi
			// ��������Ϊtest_roi����sum_mask_erodeΪ���ֵĲ���
	cv::Mat test_roi_mask;
	giFrame(cv::Rect(MiniMap_Rect_x, MiniMap_Rect_y, MiniMap_Rect_w, MiniMap_Rect_h)).copyTo(test_roi_mask, sum_mask_erode);

	cv::imshow("test_roi_mask", test_roi_mask);

	// ����ɫ������
	std::vector<std::vector<cv::Point>> N_contours;
	cv::findContours(sum_mask_erode, N_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	// ��������
	cv::Mat sum_mask_erode_contours = test_roi_mask.clone();

	cv::drawContours(sum_mask_erode_contours, N_contours, -1, cv::Scalar(0, 0, 255), 2);

	// �����������ĵ�ͼƬ���ĵ����� ��ͼƬ�л���ÿ�����ߵĽǶ�
	for (int i = 0; i < N_contours.size(); i++)
	{
		cv::Moments mu = cv::moments(N_contours[i]);
		cv::Point2d mc = cv::Point2d(mu.m10 / mu.m00, mu.m01 / mu.m00);
		cv::Point2d mc_center = cv::Point2d(sum_mask_erode_contours.cols / 2.0, sum_mask_erode_contours.rows / 2.0);
		double angle = atan2(mc.y - mc_center.y, mc.x - mc_center.x) * 180 / CV_PI;
		std::string angle_str = std::to_string(static_cast<int>(angle + 90));
		cv::putText(sum_mask_erode_contours, angle_str, mc, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
	}
	cv::imshow("sum_mask_erode_contours", sum_mask_erode_contours);

	//// test_roi
	//// ��������Ϊtest_roi����sum_mask_erodeΪ���ֵĲ���
	//cv::Mat test_roi_mask;
	//test_roi.copyTo(test_roi_mask, sum_mask_erode);

	// ����Ŀ��ģ���ͼƬ "C:\Users\GengG\source\repos\giAlgorithmVerification\gi_algorithm_verification\UI_MiniMap_MarkSouth.png"
	cv::Mat N_template = cv::imread("C:/Users/GengG/source/repos/giAlgorithmVerification/gi_algorithm_verification/UI_MiniMap_MarkSouth.png", -1);
	cv::Mat N2_template = cv::imread("C:/Users/GengG/source/repos/giAlgorithmVerification/gi_algorithm_verification/out.png", -1);
	// ���ģ��ͼƬ��ֻҪ����͸��ͨ��
	std::vector<cv::Mat> N_template_channels;
	cv::split(N_template, N_template_channels);
	cv::Mat N_template_mask = 255 - N_template_channels[3];
	N_template_mask = N2_template;
	// ģ����СΪԭ����75%
	//cv::resize(N_template_mask, N_template_mask, cv::Size(), 0.75, 0.75);


	// ��ֱ�ƥ��ͼƬ��ֻҪ����͸��ͨ�� test_roi_mask
	std::vector<cv::Mat> test_roi_mask_channels;
	cv::split(test_roi_mask, test_roi_mask_channels);
	cv::Mat test_roi_mask_mask = test_roi_mask_channels[3];



	std::vector<std::pair<double, double>> angle_maxVal_list;

	// ����ÿһ���������������ߵĽǶ���תģ�壬����ƥ��
	for (int i = 0; i < N_contours.size(); i++)
	{
		cv::Moments mu = cv::moments(N_contours[i]);
		cv::Point2d mc = cv::Point2d(mu.m10 / mu.m00, mu.m01 / mu.m00);
		cv::Point2d mc_center = cv::Point2d(sum_mask_erode_contours.cols / 2.0, sum_mask_erode_contours.rows / 2.0);
		double angle = atan2(mc.y - mc_center.y, mc.x - mc_center.x) * 180 / CV_PI + 90;
		// ���ݽǶ���תģ��ͼƬ
		cv::Mat N_template_rotate;
		cv::Point2d center(N_template_mask.cols / 2.0, N_template_mask.rows / 2.0);
		cv::Mat rot = cv::getRotationMatrix2D(center, -angle, 1.0);
		cv::warpAffine(N_template_mask, N_template_rotate, rot, N_template_mask.size(), 1, 0, cv::Scalar(0));

		// ����200��Ϊ��ֵ��ֵ����ƥ��ģ����Ĥ, ����200�Ķ����0
		cv::Mat N_template_rotate_binary;
		cv::threshold(N_template_rotate, N_template_rotate_binary, 1, 255, cv::THRESH_BINARY);
		cv::Mat N_template_rotate_binary2 = N_template_rotate_binary * 255;

		//N_template_rotate = 255 - N_template_rotate;

		// ƥ��ģ��
		cv::Mat result;
		cv::matchTemplate(test_roi_mask_mask, N_template_rotate, result, cv::TM_CCOEFF_NORMED, N_template_rotate_binary);
		// �޳�����е�inf
		cv::Mat result2;
		result.copyTo(result2);
		for (int i = 0; i < result2.rows; i++)
		{
			for (int j = 0; j < result2.cols; j++)
			{
				if (result2.at<float>(i, j) == std::numeric_limits<float>::infinity())
				{
					result2.at<float>(i, j) = 0;
				}
			}
		}
		result = result2;
		cv::imshow("result", result);
		// �������ֵ����Сֵ
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
		// �������ֵ��λ�ã����ƾ���
		cv::rectangle(sum_mask_erode_contours, maxLoc, cv::Point(maxLoc.x + N_template_rotate.cols, maxLoc.y + N_template_rotate.rows), cv::Scalar(0, 255, 0), 2);
		// �����ֵ������ͼƬ��
		cv::putText(sum_mask_erode_contours, std::to_string(maxVal), maxLoc, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
		angle_maxVal_list.push_back({ angle,maxVal });
	}
	cv::cvtColor(sum_mask_erode_contours, sum_mask_erode_contours, cv::COLOR_RGBA2RGB);
	cv::imshow("sum_mask_erode_contours", sum_mask_erode_contours);

	// ��ƥ�������ҵ����ֵ
	double maxVal = 0;
	double maxVal_angle = 0;
	for (int i = 0; i < angle_maxVal_list.size(); i++)
	{
		if (angle_maxVal_list[i].second > maxVal)
		{
			maxVal = angle_maxVal_list[i].second;
			maxVal_angle = angle_maxVal_list[i].first;
		}
	}
	if (maxVal > 0.85)
	{
		std::cout << "ƥ��ɹ����Ƕ�Ϊ��" << maxVal_angle << std::endl;
	}
	else
	{
		std::cout << "ƥ��ʧ��" << std::endl;
	}
}
void func_1()
{
	std::vector<cv::Mat> lis;
	std::vector<cv::Mat> lisRectMat;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::Mat gray;
	int i = 0;

	bool isSave = true;
	int saveCount = 0;
	
	//giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 5.0*3.0), static_cast<int>(giFrame.rows / 4.0), static_cast<int>(giFrame.cols / 5.0), static_cast<int>(giFrame.rows / 2.0)));
	giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 5.0 * 3.0), static_cast<int>(giFrame.rows / 4.0), static_cast<int>(giFrame.cols / 6.0), static_cast<int>(giFrame.rows / 2.0)));

	cv::split(giFrame, lis);

	cv::threshold(lis[3], gray, 100, 255, cv::THRESH_BINARY);

	//cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
	//cv::erode(gray, gray, erode_element);
	//cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
	//cv::dilate(gray, gray, dilate_element);

	cv::findContours(gray, contours, hierarcy, 0, 1); //CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<cv::Rect> boundRect(contours.size());  //������Ӿ��μ���

	lisRectMat.clear();


	cvtColor(gray, gray, COLOR_GRAY2RGBA);

	if (contours.size() == 0)
	{
		isSave = true;
	}
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
		//imwrite("save/out_" + to_string(saveCount) + ".png", lisRectMat[lisRectMat.size()-1]);
		saveCount++;
	}
	if (contours.size() != 0)
	{
		isSave = false;
	}

	//imshow("Genshin R", lis[0]);
	//imshow("Genshin G", lis[1]);
	//imshow("Genshin B", lis[2]);
	cv::imshow("Genshin", giFrame);
	cv::imshow("Genshin A", lis[3]);
	cv::imshow("Genshin Out", lis[3].mul(lis[2]) / 255.0);
	cv::imshow("Genshin Gray", gray);
}
void func_2()
{
	std::vector<cv::Mat> lis;
	std::vector<cv::Mat> lisRectMat;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::Mat gray;
	int i = 0;

	bool isSave = true;
	int saveCount = 0;
	
	//��ȡ��Ϸ�������Ʒ�б�����
	giFrame = giFrame(Rect(static_cast<int>(giFrame.cols / 20.0), static_cast<int>(giFrame.rows / 2.0), static_cast<int>(giFrame.cols / 6.0), static_cast<int>(giFrame.rows / 3.0)));

	//ͼ��ͨ������
	cv::split(giFrame, lis);

	// <=51 ��ֵ����ֵ
	cv::threshold(lis[3], gray, 36, 255, cv::THRESH_BINARY);

#ifdef b
	//д���ļ�
	//imwrite("im" + to_string(i++) + ".png", giFrame);
#endif

		//������ͨ����ĵ㼯
	cv::findContours(gray, contours, hierarcy, 0, 1); //CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<cv::Rect> boundRect(contours.size());  //������Ӿ��μ���

	lisRectMat.clear();

	//�ָ��±���ͨ����
	cvtColor(gray, gray, COLOR_GRAY2RGBA);

	for (int i = 0; i < contours.size(); i++)
	{
		//��ȡ�㼯����С��Ӿ���
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
		if (boundRect[i].x > static_cast<int>(giFrame.cols / 2))
		{
			continue;
		}
		//lisRectMat.push_back(giFrame(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].height, static_cast<int>(giFrame.cols*0.9 - boundRect[i].x))));
		lisRectMat.push_back(giFrame(boundRect[i]));

		//�������RGBͨ�����Ƶ���ʾͼ��
		lisRectMat[lisRectMat.size() - 1].copyTo(gray(boundRect[i]));
	}

	cv::imshow("Genshin", giFrame);
	cv::imshow("Genshin A", lis[3]);
	cv::imshow("Genshin Out", lis[3].mul(lis[2]) / 255.0);
	cv::imshow("Genshin Gray", gray);
}
int test_calc()
{
	giWindowName = L"\u539F\u795E";
	giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* ƥ�����ƣ�ԭ�� */
	
	//��ȡ��Ļ���ű���
	getGengshinImpactScale();

	giClientSize.width = static_cast<int>(screen_scale * (giClientRect.right - giClientRect.left));
	giClientSize.height = static_cast<int>(screen_scale * (giClientRect.bottom - giClientRect.top));
	
#define b0
#ifdef b
	while (getGengshinImpactScreen())
	{
		int& x = giFrame.cols, & y = giFrame.rows;
		double f = 1, fx = 1, fy = 1;

		if (static_cast<double>(x) / static_cast<double>(y) == 16.0 / 9.0)
		{

			//��������������
			if (x != 1920 && y != 1080)
			{
				cv::resize(giFrame, giFrame, cv::Size(1920, 1080));
			}
		}
		else if (static_cast<double>(x) / static_cast<double>(y) > 16.0 / 9.0)
		{

			//���ͣ��Կ�Ϊ����

			// x = (y * 16) / 9;
			f = y / 1080.0;
			//��giFrame���ŵ�1920*1080�ı���
			fx = x / f;
			// ��ͼƬ����
			cv::resize(giFrame, giFrame, cv::Size(static_cast<int>(fx), 1080));

		}
		else if (static_cast<double>(x) / static_cast<double>(y) < 16.0 / 9.0)
		{

			//���ͣ��Ը�Ϊ����

			// x = (y * 16) / 9;
			f = x / 1920.0;
			//��giFrame���ŵ�1920*1080�ı���
			fy = y / f;
			// ��ͼƬ����
			cv::resize(giFrame, giFrame, cv::Size(1920, static_cast<int>(fy)));
		}
#else
	while (1)
	{
		giFrame = imread("img.png", -1);

#endif
		func_0();


		if (1)
		{
			func_1();
		}
		else
		{
			func_2();
		}

		waitKey(42);
	}

	return 0;
}

