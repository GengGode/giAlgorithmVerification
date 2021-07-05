#include "Cv.h"

Cv::Cv(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &Cv::a10);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &Cv::a20);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &Cv::a30);

	MainMat = imread("C:/Users/GengG/source/repos/Cv测试/Cv测试/res/mat.png");
}
void Cv::paintEvent(QPaintEvent * event)
{
	//设置画面为地图
	QPainter painter(this);
	if (isE)
	{
		painter.drawImage(0, 0, MainImg);

	}
}

double dis2(double a, double b)
{
	return a * a + b * b;
}

Mat SharRectMat(Mat in,double r, int mode)
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
				ReRoi.at<uchar>(j, i) = 255.0 / (r)*(len - dis);
			}
			else
			{
				ReRoi.at<uchar>(j, i) = 255;
			}
		}
	}
	return in;
}

Mat Cv::calMask(int col, int row, double r, int flag)
{
	Mat out(row, col, CV_8UC1, Scalar(255, 255, 255));
	Rect roi;
	Mat ReRoi;
	int len;

	roi = Rect(0, 0, cvRound(r), cvRound(r));
	ReRoi = out(roi);
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
			else if(dis > len -r)
			{
				ReRoi.at<uchar>(j, i) =  255.0/(r)*( len-dis);
			}
			else
			{
				ReRoi.at<uchar>(j, i) = 255;
			}
		}
	}

	//roi = Rect(cvRound(r), 0, col- cvRound(r*2) , cvRound(r));
	//ReRoi = out(roi);
	//len = min(roi.width, roi.height);
	//for (int i = 0; i < ReRoi.rows; i++)
	//{
	//	for (int j = 0; j < ReRoi.cols; j++)
	//	{
	//		double dis = sqrt(dis2(ReRoi.cols - i, ReRoi.rows - j));
	//		if (dis > len)
	//		{
	//			ReRoi.at<uchar>(i, j) = 0;
	//		}
	//		else if (dis > len - r)
	//		{
	//			ReRoi.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
	//		}
	//		else
	//		{
	//			ReRoi.at<uchar>(i, j) = 255;
	//		}
	//	}
	//}

	roi = Rect(0, r, r * 2, col - r * 2);
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
				out.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, col-r, r, r);
	len = min(roi.width, roi.height);
	for (int i = roi.x; i < roi.x + roi.width; i++)
	{
		for (int j = roi.y; j < roi.y + roi.height; j++)
		{
			double dis = sqrt(dis2(roi.x + roi.width - i, roi.y + roi.height-r  - j));
			if (dis > len)
			{
				out.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				out.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
			}
			else
			{
				out.at<uchar>(i, j) = 255;
			}
		}
	}

	roi = Rect(0, 0, cvRound(r), cvRound(r));
	ReRoi = out(roi);
	len = min(roi.width, roi.height);
	for (int i = 0; i < ReRoi.cols; i++)
	{
		for (int j = 0; j < ReRoi.rows; j++)
		{
			double dis = sqrt(dis2(ReRoi.cols - i, ReRoi.rows - j));
			if (dis > len)
			{
				ReRoi.at<uchar>(i, j) = 0;
			}
			else if (dis > len - r)
			{
				ReRoi.at<uchar>(i, j) = 255.0 / (r)*(len - dis);
			}
			else
			{
				ReRoi.at<uchar>(i, j) = 255;
			}
		}
	}
	return out;
}


void Cv::a10()
{
	Mat MAINMASK = imread("C:/Users/GengG/source/repos/GenshinImpact_AutoMap/GenshinImpact_AutoMap/resource/BackGroundMask.bmp");
	
	std::vector<Mat> mv0;
	std::vector<Mat> mv1;
	//通道分离
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols*(MainMat.channels()), QImage::Format_RGBA8888);
	isE = true;
	update();
}

void Cv::a20()
{
	MainMat = imread("C:/Users/GengG/source/repos/Cv测试/Cv测试/res/mat.png");

	Mat MAINMASK = calMask(MainMat.cols, MainMat.rows, 20);

	std::vector<Mat> mv0;
	std::vector<Mat> mv1;

	mv0.clear();
	mv1.clear();

	//通道分离
	split(MainMat, mv0);
	split(MAINMASK, mv1);
	mv0.push_back(mv1[0]);
	merge(mv0, MainMat);
	MainImg = QImage((uchar*)(MainMat.data), MainMat.cols, MainMat.rows, MainMat.cols*(MainMat.channels()), QImage::Format_ARGB32);
	isE = true;
	update();
}

void Cv::a30()
{
}

