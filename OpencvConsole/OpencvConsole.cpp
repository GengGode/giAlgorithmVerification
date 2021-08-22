// OpencvConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "CodeTest.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int showNumber = 0;

void show(Mat img)
{
	string str = "Img" + to_string(showNumber);
	namedWindow(str, WINDOW_FREERATIO);
	imshow(str, img);
	showNumber++;
}

void unevenLightCompensate(Mat &image, int blockSize)
{
	if (image.channels() == 3) cvtColor(image, image, 7);
	double average = mean(image)[0];
	int rows_new = ceil(double(image.rows) / double(blockSize));
	int cols_new = ceil(double(image.cols) / double(blockSize));
	Mat blockImage;
	blockImage = Mat::zeros(rows_new, cols_new, CV_32FC1);
	for (int i = 0; i < rows_new; i++)
	{
		for (int j = 0; j < cols_new; j++)
		{
			int rowmin = i * blockSize;
			int rowmax = (i + 1)*blockSize;
			if (rowmax > image.rows) rowmax = image.rows;
			int colmin = j * blockSize;
			int colmax = (j + 1)*blockSize;
			if (colmax > image.cols) colmax = image.cols;
			Mat imageROI = image(Range(rowmin, rowmax), Range(colmin, colmax));
			double temaver = mean(imageROI)[0];
			blockImage.at<float>(i, j) = temaver;
		}
	}
	blockImage = blockImage - average;
	Mat blockImage2;
	resize(blockImage, blockImage2, image.size(), (0, 0), (0, 0), INTER_CUBIC);
	Mat image2;
	image.convertTo(image2, CV_32FC1);
	Mat dst = image2 - blockImage2;
	dst.convertTo(image, CV_8UC1);
}
//求区域内均值 integral即为积分图
float fastMean(cv::Mat& integral, int x, int y, int window)
{

	int min_y = std::max(0, y - window / 2);
	int max_y = std::min(integral.rows - 1, y + window / 2);
	int min_x = std::max(0, x - window / 2);
	int max_x = std::min(integral.cols - 1, x + window / 2);

	int topright = integral.at<int>(max_y, max_x);
	int botleft = integral.at<int>(min_y, min_x);
	int topleft = integral.at<int>(max_y, min_x);
	int botright = integral.at<int>(min_y, max_x);

	float res = (float)((topright + botleft - topleft - botright) / (float)((max_y - min_y) *(max_x - min_x)));

	return res;
}


cv::Mat& Sauvola(cv::Mat& inpImg, cv::Mat& resImg, int window, float k)
{
	cv::Mat integral;
	int nYOffSet = 3;
	int nXOffSet = 3;
	cv::integral(inpImg, integral);  //计算积分图像
	for (int y = 0; y < inpImg.rows; y += nYOffSet)
	{
		for (int x = 0; x < inpImg.cols; x += nXOffSet)
		{

			float fmean = fastMean(integral, x, y, window); float fthreshold = (float)(fmean*(1.0 - k));

			int nNextY = y + nYOffSet;
			int nNextX = x + nXOffSet;
			int nCurY = y;
			while (nCurY < nNextY && nCurY < inpImg.rows)
			{
				int nCurX = x;
				while (nCurX < nNextX && nCurX < inpImg.cols)
				{
					uchar val = inpImg.at<uchar>(nCurY, nCurX) < fthreshold;
					resImg.at<uchar>(nCurY, nCurX) = (val == 0 ? 0 : 255);
					nCurX++;
				}
				nCurY++;
			}

		}
	}

	return resImg;
}

void test1(Mat &in, Mat &out)
{
	Mat src(in.size(), CV_32FC3);
	for (int i = 0; i < in.rows; i++)
	{
		for (int j = 0; j < in.cols; j++)
		{
			src.at<Vec3f>(i, j)[0] = 255 * (float)in.at<Vec3b>(i, j)[0] / ((float)in.at<Vec3b>(i, j)[0] + (float)in.at<Vec3b>(i, j)[2] + (float)in.at<Vec3b>(i, j)[1] + 0.01);
			src.at<Vec3f>(i, j)[1] = 255 * (float)in.at<Vec3b>(i, j)[1] / ((float)in.at<Vec3b>(i, j)[0] + (float)in.at<Vec3b>(i, j)[2] + (float)in.at<Vec3b>(i, j)[1] + 0.01);
			src.at<Vec3f>(i, j)[2] = 255 * (float)in.at<Vec3b>(i, j)[2] / ((float)in.at<Vec3b>(i, j)[0] + (float)in.at<Vec3b>(i, j)[2] + (float)in.at<Vec3b>(i, j)[1] + 0.01);
		}
	}

	normalize(src, src, 0, 255,NORM_MINMAX);

	convertScaleAbs(src, out);
}

void test2(Mat &in, Mat &out)
{
	Mat src(in.size(), CV_32FC3);
	for (int i = 0; i < in.rows; i++)
	{
		for (int j = 0; j < in.cols; j++)
		{
			float k = min((float)in.at<Vec3b>(i, j)[0], min((float)in.at<Vec3b>(i, j)[2], (float)in.at<Vec3b>(i, j)[1]));
			src.at<Vec3f>(i, j)[0] = k;
			src.at<Vec3f>(i, j)[1] = k;
			src.at<Vec3f>(i, j)[2] = k;
		}
	}

	normalize(src, src, 0, 255, NORM_MINMAX);

	convertScaleAbs(src, out);
}

Point asd(Mat image)
{
	Mat img, imgGray;
	Mat img2, imgGraydst;
	image.copyTo(img);
	cvtColor(img, imgGray, 7);
	threshold(imgGray, imgGraydst, 230, 255, THRESH_BINARY);

	cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
	cv::dilate(imgGraydst, imgGraydst, dilate_element);
	cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));
	cv::erode(imgGraydst, imgGraydst, erode_element);

	imgGray = imgGray - imgGraydst;
	cvtColor(imgGraydst, imgGraydst, COLOR_GRAY2RGB);
	img = img - imgGraydst;
	unevenLightCompensate(img, 2);


	show(img);

	Mat out = (imgGray - img) * 5;

	threshold(out, out, 130, 255, THRESH_BINARY);

	dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(out, out, dilate_element);
	erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20));
	cv::erode(out, out, erode_element);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::findContours(out, contours, hierarcy, 0, 1);

	std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合
	cv::Point2f rect[4];

	std::vector<cv::Point2d> AvatarKeyPoint;
	double AvatarKeyPointLine[3] = { 0 };
	std::vector<cv::Point2f> AvatarKeyLine;
	cv::Point2f KeyLine;

	Point p;

	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
		AvatarKeyPoint.push_back(cv::Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));

		p=Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y + boundRect[i].height / 2);

		circle(out, p, 3, Scalar(0));

	}

	show(out);

	return p;

}

int main()
{
	Mat imgOri=imread("001.png");

	namedWindow("Img", WINDOW_FREERATIO);
	imshow("Img", imgOri);
	
	Mat img;
	imgOri(Rect(30, 30,152, 152)).copyTo(img);
	/*************************************/
	img.copyTo(imgOri);

	Mat imgGray;

	cvtColor(img, imgGray, 7);

	show(imgGray);

	double res;
	

	Mat img2, imgGraydst;

	cvtColor(img, imgGray, 7);
	threshold(imgGray, imgGraydst, 230, 255, THRESH_BINARY);

	cv::Mat dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
	cv::dilate(imgGraydst, imgGraydst, dilate_element);
	cv::Mat erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));
	cv::erode(imgGraydst, imgGraydst, erode_element);

	imgGray = imgGray - imgGraydst;
	cvtColor(imgGraydst, imgGraydst, COLOR_GRAY2RGB);
	img = img - imgGraydst;
	unevenLightCompensate(img, 2);


	show(img);

	Mat out = (imgGray - img) * 5;

	threshold(out, out, 130, 255, THRESH_BINARY);

	dilate_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(out, out, dilate_element);
	erode_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20));
	cv::erode(out, out, erode_element);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	cv::findContours(out, contours, hierarcy, 0, 1);

	std::vector<cv::Rect> boundRect(contours.size());  //定义外接矩形集合
	cv::Point2f rect[4];

	std::vector<cv::Point2d> AvatarKeyPoint;
	double AvatarKeyPointLine[3] = { 0 };
	std::vector<cv::Point2f> AvatarKeyLine;
	cv::Point2f KeyLine;

	Point p;

	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
		AvatarKeyPoint.push_back(cv::Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));

		p = Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y + boundRect[i].height / 2);

		circle(out, p, 3, Scalar(0));

	}



	circle(imgOri, p, 3, Scalar(255, 0, 0));
	line(imgOri, p, Point(img.cols / 2, img.rows / 2), Scalar(0, 255, 0));
	imshow("Img", imgOri);
	p = p - Point(img.cols / 2, img.rows / 2);
	const double rad2degScale = 180 / CV_PI;
	res = atan2(-p.y, p.x)*rad2degScale;
	res = res - 90; //从屏幕空间左侧水平线为0度转到竖直向上为0度
	std::cout << res;


	waitKey(0);
    std::cout << "Hello World!\n";
}

//let src = cv.imread("preview"');
//	cv.cvtColor(src, src, cv.COLOR_RGB2HSV, e);
//let hsv = new Cv.MatVector(;
//cv.split(src， hsv);
//let dst = new cv.Mat(); let mask = new cv.Mat(); let dtype = -1;
//cv.subtract(hsv.get(1)， hsv.get(2), dst, mask，dtype); cv.bitwise_not(dst, dst)
//cv.threshold(dst，dst，254，255, cv.THRESH_BINARY)cv.imshow(" output_canvas ", dst);
//

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
