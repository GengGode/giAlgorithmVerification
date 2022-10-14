#pragma once
#include <opencv2/opencv.hpp>

cv::Mat create_square_mask(int mask_width, int mask_height, double gradient_width)
{
	cv::Mat mask = cv::Mat::zeros(mask_height, mask_width, CV_8UC1);
	cv::Point2d center(mask_width / 2, mask_height / 2);
	double radius = mask_width / 2;
	double gradient_radius = radius - gradient_width;
	for (int i = 0; i < mask_height; i++)
	{
		for (int j = 0; j < mask_width; j++)
		{
			double distance = cv::norm(center - cv::Point2d(j, i));
			if (distance < gradient_radius)
			{
				mask.at<uchar>(i, j) = 255;
			}
			else if (distance < radius)
			{
				mask.at<uchar>(i, j) = 255 * (radius - distance) / gradient_width;
			}
		}
	}
	return mask;
}


using namespace cv;
using namespace std;

double dis2(double a, double b)
{
	return a * a + b * b;
}

Mat calMask(int col, int row, double r, int flag = 0);
Mat calMask2(int col, int row, int r, double r0, int flag = 0);
Mat calMask3(int col, int row, int r, double r0, int flag = 0);

Mat SharRectMat(Mat in, double r, int mode)
{
	//Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));
	Rect roi;
	Mat ReRoi;
	int len;

	roi = Rect(0, 0, cvRound(r), cvRound(r));
	ReRoi = in(roi);
	len = min(roi.width, roi.height);
	for (int i = 0; i < ReRoi.cols; i++)
	{
		for (int j = 0; j < ReRoi.rows; j++)
		{
			double dis = sqrt(dis2(ReRoi.cols - i, ReRoi.rows - j));

			if (dis > len)
			{
				ReRoi.at<uchar>(j, i) = 0;
			}
			else if (dis > len - r)
			{
				ReRoi.at<uchar>(j, i) = 255.0 / (r) * (len - dis);
			}
			else
			{
				ReRoi.at<uchar>(j, i) = 255;
			}
		}
	}
	return in;
}

Mat calMask3(int col, int row, int r, double r0, int flag)
{
	Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));

	int len = min(cvRound(col * 0.5), cvRound(row * 0.5));
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			double dis = sqrt(dis2(cvRound(col * 0.5) - i, cvRound(row * 0.5) - j));
			if (dis > len)
			{
				out.at<uchar>(j, i) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(j, i) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(j, i) = 255;
			}
		}
	}
	return out;
}

Mat calMask2(int col, int row, int r, double r0, int flag)
{
	Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));
	Rect roi;
	Mat ReRoi;
	int len;

	roi = Rect(0, 0, cvRound(r + r0), cvRound(r + r0));
	ReRoi = out(roi);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, (r + r0), (r + r0), col - (r + r0) * 2);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, 0));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, col - (r + r0), (r + r0), (r + r0));
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height - (r + r0) - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect((r + r0), 0, row - (r + r0) * 2, (r + r0));
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(0, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect((r + r0), col - (r + r0), row - (r + r0) * 2, (r + r0));
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(0, roi.y + roi.height - (r + r0) - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(row - (r + r0), 0, cvRound(r + r0), cvRound(r + r0));
	ReRoi = out(roi);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - (r + r0) - i, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(row - (r + r0), (r + r0), (r + r0), col - (r + r0) * 2);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - (r + r0) - i, 0));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(row - (r + r0), col - (r + r0), (r + r0), (r + r0));
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - (r + r0) - i, roi.y + roi.height - (r + r0) - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}
	return out;
}

Mat calMask(int col, int row, double r, int flag)
{
	Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));
	Rect roi;
	Mat ReRoi;
	int len;

	roi = Rect(0, 0, cvFloor(r), cvFloor(r));
	ReRoi = out(roi);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, r, r, col - r * 2);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, 0));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, col - r, r, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height - r - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(r, 0, row - r * 2, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(0, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(r, col - r, row - r * 2, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(0, roi.y + roi.height - r - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}
	//std::cout << cvFloor(row - r) << std::endl<< out.rows<< " "<<cvFloor(row - r)+ cvFloor(r) << std::endl;

	roi = Rect(0, cvFloor(row - r), cvFloor(r), cvFloor(r));
	std::cout << roi << std::endl;
	std::cout << out.size() << std::endl;

	ReRoi = out(roi);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - r - i, roi.y + roi.height - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(row - r, r, r, col - r * 2);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - r - i, 0));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(row - r, col - r, r, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - r - i, roi.y + roi.height - r - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r) * (len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}
	return out;
}


void a10()
{
	auto MainMat = imread("C:/Users/GengG/source/repos/Cv≤‚ ‘/Cv≤‚ ‘/res/mat.png");

	Mat MAINMASK = calMask(MainMat.cols, MainMat.rows, 20);

	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	mv0.clear();
	mv1.clear();

	//Õ®µ¿∑÷¿Î
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	//MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols * (MainMat.channels()), QImage::Format_ARGB32);
	//isE = true;
	//update();
}

void a20()
{
	auto MainMat = imread("C:/Users/GengG/source/repos/Cv≤‚ ‘/Cv≤‚ ‘/res/mat2.png");

	Mat rotation0 = getRotationMatrix2D(Point2f(0, 0), 0, 1);//º∆À„∑≈…‰±‰ªªæÿ’Û

	double b[2][3] = { {1,0,110.5 },{0,1,10.3} };

	Mat buf(2, 3, CV_64FC1, b[0]);

	warpAffine(MainMat, MainMat, buf, MainMat.size());

	Mat MAINMASK = calMask2(MainMat.cols, MainMat.rows, 20, 50);

	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	mv0.clear();
	mv1.clear();

	//Õ®µ¿∑÷¿Î
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	//MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols * (MainMat.channels()), QImage::Format_ARGB32);
	//isE = true;
	//update();
}

void a30()
{
	auto MainMat = imread("C:/Users/GengG/source/repos/Cv≤‚ ‘/Cv≤‚ ‘/res/mat.png");

	Mat MAINMASK = calMask3(MainMat.cols, MainMat.rows, 20, 3);

	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	mv0.clear();
	mv1.clear();

	//Õ®µ¿∑÷¿Î
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	//MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols * (MainMat.channels()), QImage::Format_ARGB32);
	//isE = true;
	//update();
}